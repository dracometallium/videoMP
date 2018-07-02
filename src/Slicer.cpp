#include "Slicer.hpp"
Item **Slicer::slice(Item * item, int numParts)
{
	int i;
	Item **parts;
	parts = new Item *[numParts];
	for (i = 0; i < numParts; i++) {
		parts[i] = new Item;
		parts[i]->num = i;
	}
	return parts;
}

int Slicer::resetItem(Item * item)
{
	return 0;
}

Slicer::~Slicer()
{
}

int Slicer::delPart(Item * item)
{
	delete item;
	return 0;
}
