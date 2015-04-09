#ifndef __ItemSwitch_HPP
#define __ItemSwitch_HPP

#include <omp.h>
#include <vector>
#include <cstdlib>
#include "PluginStack.hpp"
#include "RingStack.hpp"
#include "Item.hpp"

class ItemSwitch {
 public:
	ItemSwitch(int numThreads, RingStack * rs);
	int addPluginStack(PluginStack * ps);
	int run();
	int stop();
	int numItems;

 private:
	int NTHREADS;
	RingStack *ringStack;
	 std::vector < PluginStack * >pluginStack;
	int numPStaks;
	volatile int running;
};
#endif
