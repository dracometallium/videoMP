#include "FrameSlicer.hpp"
#include <cmath>

int FrameSlicer::BORDER = 50;

FrameSlicer::FrameSlicer()
{
	oldNumParts = 0;
}

int FrameSlicer::optimalSize(int imgH, int imgW, int parts)
{
	float bestRatio, ratio;
	int col, line;
	int nH, nW;
	int i;
	bestRatio = std::numeric_limits < float >::infinity();
	for (i = 1; i <= parts; i++) {
		if ((parts % i) == 0) {
			col = parts / i;
			line = i;
			nW = (imgW + col - 1) / col;
			nH = (imgH + line - 1) / line;
			ratio = fabs((nH / nW) - 1);
			if (ratio < bestRatio) {
				c = col;
				l = line;
				bestRatio = ratio;
			}
		}
	}
	return 0;
}

Item **FrameSlicer::slice(Item * item, int numParts)
{
	int imgH, imgW, i;
	int x, y, h, w;
	Frame *frame;
	Item **parts;
	IplImage *tImg, *img;
	frame = (Frame *) item;
	imgH = frame->frame->height;
	imgW = frame->frame->width;
#pragma omp critical
	if (oldNumParts != numParts) {
		optimalSize(imgH, imgW, numParts);
		oldNumParts = numParts;
	}
	parts = new Item *[numParts];
	img = frame->frame;
	for (i = 0; i < numParts; i++) {
		w = imgW / c;
		h = imgH / l;
		x = (i % c) * w - BORDER;
		y = (i / l) * l - BORDER;
		x = (x < 0) ? 0 : x;
		y = (y < 0) ? 0 : y;
		w = w + 2 * BORDER;
		y = y + 2 * BORDER;

		w = (x + w > imgW) ? imgW - x : w;
		h = (y + h > imgH) ? imgH - y : h;

		cvSetImageROI(img, cvRect(x, y, w, h));
		tImg = cvCreateImage(cvSize(w, h), img->depth, img->nChannels);
		cvCopy(tImg, img, NULL);
		cvResetImageROI(img);

		parts[i] = new Frame(tImg);
		((Frame *) parts[i])->initData();;
	}
	return parts;
}
