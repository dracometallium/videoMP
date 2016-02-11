#include "Input.hpp"

 Input::Input(RingStack * rs):Input(rs, 0)
{
}

Input::Input(RingStack * rs, double ignoreTime)
{
	ringStack = rs;
	numItems = 0;
	running = 0;
	ignore = ignoreTime;
}

int Input::run()
{
	Item *item;
	running = 1;
	ignore = omp_get_wtime() + ignore;
	while (running) {
		item = generate();
		if (item != NULL) {
			if (ignore != 0) {
				if (omp_get_wtime() > ignore) {
					ignore = 0;
				} else {
					item->ignore = true;
				}
			}
#pragma omp critical (RingStack)
			{
				item = ringStack->put(item);
				if (ignore == 0) {
					numItems++;
				}
			}
			delete item;
		}
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
	usleep(16000);
	return new Item;
}

Input::~Input()
{
}
