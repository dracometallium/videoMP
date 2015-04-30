//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    plugin_colorsegmentation.h
  \brief   C++ Interface: PluginColorSegmentation
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#ifndef __PluginColorSegmentation_HPP__
#define __PluginColorSegmentation_HPP__

#include "../Plugin.hpp"
#include "Frame.hpp"
#include "segmentation.h"
#include <stdio.h>

class PluginColorSegmentation: public Plugin {
public:
	PluginColorSegmentation(std::vector<sColor*>&color);

	virtual int process(Item * item);

private:
	int getPixelColorType(int H, int S, int V);
	int getPixelColorTypeHSV(int H, int S, int V, std::vector<sColor*>&color);
	Segmentation segmentation;
	std::vector<sColor*> color;
};

#endif
