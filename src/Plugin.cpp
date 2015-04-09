#include "Plugin.hpp"

Plugin::Plugin()
{
}

int Plugin::process(Item * item)
{
	usleep(10);
	return 0;
}
