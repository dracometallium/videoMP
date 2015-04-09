#include "Input.hpp"

Input::Input(RingStack * rs)
{
	ringStack = rs;
	running = 0;
}

int Input::run()
{
	Item *item;
	running = 1;
	while (running) {
		item = generate();
#pragma omp critical (RingStack)
		{
			item = ringStack->put(item);
			numItems++;
		}
		delete item;
	}
	return 0;
}

int Input::stop()
{
	running = 0;
	return 0;
}

Item *Input::generate()
{
	usleep(16);
	return new Item;
}

Input::~Input()
{
}
