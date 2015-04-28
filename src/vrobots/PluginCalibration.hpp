/*
 * plugin_calibration.h
 *
 *  Created on: 16/01/2014
 *      Author: willytell
 */

#ifndef __PluginCalibration_HPP_
#define __PluginCalibration_HPP_

#include "../Plugin.hpp"
#include "Frame.hpp"
#include <cv.h>

class PluginCalibration:public Plugin {
 public:
	PluginCalibration();

	virtual int process(Frame * frame);

 private:
	int i;
	IplImage *t;
	IplImage *z;
	CvMat *intrinsic;
	CvMat *distortion;
	IplImage *mapx;
	IplImage *mapy;
};

#endif
