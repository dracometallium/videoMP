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
#pragma omp parallel num_threads(NTHREADS)
	{
		Item *item;
		Item **p;
		int i, t;
		while (running) {
#pragma omp critical (RingStack)
			{
				item = ringStack->get();
			}
			if (item != NULL) {
				p = slicer->slice(item, NPARTS);
#pragma omp parallel num_threads(NPARTS) private(i, t)
				{
					t = omp_get_thread_num();
#pragma omp parallel for num_threads(numPStaks)
					for (i = 0; i < numPStaks; i++) {
						pluginStack[i]->process(p[t]);
					}
					delete p[t];
				}
				delete item;
#pragma omp atomic
				numItems++;
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
