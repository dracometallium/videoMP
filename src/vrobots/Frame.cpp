#include "Frame.hpp"
#include <omp.h>

#include "ball.h"
#include "datastruct.h"
#include "homography.h"
#include "marker.h"
#include "pattern.h"
#include "team.h"

std::vector < std::vector < sData * >*>Frame::dpool;

Frame::Frame(IplImage * iframe)
 : Item()
{
	frame = iframe;
	data = NULL;
}

Frame::~Frame()
{
	cvReleaseImage(&frame);
	if (data != NULL) {
#pragma omp critical (dpool)
		{
			dpool.push_back(data);
		}
		data = NULL;
	}
}

int Frame::initData()
{
	data = NULL;
#pragma omp critical (dpool)
	{
		if (!dpool.empty()) {
			data = dpool.back();
			dpool.pop_back();
		}
	}
	if (data == NULL) {
		data = newData(frame);
	}
	resetData();
	return 0;
}

void Frame::resetData()
{
	for (int i = 0; i < NUM_COLOR_TYPES; ++i) {
		cvSet((*data)[i]->segmentated, cvScalar(0));	// Clear image to black.
		(*data)[i]->result = 0;
		cvReleaseBlobs((*data)[i]->blobs);
		cvReleaseTracks((*data)[i]->tracks);
	}

	for (unsigned int j = 0; j < (*data)[0]->blue_team->patches.size(); ++j)
		(*data)[0]->blue_team->patches[j]->detected = false;

}

std::vector < sData * >*Frame::newData(IplImage * img)
{
	Team *blue_team;
	Team *yellow_team;
	Ball *ball;
	Homography *h;
	std::vector < sData * >*ndata;
	ndata = new std::vector < sData * >;

	CvPoint p1, p2, p3, p4;
	CvPoint u1, u2, u3, u4;

	blue_team = new Team();
	yellow_team = blue_team;
	ball = new Ball();
	h = new Homography;

	//ball
	ball->enabled = true;
	ball->color_id = cAQUA;
	ball->area_min = 15;
	ball->area_max = 5000;

	//team
	blue_team->enabled = true;
	blue_team->team_id = 1;
	blue_team->team_colorid = cBLUE;
	blue_team->usesColor.push_back(cPURPLE);	//colors used by a team
	blue_team->usesColor.push_back(cGREEN);	//colors used by a team
	blue_team->usesColor.push_back(cPINK);	//colors used by a team

	//patch of a team
	blue_team->patches.push_back(new Pattern());
	blue_team->patches[0]->enabled = true;
	blue_team->patches[0]->num_markers = 4;
	blue_team->patches[0]->id = 1;
	blue_team->patches[0]->usesColor.push_back(cPURPLE);	//colors used by a particular patch
	blue_team->patches[0]->usesColor.push_back(cGREEN);	//colors used by a particular patch

	//markers of a patch
	blue_team->patches[0]->markers.push_back(new Marker());
	blue_team->patches[0]->markers[0]->colorid = cPURPLE;
	blue_team->patches[0]->markers.push_back(new Marker());
	blue_team->patches[0]->markers[1]->colorid = cGREEN;
	blue_team->patches[0]->markers.push_back(new Marker());
	blue_team->patches[0]->markers[2]->colorid = cGREEN;
	blue_team->patches[0]->markers.push_back(new Marker());
	blue_team->patches[0]->markers[3]->colorid = cGREEN;

	//patch of a team
	blue_team->patches.push_back(new Pattern());
	blue_team->patches[1]->enabled = true;
	blue_team->patches[1]->num_markers = 4;
	blue_team->patches[1]->id = 2;
	blue_team->patches[1]->usesColor.push_back(cPINK);	//colors used by a particular patch
	blue_team->patches[1]->usesColor.push_back(cGREEN);	//colors used by a particular patch

	//markers of a patch
	blue_team->patches[1]->markers.push_back(new Marker());
	blue_team->patches[1]->markers[0]->colorid = cPINK;
	blue_team->patches[1]->markers.push_back(new Marker());
	blue_team->patches[1]->markers[1]->colorid = cGREEN;
	blue_team->patches[1]->markers.push_back(new Marker());
	blue_team->patches[1]->markers[2]->colorid = cGREEN;
	blue_team->patches[1]->markers.push_back(new Marker());
	blue_team->patches[1]->markers[3]->colorid = cGREEN;

	//patch of a team
	blue_team->patches.push_back(new Pattern());
	blue_team->patches[2]->enabled = true;
	blue_team->patches[2]->num_markers = 4;
	blue_team->patches[2]->id = 3;
	blue_team->patches[2]->usesColor.push_back(cGREEN);	//colors used by a particular patch
	blue_team->patches[2]->usesColor.push_back(cPURPLE);	//colors used by a particular patch

	//markers of a patch
	blue_team->patches[2]->markers.push_back(new Marker());
	blue_team->patches[2]->markers[0]->colorid = cPURPLE;
	blue_team->patches[2]->markers.push_back(new Marker());
	blue_team->patches[2]->markers[1]->colorid = cGREEN;
	blue_team->patches[2]->markers.push_back(new Marker());
	blue_team->patches[2]->markers[2]->colorid = cPURPLE;
	blue_team->patches[2]->markers.push_back(new Marker());
	blue_team->patches[2]->markers[3]->colorid = cGREEN;

	//patch of a team
	blue_team->patches.push_back(new Pattern());
	blue_team->patches[3]->enabled = true;
	blue_team->patches[3]->num_markers = 4;
	blue_team->patches[3]->id = 4;
	blue_team->patches[3]->usesColor.push_back(cGREEN);	//colors used by a particular patch
	blue_team->patches[3]->usesColor.push_back(cPINK);	//colors used by a particular patch

	//markers of a patch
	blue_team->patches[3]->markers.push_back(new Marker());
	blue_team->patches[3]->markers[0]->colorid = cGREEN;
	blue_team->patches[3]->markers.push_back(new Marker());
	blue_team->patches[3]->markers[1]->colorid = cGREEN;
	blue_team->patches[3]->markers.push_back(new Marker());
	blue_team->patches[3]->markers[2]->colorid = cPINK;
	blue_team->patches[3]->markers.push_back(new Marker());
	blue_team->patches[3]->markers[3]->colorid = cPINK;

	//patch of a team
	blue_team->patches.push_back(new Pattern());
	blue_team->patches[4]->enabled = true;
	blue_team->patches[4]->num_markers = 4;
	blue_team->patches[4]->id = 5;
	blue_team->patches[4]->usesColor.push_back(cGREEN);	//colors used by a particular patch
	blue_team->patches[4]->usesColor.push_back(cPINK);	//colors used by a particular patch

	//markers of a patch
	blue_team->patches[4]->markers.push_back(new Marker());
	blue_team->patches[4]->markers[0]->colorid = cPINK;
	blue_team->patches[4]->markers.push_back(new Marker());
	blue_team->patches[4]->markers[1]->colorid = cGREEN;
	blue_team->patches[4]->markers.push_back(new Marker());
	blue_team->patches[4]->markers[2]->colorid = cPINK;
	blue_team->patches[4]->markers.push_back(new Marker());
	blue_team->patches[4]->markers[3]->colorid = cPINK;

	// dimensions
	p1.x = 0;
	p1.y = 240;
	p2.x = 320;
	p2.y = 240;
	p3.x = 320;
	p3.y = 0;
	p4.x = 0;
	p4.y = 0;
	h->setImagePoints(p1, p2, p3, p4);

	// Dimensions for robot soccer field
	//x = 1800;
	//y = 1500;

	u1.x = 0;
	u1.y = 1500;		//0-1
	u2.x = 1850;
	u2.y = 1500;		//1-1
	u3.x = 1850;
	u3.y = 0;		//1-0
	u4.x = 0;
	u4.y = 0;		//0-0
	h->setRealPoints(u1, u2, u3, u4);

	h->calcMH();

	CvSize imgSize = cvGetSize(img);

	for (int i = 0; i < NUM_COLOR_TYPES; i++) {
		(*ndata).push_back(new sData());
		(*ndata)[i]->image_hsv =
		    cvCreateImage(imgSize, img->depth, img->nChannels);
		(*ndata)[i]->segmentated = cvCreateImage(imgSize, 8, 1);
		cvSet((*ndata)[i]->segmentated, cvScalar(0));	// Clear image to black.
		(*ndata)[i]->morphKernel =
		    cvCreateStructuringElementEx(2, 2, 1, 1, CV_SHAPE_RECT,
						 NULL);
		(*ndata)[i]->labelImg =
		    cvCreateImage(imgSize, IPL_DEPTH_LABEL, 1);
		cvReleaseBlobs((*ndata)[i]->blobs);
		cvReleaseTracks((*ndata)[i]->tracks);
		(*ndata)[i]->blue_team = blue_team;
		(*ndata)[i]->yellow_team = yellow_team;
		(*ndata)[i]->ball = ball;
		(*ndata)[i]->enable = false;
		(*ndata)[i]->homography = h;
		(*ndata)[i]->n = 1;
		(*ndata)[i]->frameNumber = 0;
	}

	(*ndata)[blue_team->team_colorid]->enable = true;
	for (unsigned int i = 0; i < blue_team->usesColor.size(); ++i) {
		(*ndata)[blue_team->usesColor[i]]->enable = true;
	}
	(*ndata)[ball->color_id]->enable = true;
	return ndata;
}
