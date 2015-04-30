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
  \file    plugincolorconversions.cpp
  \brief   C++ Implementation: PluginColorConversions
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "PluginColorConversions.hpp"

int PluginColorConversions::process(Item * item)
{
	Frame *frame;
	frame = (Frame *) item;
	cvCvtColor(frame->frame, (*frame->data)[0]->image_hsv, CV_BGR2GRAY);	// proyecto Laser

	return 0;
}
