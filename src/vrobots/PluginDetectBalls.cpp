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
  \file    plugin_detect_balls.cpp
  \brief   C++ Implementation: PluginDetectBalls
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "PluginDetectBalls.hpp"

int PluginDetectBalls::process(Frame * frame)
{
	colorid = frame->data[0]->ball->color_id;
	frame->data[colorid]->result =
	    cvLabel(frame->data[colorid]->segmentated,
		    frame->data[colorid]->labelImg,
		    frame->data[colorid]->blobs);
	cvFilterByArea(frame->data[colorid]->blobs,
		       frame->data[colorid]->ball->area_min,
		       frame->data[colorid]->ball->area_max);

	for (CvBlobs::iterator it = frame->data[colorid]->blobs.begin();
	     it != frame->data[colorid]->blobs.end(); ++it) {
		int width = MAX((*it).second->maxx - (*it).second->minx,
				(*it).second->maxy - (*it).second->miny);
		width = (int)width;	//*1.5;
		//bbox2D(const CvSize imgSize, const int px_length, const CvPoint center, CvRect *box)
		bbox2D(cvGetSize(frame->frame), width, cvPoint((*it).second->centroid.x, (*it).second->centroid.y), &r);	//calculate r box to set image roi later.
		cvRectangle(frame->frame, cvPoint(r.x, r.y),
			    cvPoint(r.x + r.width, r.y + r.height), cvScalar(0,
									     0,
									     255,
									     0),
			    1, 8, 0);

		frame->data[colorid]->ball->center.x =
		    (int)(*it).second->centroid.x;
		frame->data[colorid]->ball->center.y =
		    (int)(*it).second->centroid.y;
		frame->data[colorid]->ball->field_pos = frame->data[0]->homography->transPoint(frame->data[colorid]->ball->center);	//field position

		break;
	}

	return 0;
}

void PluginDetectBalls::bbox2D(const CvSize imgSize, const int px_length,
			  const CvPoint center, CvRect * box)
{
	int l = (int)(px_length / 2);

	box->x = (center.x - l);
	if (box->x < 0)
		box->x = 0;

	box->y = (center.y - l);
	if (box->y < 0)
		box->y = 0;

	if (imgSize.width < (box->x + px_length))
		box->width = (imgSize.width - 1);
	else
		box->width = px_length;

	if (imgSize.height < (box->y + px_length))
		box->height = (imgSize.height - 1);
	else
		box->height = px_length;
}
