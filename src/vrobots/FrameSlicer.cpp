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
	int bH, bW;
	int i;
	bH = 0;
	bW = 0;
	bestRatio = std::numeric_limits < float >::infinity();
	for (i = 1; i <= parts; i++) {
		if ((parts % i) == 0) {
			col = parts / i;
			line = i;
			nW = (imgW + col - 1 +
			      ((BORDER / 2) * (col - 1))) / col;
			nH = (imgH + line - 1 +
			      ((BORDER / 2) * (line - 1))) / line;
			ratio = fabs(((nH * 1.0) / nW) - 1);
			if (ratio < bestRatio) {
				c = col;
				l = line;
				bW = nW;
				bH = nH;
				bestRatio = ratio;
			}
		}
	}
	Frame::minSize = cvSize(bW, bH);
	return 0;
}

int FrameSlicer::resetItem(Item * item)
{
	((Frame *) item)->initData();;
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
	img = frame->frame;
	imgH = img->height;
	imgW = img->width;
	if (oldNumParts != numParts) {
#pragma omp critical (calcOptimalSize)
		if (oldNumParts != numParts) {
			optimalSize(imgH, imgW, numParts);
			oldNumParts = numParts;
		}
	}
	parts = new Item *[numParts];
	for (i = 0; i < numParts; i++) {
		w = (imgW + c - 1 + ((BORDER / 2) * (c - 1))) / c;
		h = (imgH + l - 1 + ((BORDER / 2) * (l - 1))) / l;
		x = (i % c) * (w - BORDER);
		y = (i / c) * (h - BORDER);

		w = (x + w > imgW) ? imgW - x : w;
		h = (y + h > imgH) ? imgH - y : h;

		tImg = cvCreateImageHeader(cvSize(imgW, imgH), img->depth,
					   img->nChannels);
		tImg->imageData = img->imageData;
		tImg->imageDataOrigin = img->imageDataOrigin;
		cvSetImageROI(tImg, cvRect(x, y, w, h));

		parts[i] = new Frame(tImg);
	}
	return parts;
}

int FrameSlicer::delPart(Item * item)
{
	Frame *frame;
	frame = (Frame *) item;
	frame->frame->imageData = NULL;
	frame->frame->imageDataOrigin = NULL;
	frame->frame->imageSize = 0;
	frame->frame->height = 0;
	frame->frame->width = 0;
	delete frame;
	return 0;
}
