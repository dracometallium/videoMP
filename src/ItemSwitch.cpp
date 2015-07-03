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
	maxThreshold = 0;
	maxItemWait = 0;
	freshItems = 0;
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
	int iNumItems, iFreshItems;
	iNumItems = 0;
	iFreshItems = 0;
	running = 1;
#pragma omp parallel num_threads(NTHREADS) reduction(+:iNumItems,iFreshItems)
	{
		Item *item;
		Item **p;
		int i, t;
		double dtime;
		while (running) {
			item = NULL;
			if (ringStack->getSize() > 0) {
#pragma omp critical (RingStack)
				{
					item = ringStack->get();
				}
			}
			if (item != NULL) {
#pragma omp parallel for num_threads(numPStaks) private(i, p)
				for (i = 0; i < numPStaks; i++) {
					p = slicer->slice(item, NPARTS);
#pragma omp parallel num_threads(NPARTS) private(t)
					{
						t = omp_get_thread_num();
						pluginStack[i]->process(p[t]);
						delete p[t];
					}
					delete p;
				}
				dtime = omp_get_wtime() - item->time;
				iNumItems++;
				if (dtime <= maxThreshold) {
					iFreshItems++;
				}
				if (maxItemWait < dtime) {
#pragma omp critical
					if (maxItemWait < dtime) {
						maxItemWait = dtime;
					}
				}
				delete item;
			}
		}
	}
	numItems = iNumItems;
	freshItems = iFreshItems;
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
