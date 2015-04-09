#ifndef __Plugin_HPP
#define __Plugin_HPP

#include <unistd.h>
#include "Item.hpp"

class Plugin {

 public:
	Plugin();
	virtual int process(Item * item);

};
#endif
