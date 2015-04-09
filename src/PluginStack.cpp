#include "PluginStack.hpp"
PluginStack::PluginStack(int numThreads, Slicer * s)
{
	NTHREADS = numThreads;
	slicer = s;
	nplugins = 0;
	if (!omp_get_nested()) {
		omp_set_nested(1);
	}
}

int PluginStack::addPlugin(Plugin * p)
{
	plugins.push_back(p);
	nplugins++;
	return 0;
}

int PluginStack::process(Item * item)
{
	int t, i;
	Item **parts;
	parts = slicer->slice(item, NTHREADS);
#pragma omp parallel num_threads(NTHREADS) private(i, t)
	{
		t = omp_get_thread_num();
		for (i = 0; i < nplugins; i++) {
			plugins[i]->process(parts[t]);
		}
		delete parts[t];
	}
	delete[]parts;
	return 0;
}
