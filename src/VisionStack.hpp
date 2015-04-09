#ifndef __VisionStack_HPP
#define __VisionStack_HPP

#include <vector>
#include <omp.h>
#include "Plugin.hpp"
#include "Slicer.hpp"
#include "Item.hpp"

class VisionStack {

 public:
	VisionStack(int numThreads, Slicer s);
	int addPlugin(Plugin * p);
	int process(Item * item);

 private:
	int NTHREADS;
	Slicer *slicer;
	 std::vector < Plugin * >plugins;
	int nplugins;
}
#endif
