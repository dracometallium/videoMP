#include "RingStack.hpp"

RingStack::RingStack(int MaxItems)
{
	int i;
	SIZE = MaxItems;
	numItems = 0;
	top = 0;
	items = new Item *[MaxItems];
	for (i = 0; i < MaxItems; i++) {
		items[i] = NULL;
	}
}

RingStack::~RingStack()
{
	int i;
	for (i = 0; i < SIZE; i++) {
		delete items[i];
	}
	delete[]items;
}

Item *RingStack::get()
{
	Item *item;
	item = NULL;
	if (numItems) {
		item = items[top];
		numItems--;
		top = (top - 1 + SIZE) % SIZE;
	}
	return item;
}

Item *RingStack::put(Item * item)
{
	Item *oldItem;
	oldItem = NULL;
	top = (top + 1) % SIZE;
	if (numItems == SIZE) {
		oldItem = items[top];
	} else {
		numItems++;
	}
	items[top] = item;
	return oldItem;
}

int RingStack::getSize()
{
	return numItems;
}

int RingStack::getMax()
{
	return SIZE;
}
