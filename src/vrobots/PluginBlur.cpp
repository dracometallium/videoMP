/*
 * plugin_blur.cpp
 *
 *  Created on: 29/11/2014
 *      Author: willytell
 */

#include "PluginBlur.hpp"

int PluginBlur::process(Frame * frame)
{
	cvSmooth(frame->data[0]->image_hsv, frame->data[0]->image_hsv, CV_BLUR, 3);
	return 0;
}
