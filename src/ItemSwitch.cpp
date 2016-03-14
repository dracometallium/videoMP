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
#pragma omp single
	while (running) {
		item = NULL;
		if (ringStack->getSize() > 0 && (threads > 0)) {
#pragma omp critical (RingStack)
			item = ringStack->get();
		}
		if (item != NULL && running)
#pragma omp task firstprivate(item)
		{
			double dtime;
			int i;
			for (i = 0; i < numPStaks; i++)
#pragma omp task firstprivate(i) default(shared)
			{

				Item **p;
				if (running && (omp_get_wtime() - item->time) <
				    maxThreshold) {
					int t;
					int deltaThreads;
#pragma omp atomic
					threads--;
					p = slicer->slice(item, NPARTS);
					deltaThreads = NPARTS - 1;
#pragma omp atomic
					threads = threads - deltaThreads;
					for (t = 0; t < NPARTS; t++)
#pragma omp task firstprivate(t) default(shared)
					{
						double dt;
						dt = omp_get_wtime() -
						    item->time;
						if ((dt < maxThreshold)
						    && running) {
							slicer->resetItem(p[t]);
							pluginStack[i]->process
							    (p[t]);
						}
						slicer->delPart(p[t]);
#pragma omp atomic
						threads++;
					}
#pragma omp taskwait
					delete p;
				}
			}
#pragma omp taskwait
#pragma omp atomic
			threads--;
			dtime = omp_get_wtime() - item->time;
			if (ignore) {
#pragma omp critical (itemIgnore)
				ignore = Item::ignore;
			}
			if ((dtime < maxThreshold) && !ignore && running) {
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
