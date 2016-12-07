#include "ItemSwitch.hpp"
#include <stdio.h>

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
	omp_set_dynamic(0);
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
	running = 1;
	Item *item;
	int threads = NTHREADS;
	bool ignore = true;
#pragma omp parallel num_threads(NTHREADS + 1) default(shared)
#pragma omp master
	while (running) {
		item = NULL;
		if (ringStack->getSize() > 0 && (threads > 0)) {
#pragma omp critical (RingStack)
			{
				item = ringStack->get();
			}
		}
		if (item != NULL) {
#pragma omp atomic
			threads--;
#pragma omp task firstprivate(item) default(shared)
			if (running && (omp_get_wtime() - item->time) <
			    maxThreshold) {
				bool tooLate = false;
				double dtime;
				int t;
				Item **p;
				int deltaThreads;
				deltaThreads = NPARTS - 1;
				p = slicer->slice(item, NPARTS);
#pragma omp atomic
				threads = threads - deltaThreads;
				for (t = 0; t < NPARTS; t++)
#pragma omp task firstprivate(t) default(shared)
				{
					double dt;
					int i;
					dt = omp_get_wtime() - item->time;
					if (dt > maxThreshold) {
						tooLate = true;
					}
					for (i = 0; i < numPStaks &&
					     !tooLate; i++) {
						slicer->resetItem(p[t]);
						pluginStack[i]->process(p[t]);
						dt = omp_get_wtime() -
						    item->time;
						if (dt > maxThreshold) {
							tooLate = true;
						}
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
				if (dtime > maxThreshold) {
					tooLate = true;
				}
				if (ignore) {
#pragma omp critical (itemIgnore)
					ignore = Item::ignore;
				}
				if (!tooLate && !ignore) {
#pragma omp atomic
					totalWait = totalWait + dtime;
					if (maxItemWait < dtime) {
#pragma omp critical (maxItemWait)
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
