#include "Plugin.hpp"

Plugin::Plugin()
{
}

int Plugin::process(Item * item)
{
	usleep(10000);
	return 0;
}
