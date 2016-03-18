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

#include "FastCapture.hpp"

FastCapture::FastCapture(RingStack * rs, std::string _filename, double ignore,
			 int FPS)
:Input(rs, ignore)
{
	Frame *nframe;
	IplImage *tIpl;
	IplImage *ipl;
	int i;
	int loops, nframes;
	int origFPS, mult;
	loops = 3;
	filename = _filename;
	if (!capture.open(filename)) {
		std::cout << "Failed to open video: " << filename << std::endl;
	}
	fps = capture.get(CV_CAP_PROP_FPS);	// by default it's initialized to video's FPS.
	if (fps == 0) {
		std::cout <<
		    "Failed to read frame per second (FPS) from video file."
		    << "By default it is set to 30 fps. " << std::endl;
		fps = 30;
	}
	origFPS = fps;
	if (0 < FPS) {
		fps = FPS;
		forceFPS = true;
	} else {
		forceFPS = false;
	}
	nframes = origFPS * (11 + ignore);
	mult = (fps + origFPS - 1) / origFPS;
	irs = new RingStack(nframes * loops * mult);
	tIpl = new IplImage[nframes];
	dTime = (1.0 / fps);
	total_frames = (int)capture.get(CV_CAP_PROP_FRAME_COUNT);
	frame_height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	frame_width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
	lastTime = 0;
	for (i = 0; i < nframes; i++) {
		capture >> tframe;
		tIpl[i] = tframe;
	}
	for (i = 0; (i < nframes * loops); i++) {
		ipl = cvCloneImage(&(tIpl[i % nframes]));
		nframe = new Frame(ipl);
		irs->put(nframe);
	}
	delete tIpl;
}

Item *FastCapture::generate()
{
	Item *nframe;
	bool emptyRS;
	if (lastTime == 0) {
		lastTime = omp_get_wtime() - dTime;
	}
	lastTime = lastTime + dTime;
	if (!forceFPS) {
#pragma omp critical (RingStack)
		emptyRS = (ringStack->getSize() == 0);
	} else {
		emptyRS = false;
	}
	while (lastTime > omp_get_wtime() && !emptyRS) {
		usleep(250000 * dTime);
		if (!forceFPS) {
#pragma omp critical (RingStack)
			emptyRS = (ringStack->getSize() == 0);
		}
	}

	nframe = irs->get();

	if (nframe != NULL) {
		lastTime = (emptyRS) ? omp_get_wtime() : lastTime;
		nframe->time = lastTime;
	}

	return nframe;
}

void FastCapture::iplimage_from_cvmat(CvMat input, IplImage * output)
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
