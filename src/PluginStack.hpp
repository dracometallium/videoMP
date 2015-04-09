#ifndef __PluginStack_HPP
#define __PluginStack_HPP

#include <vector>
#include <omp.h>
#include "Plugin.hpp"
#include "Slicer.hpp"
#include "Item.hpp"

class PluginStack {

 public:
	PluginStack(int numThreads, Slicer * s);
	int addPlugin(Plugin * p);
	int process(Item * item);

 private:
	int NTHREADS;
	Slicer *slicer;
	 std::vector < Plugin * >plugins;
	int nplugins;
};
#endif
