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
  \file    plugin_colorsegmentation.cpp
  \brief   C++ Implementation: PluginColorSegmentation
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "PluginColorSegmentation.hpp"

PluginColorSegmentation::PluginColorSegmentation(std::vector < sColor * >&color)
{
	this->color = color;
	segmentation.buildThreshold(color);
}

int PluginColorSegmentation::process(Item * item)
{
	Frame *frame;
	frame = (Frame *) item;
	CvSize imgS = cvGetSize((*frame->data)[0]->image_hsv);
	int hIn = imgS.height;
	int wIn = imgS.width;
	int rowSizeIn = (*frame->data)[0]->image_hsv->widthStep;	// Size of row in bytes, including extra padding
	char *imOfsIn = (*frame->data)[0]->image_hsv->imageData;	// Pointer to the start of the input image HSV pixels.

	for (int y = 0; y < hIn; y++) {
		for (int x = 0; x < wIn; x++) {
			// Get the HSV pixel components
			uchar H = imOfsIn[y * rowSizeIn + x * 3 + 0];	// Hue
			uchar S = imOfsIn[y * rowSizeIn + x * 3 + 1];	// Saturation
			uchar V = imOfsIn[y * rowSizeIn + x * 3 + 2];	// Value (Brightness)

			// Determina de qué color es el píxel HSV.
			unsigned c =
			    ((segmentation.h_class[H] & segmentation.
			      s_class[S]) & segmentation.v_class[V]);

			//TODO: cambiar 2050 por una constante definida en segmentation.cpp, que es el índice del arreglo nombrado "hash".
			if ((c != 0) && (c < HASH_INDEX_MAX)) {
				int colorId = segmentation.hash[c];
				if ((*frame->data)[colorId]->enable)
					(*frame->data)[colorId]->segmentated->
					    imageData[(*frame->data)
						      [colorId]->
						      segmentated->widthStep *
						      y + x] = (char)255;
			}
		}
	}

	return 0;
}

int PluginColorSegmentation::getPixelColorType(int H, int S, int V)
{
	int color;
	if (V < 75)
		color = cBLACK;
	else if (V > 190 && S < 27)
		color = cWHITE;
	else if (S < 53 && V < 185)
		color = cGREY;
	else {			// Is a color
		if (H < 14)
			color = cRED;
		else if (H < 25)
			color = cORANGE;
		else if (H < 34)
			color = cYELLOW;
		else if (H < 73)
			color = cGREEN;
		else if (H < 102)
			color = cAQUA;
		else if (H < 127)
			color = cBLUE;
		else if (H < 149)
			color = cPURPLE;
		else if (H < 175)
			color = cPINK;
		else		// full circle
			color = cRED;	// back to Red
	}
	return color;
}

inline int PluginColorSegmentation::getPixelColorTypeHSV(int H, int S, int V,
							 std::vector <
							 sColor * >&color)
{
	if (V < color[0]->hsv_max.value)
		return 0;	//cBLACK
	else if (V > color[1]->hsv_min.value
		 && S < color[1]->hsv_max.saturation)
		return 1;	//cWHITE;
	else if (S < color[2]->hsv_max.saturation
		 && V < color[2]->hsv_max.value)
		return 2;	//cGREY;
	else {			// Is a color
		for (unsigned int i = 3; i < color.size(); ++i) {
			if (color[i]->habilitar)
				if (H <= color[i]->hsv_max.hue) {
					return i;
				}
		}
	}

	return (-1);
}
