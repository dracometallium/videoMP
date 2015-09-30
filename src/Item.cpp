#include "Item.hpp"
#include <omp.h>

Item::Item()
{
	time = omp_get_wtime();
}

int Item::delPart()
{
	delete this;
	return 0;
}

Item::~Item()
{
}
