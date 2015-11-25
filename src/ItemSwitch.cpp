#include "ItemSwitch.hpp"

ItemSwitch::ItemSwitch(int numThreads, int nparts, Slicer * s, RingStack * rs)
{
	ringStack = rs;
	NTHREADS = numThreads;
	NPARTS = nparts;
	slicer = s;
	running = 0;
	numPStaks = 0;
	numItems = 0;
	maxThreshold = 5;
	maxItemWait = 0;
	totalWait = 0;
	if (!omp_get_nested()) {
		omp_set_nested(1);
	}
}

int ItemSwitch::addPluginStack(PluginStack * ps)
{
	pluginStack.push_back(ps);
	numPStaks++;
	return 0;
}

int ItemSwitch::run()
{
	int i, t;
	running = 1;
	Item *item;
	Item **p;
	int threads = NTHREADS;
#pragma omp parallel num_threads(NTHREADS + 1)
#pragma omp single
	while (running) {
		item = NULL;
		if (ringStack->getSize() > 0 && (threads > 0)) {
#pragma omp critical (RingStack)
			{
				item = ringStack->get();
			}
		}
		if (item != NULL) {
			p = slicer->slice(item, NPARTS);
#pragma omp task private(t) firstprivate(p, item)
			if (running && (omp_get_wtime() - item->time) <
			    maxThreshold) {
				bool tooLate = false;
				double dtime;
				for (t = 0; t < NPARTS; t++)
#pragma omp task private(i) firstprivate(t)
				{
					double dt;
#pragma omp atomic
					threads--;
					dt = omp_get_wtime() - item->time;
					if (dt > maxThreshold) {
						tooLate = true;
					}
					for (i = 0; i < numPStaks &&
					     !tooLate; i++) {
						pluginStack[i]->process(p[t]);
					}
					slicer->delPart(p[t]);
#pragma omp atomic
					threads++;
				}

#pragma omp taskwait
#pragma omp atomic
				threads--;
				delete p;
				dtime = omp_get_wtime() - item->time;
				if (!tooLate && dtime < maxThreshold) {
#pragma omp atomic
					totalWait = totalWait + dtime;
					if (maxItemWait < dtime) {
#pragma omp critical
						if (maxItemWait < dtime) {
							maxItemWait = dtime;
						}
					}
#pragma omp atomic
					numItems++;
				}
				delete item;
#pragma omp atomic
				threads++;
			} else {
#pragma omp atomic
				threads--;
				delete item;
#pragma omp atomic
				threads++;
			}
		}
	}
#pragma omp taskwait
	return 0;
}

int ItemSwitch::stop()
{
	running = 0;
	return 0;
}

int ItemSwitch::setThreshold(double t)
{
	maxThreshold = t;
	return 0;
}
