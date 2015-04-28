/*
 * plugin_blur.h
 *
 *  Created on: 29/11/2014
 *      Author: willytell
 */

#ifndef __PluginBlur_HPP_
#define __PluginBlur_HPP_

#include "../Plugin.hpp"
#include "Frame.hpp"

class PluginBlur:public Plugin {

 public:

	virtual int process(Frame * frame);
};

#endif
