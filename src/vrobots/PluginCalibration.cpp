/*
 * plugin_calibration.cpp
 *
 *  Created on: 16/01/2014
 *      Author: willytell
 */

#include "PluginCalibration.hpp"

PluginCalibration::PluginCalibration()
{
	i = 0;
}

int PluginCalibration::process(Item * item)
{
	Frame *frame;
	IplImage *t;
	frame = (Frame *) item;
	if (i == 0)
#pragma omp critical
	{
		if (i == 0) {
			intrinsic =
			    (CvMat *) cvLoad("src/vrobots/Intrinsics.xml");
			distortion =
			    (CvMat *) cvLoad("src/vrobots/Distortion.xml");
			mapx =
			    cvCreateImage(cvGetSize(frame->frame),
					  IPL_DEPTH_32F, 1);
			mapy =
			    cvCreateImage(cvGetSize(frame->frame),
					  IPL_DEPTH_32F, 1);
			cvInitUndistortMap(intrinsic, distortion, mapx, mapy);
			i++;
		}
	}
	t = cvCloneImage(frame->frame);
	cvRemap(t, frame->frame, mapx, mapy);	// Undistort image
	cvReleaseImage(&t);
	return 0;
}
