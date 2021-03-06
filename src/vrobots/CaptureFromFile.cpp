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

CaptureFromFile::CaptureFromFile(RingStack * rs, std::string _filename, double
				 ignore)
:Input(rs, ignore)
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
	lastTime = 0;
}

Item *CaptureFromFile::generate()
{
	Frame *nframe;
	IplImage tIpl;
	IplImage *ipl;
	double tTime;
	// TODO: fps should be changeable to video reproduction speed
	tTime = omp_get_wtime();
	while (tTime < (lastTime + 1.0 / fps)) {
		usleep(10000 / fps);
		tTime = omp_get_wtime();
	}
	lastTime = tTime;

	//cvCloneImage turns a Mat to a IplImage
	capture >> tframe;	// get a new frame from camera.

	//ipl = cvCreateImage(cvSize(frame_height, frame_width), 8, 3);
//      iplimage_from_cvmat(tframe, ipl);
	tIpl = tframe;
	ipl = cvCloneImage(&tIpl);

	nframe = new Frame(ipl);

	return nframe;
}

void CaptureFromFile::iplimage_from_cvmat(CvMat input, IplImage * output)
{
	int x, y;
	for (x = 0; x < output->width; x++) {
		for (y = 0; y < output->height; y++) {
			// note: CvMat is indexed (row, column) but IplImage is indexed (x,y)
			// so the indexes must be interchanged!
			cvSetReal2D(output, x, y,
				    CV_MAT_ELEM(input, uchar, y, x));
		}
	}
}
