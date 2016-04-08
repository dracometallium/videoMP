#ifndef __RingStack_HPP
#define __RingStack_HPP

#include "Item.hpp"
#include <cstdlib>

#define RingStack_STACK false

class RingStack {

 public:
	RingStack(int MaxItems);
	~RingStack();

	//Returns NULL if the stack is empty.
	Item *get();
	Item *getTop();
	Item *getBottom();

	//If the stack was full, returns the discarded item, otherwise NULL.
	Item *put(Item * item);

	int getSize();

	int getMax();

 private:
	int SIZE;
	int numItems;
	int top;
	Item **items;

};
#endif
