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
	bestRatio = std::numeric_limits < float >::infinity();
	for (col = 1; c < (parts - 1); col++) {
		line = parts - col;
		nH = imgH + ((line - (imgH % line)) % line);
		nW = imgW + ((col - (imgW % col)) % col);
		ratio = fabs(((nH / line) / (nW / col)) - 1);
		if (ratio < bestRatio) {
			c = col;
			l = line;
			bestRatio = ratio;
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
	frame = (Frame *) item;
	imgH = frame->frame->height;
	imgW = frame->frame->width;
#pragma omp critical
	if (oldNumParts != numParts) {
		optimalSize(imgH, imgW, numParts);
		oldNumParts = numParts;
	}
	parts = new Item *[numParts];
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

		parts[i] = new Frame(frame->frame);
		((Frame *)parts[i])->initData();;
	}
	return parts;
}
