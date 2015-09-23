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
			nW = imgW / col + BORDER;
			nH = imgH / line + BORDER;
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
	int x, y, h, w, zw, zh;
	Frame *frame;
	Item **parts;
	IplImage *tImg, *img;
	frame = (Frame *) item;
	img = frame->frame;
	imgH = img->height;
	imgW = img->width;
	if (oldNumParts != numParts) {
#pragma omp critical
		if (oldNumParts != numParts) {
			optimalSize(imgH, imgW, numParts);
			oldNumParts = numParts;
		}
	}
	parts = new Item *[numParts];
	for (i = 0; i < numParts; i++) {
		w = imgW / col + BORDER;
		h = imgH / line + BORDER;
		x = (i % c) * (w - BORDER);
		y = (i / c) * (h - BORDER);

		zw = (x + w > imgW) ? imgW - x : w;
		zh = (y + h > imgH) ? imgH - y : h;

		cvSetImageROI(img, cvRect(x, y, zw, zh));
		tImg = cvCreateImage(cvSize(w, h), img->depth, img->nChannels);
		cvSet(tImg, cvScalar(0));	// Clear image to black.
		cvSetImageROI(tImg, cvRect(0, 0, zw, zh));
		cvCopy(img, tImg, NULL);
		cvResetImageROI(tImg);
		cvResetImageROI(img);

		parts[i] = new Frame(tImg);
		((Frame *) parts[i])->initData();;
	}
	return &(parts[0]);
}
