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
  \file    plugin_find_secondaries_blobs.cpp
  \brief   C++ Implementation: PluginFindSecondariesBlobs
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "PluginFindSecondariesBlobs.hpp"
#include <stdio.h>

#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

PluginFindSecondariesBlobs::PluginFindSecondariesBlobs()
{
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, 0.7, 0.7, 0,
		   2);

	M = cvCreateMat(2, 1, CV_32FC1);
	M->data.fl[0 * M->cols + 0] = (float)15.0;	// M_x
	M->data.fl[1 * M->cols + 0] = (float)0.0;	// M_y

	draw = false;

}

int PluginFindSecondariesBlobs::process(Item * item)
{
	CvMat *r_mat;
	CvMat *rot_mat;
	Frame *frame;
	CvRect r;
	PatternMatching *matching;
	frame = (Frame *) item;
	rot_mat = cvCreateMat(2, 2, CV_32FC1);
	r_mat = cvCreateMat(2, 1, CV_32FC1);
	matching = new PatternMatching();
	int colorId = (*frame->data)[0]->blue_team->team_colorid;
	for (CvBlobs::iterator it = (*frame->data)[colorId]->blobs->begin();
	     it != (*frame->data)[colorId]->blobs->end(); ++it) {
		int width = MAX((*it).second->maxx - (*it).second->minx,
				(*it).second->maxy - (*it).second->miny);
		width = (int)width *3;
		bbox2D(cvGetSize(frame->frame), width, cvPoint((*it).second->centroid.x, (*it).second->centroid.y), &r);	//calculate r box to set image roi later.
		if(draw){
			cvRectangle(frame->frame, cvPoint(r.x, r.y),
				    cvPoint(r.x + r.width, r.y + r.height),
				    cvScalar(255, 0, 0, 0), 1, 8, 0);
		}

		std::vector < Marker * >markers;
		Marker *mcenter = new Marker();	//main team marker

		mcenter->center.x = (int)(*it).second->centroid.x;
		mcenter->center.y = (int)(*it).second->centroid.y;
		mcenter->area = (*it).second->area;

		for (unsigned int i = 0;
		     i < (*frame->data)[0]->blue_team->usesColor.size(); ++i) {
			int cid = (*frame->data)[0]->blue_team->usesColor[i];

			CvRect imgROI =
				cvGetImageROI ((*frame->data)[cid]->segmentated);

			cvSetImageROI((*frame->data)[cid]->segmentated,
				      cvRect(r.x, r.y, r.width, r.height));
			(*frame->data)[cid]->result =
			    cvLabel((*frame->data)[cid]->segmentated,
				    (*frame->data)[cid]->labelImg,
				    *((*frame->data)[cid]->blobs));
			cvFilterByArea(*((*frame->data)[cid]->blobs), 10, 50000);

			for (CvBlobs::iterator
			     it2 = (*frame->data)[cid]->blobs->begin();
			     it2 != (*frame->data)[cid]->blobs->end(); ++it2) {
				Marker *m = new Marker();
				m->center.x =
				    (int)(*it2).second->centroid.x + r.x;
				m->center.y =
				    (int)(*it2).second->centroid.y + r.y;
				m->colorid = cid;
				m->area = (*it2).second->area;
				markers.push_back(m);

				if(draw){
					cvCircle(frame->frame,
						 cvPoint((*it2).second->centroid.x +
							 r.x,
							 (*it2).second->centroid.y +
							 r.y), 3, cvScalar(0, 255, 0,
									   0), 1, 8, 0);
				}
			}
			cvSetImageROI((*frame->data)[cid]->segmentated, imgROI);
		}

		matching->calcMarkersAngle(mcenter, markers);
		matching->sortMarkersByAngle(markers);

		//TODO: it isn't efficient at all!
		for (unsigned int j = 0;
		     j < (*frame->data)[0]->blue_team->patches.size(); ++j) {
			bool f =
			    matching->
			    matchingMarkersByPosition((*frame->
						       data)
						      [0]->blue_team->patches
						      [j]->markers,
						      markers);
			if (f) {
				// Patch detected !!
				(*frame->data)[0]->blue_team->patches[j]->center = mcenter->center;	//pixel position
				(*frame->data)[0]->blue_team->patches[j]->teamMarker.area = mcenter->area;	// team marker area
				for (unsigned int k = 0;
				     k <
				     (*frame->data)[0]->blue_team->
				     patches[j]->markers.size(); k++) {
					(*frame->data)[0]->blue_team->
					    patches[j]->markers[k]->center =
					    markers[k]->center;
					(*frame->data)[0]->blue_team->
					    patches[j]->markers[k]->area =
					    markers[k]->area;
				}

				(*frame->data)[0]->blue_team->patches[j]->field_pos = (*frame->data)[0]->homography->transPoint(mcenter->center);	//field position

				(*frame->data)[0]->blue_team->
				    patches[j]->orientation = markers[1]->angle;
				rotation_matrix(rot_mat, (*frame->data)[0]->
						blue_team->patches[j]->
						orientation);
				(*frame->data)[0]->blue_team->patches[j]->detected = true;	// detected object

				//rotation
				r_mat->data.fl[0 * r_mat->cols + 0] =
				    M->data.fl[0 * M->cols +
						  0] * rot_mat->data.fl[0 *
									   rot_mat->cols
									   +
									   0] +
				    M->data.fl[1 * M->cols +
						  0] * rot_mat->data.fl[0 *
									   rot_mat->cols
									   + 1];
				r_mat->data.fl[1 * r_mat->cols + 0] =
				    M->data.fl[0 * M->cols +
						  0] * rot_mat->data.fl[1 *
									   rot_mat->cols
									   +
									   0] +
				    M->data.fl[1 * M->cols +
						  0] * rot_mat->data.fl[1 *
									   rot_mat->cols
									   + 1];
				int x =
				    (int)
				    cvRound(r_mat->data.fl[0 * r_mat->cols]);
				int y =
				    (int)
				    cvRound(r_mat->data.fl[1 * r_mat->cols]);

				//traslation
				x = x +
				    (*frame->data)[0]->blue_team->
				    patches[j]->center.x;
				y = y +
				    (*frame->data)[0]->blue_team->
				    patches[j]->center.y;
				if(draw){
					cvCircle(frame->frame, cvPoint(x, y), 2,
						 cvScalar(255, 255, 0), CV_FILLED,
						 CV_AA, 0);
				}

				std::stringstream Num;
				std::string str;
				Num << (*frame->data)[0]->blue_team->
				    patches[j]->id;

				str = Num.str();
				char *char_type = (char *)str.c_str();
				if(draw){
					cvPutText(frame->frame, char_type, cvPoint(x, y),
						  &font, cvScalar(255, 0, 0));
				}

				for (unsigned int k = 0; k < markers.size();
				     ++k) {
					if(draw){
						cvLine(frame->frame,
						       cvPoint(mcenter->center.x,
							       mcenter->center.y),
						       cvPoint(markers[k]->center.x,
							       markers[k]->center.y),
						       cvScalar(255, 0, 0), 1);
					}
					(*frame->data)[0]->blue_team->patches[j]->markers[k]->dist = matching->euclideanDistance2D(mcenter->center, markers[k]->center);	// distance between central main marker and a marker.
				}
			}
		}

		//cleaning
		for (unsigned int j = 0; j < markers.size(); ++j) {
			markers[j]->~Marker();
			markers.erase(markers.begin());
		}
	//	mcenter->~Marker();
		delete mcenter;
	}
	cvReleaseMat(&rot_mat);
	cvReleaseMat(&r_mat);
	delete matching;
	return 0;
}

void PluginFindSecondariesBlobs::rotation_matrix(CvMat * rot_mat, float tita)
{
	//cvZero(rot_mat);
	rot_mat->data.fl[0 * rot_mat->cols + 0] = (float)cos(tita);
	rot_mat->data.fl[0 * rot_mat->cols + 1] = (float)-sin(tita);
	rot_mat->data.fl[1 * rot_mat->cols + 0] = (float)sin(tita);
	rot_mat->data.fl[1 * rot_mat->cols + 1] = (float)cos(tita);
}

void PluginFindSecondariesBlobs::bbox2D(const CvSize imgSize,
					const int px_length,
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
