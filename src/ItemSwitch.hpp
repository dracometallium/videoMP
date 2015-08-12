#ifndef __ItemSwitch_HPP
#define __ItemSwitch_HPP

#include <omp.h>
#include <vector>
#include <cstdlib>
#include "PluginStack.hpp"
#include "RingStack.hpp"
#include "Item.hpp"
#include "Slicer.hpp"

class ItemSwitch {
 public:
	ItemSwitch(int numThreads, int nparts, Slicer * s, RingStack * rs);
	int addPluginStack(PluginStack * ps);
	int run();
	int stop();

	/* Debug methods */
	int numItems;
	double maxItemWait;
	int setThreshold(double t);

 private:
	int NTHREADS;
	int NPARTS;
	RingStack *ringStack;
	Slicer *slicer;
	 std::vector < PluginStack * >pluginStack;
	int numPStaks;
	volatile int running;

	/* Debug info */
	double maxThreshold;
};
#endif
