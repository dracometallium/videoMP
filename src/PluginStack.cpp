#include "PluginStack.hpp"
PluginStack::PluginStack()
{
	nplugins = 0;
}

int PluginStack::addPlugin(Plugin * p)
{
	plugins.push_back(p);
	nplugins++;
	return 0;
}

int PluginStack::process(Item * item)
{
	int i;
	for (i = 0; i < nplugins; i++) {
		plugins[i]->process(item);
	}
	return 0;
}
