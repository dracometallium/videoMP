#include "Item.hpp"
#include <omp.h>

Item::Item()
{
	time = omp_get_wtime();
}

Item::~Item()
{
}
