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
	if(RingStack_STACK){
		item = getTop();
	} else {
		item = getBottom();
	}
	return item;
}

Item *RingStack::getBottom()
{
	Item *item;
	int nItem;
	item = NULL;
	nItem = (top - numItems + SIZE) % SIZE;
	if (numItems) {
		item = items[nItem];
		items[nItem] = NULL;
		numItems--;
	}
	return item;
}

Item *RingStack::getTop()
{
	Item *item;
	item = NULL;
	if (numItems) {
		item = items[top];
		items[top] = NULL;
		numItems--;
		top = (top - 1 + SIZE) % SIZE;
	}
	return item;
}

Item *RingStack::put(Item * item)
{
	Item *oldItem;
	oldItem = NULL;
	if(item == NULL){
		return NULL;
	}
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
