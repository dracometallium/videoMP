#include "Item.hpp"
#include <omp.h>

bool Item::ignore = true;

Item::Item()
{
	time = omp_get_wtime();
}

Item::~Item()
{
}
