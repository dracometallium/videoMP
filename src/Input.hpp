#ifndef __Input__HPP
#define __Input__HPP

#include <omp.h>
#include <unistd.h>
#include "RingStack.hpp"
#include "Item.hpp"

class Input {

 public:
	Input(RingStack * rs);
	Input(RingStack * rs, double ignore);
	//run can be overridden, but it shouldn't be necessary.
	virtual int run();
	int stop();
	int numItems;
	 virtual ~ Input();

 protected:
	 RingStack * ringStack;

 private:
	volatile int running;
	double ignore;
	//generate should be overridden.
	virtual Item *generate();

};
#endif
