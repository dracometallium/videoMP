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
  \file    plugin_detect_balls.h
  \brief   C++ Interface: PluginDetectBalls
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#ifndef __PluginDetectBalls_HPP__
#define __PluginDetectBalls_HPP__

#include "../Plugin.hpp"
#include "Frame.hpp"
#include <cv.h>

class PluginDetectBalls: public Plugin {
public:
	PluginDetectBalls();
	virtual ~PluginDetectBalls();

	virtual int process(Frame * frame);

private:
	CvRect r;
	int colorid;
	void bbox2D(const CvSize imgSize, const int px_length, const CvPoint center, CvRect *r);
};

#endif
