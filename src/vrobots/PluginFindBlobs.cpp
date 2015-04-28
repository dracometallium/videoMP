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
  \file    plugin_find_blobs.cpp
  \brief   C++ Implementation: PluginFindBlobs
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "PluginFindBlobs.hpp"
int PluginFindBlobs::process(Frame * frame) {
	frame->data[cBLUE]->result = cvLabel(frame->data[cBLUE]->segmentated,
			frame->data[cBLUE]->labelImg, frame->data[cBLUE]->blobs);
	cvFilterByArea(frame->data[cBLUE]->blobs,5, 50000);

	return 0;
}

