#include "Plugin.hpp"
#include <stdio.h>

Plugin::Plugin()
{
}

int Plugin::process(Item * item)
{
	int i, s;
	s = 77;
	for (i = 0; i < 0x0FFFFFF; i++) {
		s = ((s * 3) + 13) % 101;
	}
	fprintf(stderr, ":%d:\n", s);
	return 0;
}
