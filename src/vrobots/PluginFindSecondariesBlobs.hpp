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
  \file    plugin_find_secondaries_blobs.h
  \brief   C++ Interface: PluginFindSecondariesBlobs
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#ifndef __PluginFindSecondariesBlobs_HPP__
#define __PluginFindSecondariesBlobs_HPP__

#include "../Plugin.hpp"
#include "Frame.hpp"
#include "marker.h"
#include "pattern_matching.h"
#include <math.h>

class PluginFindSecondariesBlobs:public Plugin {
 public:
	PluginFindSecondariesBlobs();
	virtual ~ PluginFindSecondariesBlobs();

	virtual int process(Frame * frame);

 private:
	void rotation_matrix(float tita);

 private:
	void bbox2D(const CvSize imgSize, const int px_length,
		    const CvPoint center, CvRect * r);
	CvRect r;
	PatternMatching *matching;
	CvMat *rot_mat;
	CvMat *r_mat;
	CvMat *M;
	CvFont font;
};

#endif
