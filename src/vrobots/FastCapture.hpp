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
  \file    capturefromfile.h
  \brief   C++ Interface: CaptureFromFile
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#ifndef __CaptureFromFile_HPP
#define __CaptureFromFile_HPP

#include <iostream>
#include "../Input.hpp"
#include "Frame.hpp"
#include <unistd.h>
#include <string>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

/*!
  \class   CaptureFromFile
  \brief   Get frames capturing it from a video file.
*/

using namespace cv;
class FastCapture:public Input {
 public:
	FastCapture(RingStack * rs, std::string _filename);
	virtual Item *generate();

 protected:
	 std::string filename;
	VideoCapture capture;
	Mat tframe;
	RingStack * irs;
	int fps;
	int total_frames;
	int frame_height;
	int frame_width;
	int frame_counter;
 private:
	void iplimage_from_cvmat(CvMat input, IplImage * output);
	double lastTime;
	double dTime;
};

#endif
