#ifndef __Slicer_HPP
#define __Slicer_HPP
#include "Item.hpp"
class Slicer {
 public:
	virtual Item ** slice(Item * item, int numParts);
	 virtual ~ Slicer();
	virtual int delPart(Item * item);
};
#endif
