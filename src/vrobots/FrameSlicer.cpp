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
			nH = imgH / line + BORDER;
			nW = imgW / col + BORDER;
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
	int x, y, xe, ye, w, h;
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
		w = imgW / c + BORDER;
		h = imgH / l + BORDER;
		x = (i % c) * (w - BORDER);
		y = (i / c) * (h - BORDER);

		w = (x + w > imgW) ? imgW - x : w;
		h = (y + h > imgH) ? imgH - y : h;

		//printf("%d:%d,%d:%d,%d:%dx%d\n", i, i % c, i / c, x, y, w, h);

		tImg = cvCreateImageHeader(cvSize(imgW, imgH), img->depth,
					   img->nChannels);
		tImg->imageData = img->imageData;
		cvSetImageROI(tImg, cvRect(x, y, w, h));

		parts[i] = new Frame(tImg);
		((Frame *) parts[i])->initData();;
	}
	return &(parts[0]);
}
