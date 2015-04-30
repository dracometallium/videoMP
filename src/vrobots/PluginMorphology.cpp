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
  \file    plugin_morphology.cpp
  \brief   C++ Implementation: PluginMorphology
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "PluginMorphology.hpp"

int PluginMorphology::process(Item * item)
{
	Frame *frame;
	frame = (Frame *) item;
	cvMorphologyEx((*frame->data)[cBLUE]->segmentated,
		       (*frame->data)[cBLUE]->segmentated, NULL,
		       (*frame->data)[cBLUE]->morphKernel, CV_MOP_OPEN, 1);
	cvMorphologyEx((*frame->data)[cYELLOW]->segmentated,
		       (*frame->data)[cYELLOW]->segmentated, NULL,
		       (*frame->data)[cYELLOW]->morphKernel, CV_MOP_OPEN, 1);
	return 0;
}
