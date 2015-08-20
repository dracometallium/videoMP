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
	double dtime;
	running = 1;
	Item *item;
	dtime = 0;
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
#pragma omp task private(t) firstprivate(item) if((threads = (threads - 1)) < 0)
			if (running && (omp_get_wtime() - item->time) <
			    maxThreshold) {
				Item **p;
				p = slicer->slice(item, NPARTS);
				for (t = 0; t < NPARTS; t++) {
#pragma omp task private(i) firstprivate(t) if((threads = (threads - 1)) < 0)
					{
						for (i = 0; i < numPStaks; i++) {
							pluginStack[i]->process
							    (p[t]);
						}
						delete p[t];
#pragma omp atomic
						threads++;
					}
				}
#pragma omp atomic
				threads++;
	/*
	 * There is no point in taking away a thread if it is just going to
	 * wait.
	 */
#pragma omp taskwait
#pragma omp atomic
				threads--;
				delete p;
				dtime = omp_get_wtime() - item->time;
#pragma omp atomic
				totalWait = totalWait + dtime;
				if (maxItemWait < dtime) {
#pragma omp critical
					if (maxItemWait < dtime) {
						maxItemWait = dtime;
					}
				}
				delete item;
#pragma omp atomic
				numItems++;
#pragma omp atomic
				threads++;
			}
		}
	}
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
