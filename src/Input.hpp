#ifndef __Input__HPP
#define __Input__HPP

#include <omp.h>
#include <unistd.h>
#include "RingStack.hpp"
#include "Item.hpp"

class Input {

 public:
	Input(RingStack * rs);
	//run can be overridden, but it shouldn't be necessary.
	virtual int run();
	int stop();
	int numItems;
	 virtual ~ Input();

 private:
	volatile int running;
	RingStack *ringStack;
	//generate should be overridden.
	virtual Item *generate();

};
#endif
