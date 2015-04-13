#ifndef __PluginStack_HPP
#define __PluginStack_HPP

#include <vector>
#include "Plugin.hpp"
#include "Item.hpp"

class PluginStack {

 public:
	PluginStack();
	int addPlugin(Plugin * p);
	int process(Item * item);

 private:
	 std::vector < Plugin * >plugins;
	int nplugins;
};
#endif
