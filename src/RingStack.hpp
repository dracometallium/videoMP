#ifndef __RingStack_HPP
#define __RingStack_HPP

#include "Item.hpp"
#include <cstdlib>

class RingStack {

 public:
	RingStack(int MaxItems);
	~RingStack();

	//Returns NULL if the stack is empty.
	Item *get();

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
