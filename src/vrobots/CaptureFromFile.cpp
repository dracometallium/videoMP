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
  \file    capturefromfile.cpp
  \brief   C++ Implementation: CaptureFromFile
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "CaptureFromFile.hpp"

CaptureFromFile::CaptureFromFile(RingStack * rs, std::string _filename)
 : Input(rs)
{
	filename = _filename;
	if (!capture.open(filename)) {
		std::cout << "Failed to open video: " << filename << std::endl;
	}
	fps = capture.get(CV_CAP_PROP_FPS);	// by default it's initialized to video's FPS.
	if (fps == 0) {
		std::cout <<
		    "Failed to read frame per second (FPS) from video file. By default it is set to 30 fps. "
		    << std::endl;
		fps = 30;
	}
	total_frames = (int)capture.get(CV_CAP_PROP_FRAME_COUNT);
	frame_height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	frame_width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
}

Item *CaptureFromFile::generate()
{
	// TODO: fps should be changeable to video reproduction speed
	Frame *nframe;
	usleep(1000000 / fps);
	//cvCloneImage turns a Mat to a IplImage
	capture >> tframe;	// get a new frame from camera.
	nframe = new Frame(cvCloneImage(((IplImage *) & tframe)));

	return nframe;
}
