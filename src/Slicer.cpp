#include "Slicer.hpp"
Item **Slicer::slice(Item * item, int numParts)
{
	int i;
	Item **parts;
	parts = new Item *[numParts];
	for (i = 0; i < numParts; i++) {
		parts[i] = new Item;
	}
	return parts;
}

Slicer::~Slicer()
{
}

int Slicer::delPart(Item * item)
{
	delete item;
	return 0;
}
