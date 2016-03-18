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

#include "vrobots/FastCapture.hpp"
#include "vrobots/FrameSlicer.hpp"

#include "vrobots/PluginBlur.hpp"
#include "vrobots/PluginCalibration.hpp"
#include "vrobots/PluginColorConversions.hpp"
#include "vrobots/PluginColorSegmentation.hpp"
#include "vrobots/PluginDetectBalls.hpp"
#include "vrobots/PluginFindBlobs.hpp"
#include "vrobots/PluginFindSecondariesBlobs.hpp"
#include "vrobots/PluginMorphology.hpp"
#include "vrobots/PluginNetworking.hpp"

int main(int carg, char **varg)
{
	Input *input;
	ItemSwitch *is;
	PluginStack *ps1, *ps2;
	RingStack *rs;
	Slicer *sl;
	int NTHREADS, NPARTS;
	double maxThreshold;
	char *file = "../robots.avi";
	int FPS;
	maxThreshold = 10.0;
	FPS = 0;

	if (carg > 2) {
		NTHREADS = atoi(varg[1]);
		NPARTS = atoi(varg[2]);
		if(carg > 3){
			file = varg[3];
		}
		if(carg > 4){
			FPS = atoi(varg[4]);
		}
	} else {
		NTHREADS = 2;
		NPARTS = 2;
	}

	std::vector < sColor * >color;
	sColor *c;
	//Colors!!
	//cBlack
	c = new sColor();
	c->hsv_min.hue = 0;
	c->hsv_max.hue = 255;
	//c->hsv_max.hue = 179;
	c->hsv_min.saturation = 0;
	c->hsv_max.saturation = 170;
	//c->hsv_max.saturation = 255;
	c->hsv_min.value = 0;
	c->hsv_max.value = 50;
	//c->hsv_max.value = 75;
	c->background = true;
	color.push_back(c);

	//cWHITE
	c = new sColor();
	c->hsv_min.hue = 0;
	c->hsv_max.hue = 179;
	c->hsv_min.saturation = 0;
	c->hsv_max.saturation = 27;
	c->hsv_min.value = 190;
	c->hsv_max.value = 255;
	c->background = true;
	color.push_back(c);

	//cGREY
	c = new sColor();
	c->hsv_min.hue = 0;
	c->hsv_max.hue = 179;
	c->hsv_min.saturation = 0;
	c->hsv_max.saturation = 53;
	c->hsv_min.value = 0;
	c->hsv_max.value = 185;
	color.push_back(c);

	//cRED
	c = new sColor();
	c->hsv_min.hue = 0;
	c->hsv_max.hue = 0;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	color.push_back(c);

	//cBROWN
	c = new sColor();
	c->hsv_min.hue = 15;
	c->hsv_max.hue = 18;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	color.push_back(c);

	//cORANGE
	c = new sColor();
	c->hsv_min.hue = 19;
	c->hsv_max.hue = 25;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	color.push_back(c);

	//cYELLOW
	c = new sColor();
	//c->habilitar = true;
	c->hsv_min.hue = 26;
	c->hsv_max.hue = 34;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	color.push_back(c);

	//cGREEN
	c = new sColor();
	c->habilitar = true;
	c->hsv_min.hue = 79 - 5;
	c->hsv_max.hue = 79 + 5;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	c->rgb.red = 0;
	c->rgb.blue = 0;
	c->rgb.green = 255;
	color.push_back(c);

	//cAQUA
	c = new sColor();
	c->hsv_min.hue = 2;
	c->hsv_max.hue = 10;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	color.push_back(c);

	//cBLUE
	c = new sColor();
	//c->habilitar = true;
	c->hsv_min.hue = 102 - 10;
	c->hsv_max.hue = 102 + 10;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	c->rgb.red = 0;
	c->rgb.blue = 255;
	c->rgb.green = 0;
	color.push_back(c);

	//cPURPLE
	c = new sColor();
	c->hsv_min.hue = 110;
	c->hsv_max.hue = 124;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	color.push_back(c);

	//cPINK
	c = new sColor();
	c->hsv_min.hue = 145;
	c->hsv_max.hue = 165;
	c->hsv_min.saturation = 28;
	c->hsv_max.saturation = 255;
	c->hsv_min.value = 76;
	c->hsv_max.value = 255;
	color.push_back(c);

	rs = new RingStack(60);
	input = new FastCapture(rs, file, 3.0, FPS);
	sl = new FrameSlicer();
	is = new ItemSwitch(NTHREADS, NPARTS, sl, rs);
	ps1 = new PluginStack();
	ps2 = new PluginStack();

//      ps1->addPlugin(new PluginCalibration());
	ps1->addPlugin(new PluginColorConversions());
	ps1->addPlugin(new PluginColorSegmentation(color));
	ps1->addPlugin(new PluginMorphology());
	ps1->addPlugin(new PluginDetectBalls());
	ps1->addPlugin(new PluginNetworking(2));

//      ps2->addPlugin(new PluginCalibration());
	ps2->addPlugin(new PluginColorConversions());
	ps2->addPlugin(new PluginColorSegmentation(color));
	ps2->addPlugin(new PluginMorphology());
	ps2->addPlugin(new PluginFindBlobs());
	ps2->addPlugin(new PluginFindSecondariesBlobs());
	ps2->addPlugin(new PluginNetworking(1));

	is->addPluginStack(ps1);
	is->addPluginStack(ps2);
	is->setThreshold(maxThreshold);

	sleep(2);
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
			sleep(13);
			input->stop();
			is->stop();
		}
	}

	std::cout << NTHREADS << " " << NPARTS << " "
	    << input->numItems << " "
	    << is->numItems << " "
	    << is->maxItemWait << " "
	    << is->totalWait << std::endl;
	delete input;
	delete is;
	delete ps1;
	delete ps2;
	delete rs;
	delete sl;

	return 0;

}
