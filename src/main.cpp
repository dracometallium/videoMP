#include <omp.h>
#include <iostream>
#include <unistd.h>

#include "RingStack.hpp"
#include "Item.hpp"
#include "PluginStack.hpp"
#include "ItemSwitch.hpp"
#include "Input.hpp"
#include "Plugin.hpp"
#include "Slicer.hpp"
int main()
{
	int i;
	Input *input;
	ItemSwitch *is;
	PluginStack *ps1, *ps2, *ps3, *ps4;
	RingStack *rs;
	Slicer *sl;
	Plugin plugins[5];

	rs = new RingStack(60);
	is = new ItemSwitch(2, rs);
	input = new Input(rs);
	sl = new Slicer();
	ps1 = new PluginStack(2, sl);
	ps2 = new PluginStack(2, sl);
	ps3 = new PluginStack(2, sl);
	ps4 = new PluginStack(2, sl);
	for (i = 0; i < 5; i++) {
		ps1->addPlugin(&plugins[i]);
		ps2->addPlugin(&plugins[i]);
		ps3->addPlugin(&plugins[i]);
		ps4->addPlugin(&plugins[i]);
	}
	is->addPluginStack(ps1);
	is->addPluginStack(ps2);
	is->addPluginStack(ps3);
	is->addPluginStack(ps4);

#pragma omp parallel sections num_threads(3)
	{
#pragma omp section
		{
			//run Input
			input->run();
		}
#pragma omp section
		{
			//run ItemSwitch
			is->run();
		}
#pragma omp section
		{
			//run UI
			sleep(20);
			input->stop();
			is->stop();
		}
	}

	std::cout << "Input items:\t " << input->numItems << std::endl;
	std::cout << "Process items:\t " << is->numItems << std::endl;
	std::cout << "Loss:\t\t " << 100 - ((is->numItems*100.0)/input->numItems) << "%" << std::endl;
	delete input;
	delete is;
	delete ps1;
	delete ps2;
	delete rs;
	delete sl;

	return 0;

}
