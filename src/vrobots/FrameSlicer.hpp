#ifndef __FrameSlicer_HPP
#define __FrameSlicer_HPP

#include "../Slicer.hpp"
#include "Frame.hpp"
#include <cv.h>

class FrameSlicer:public Slicer {
 public:
	virtual Item ** slice(Item * item, int numParts);
	 FrameSlicer();

 private:
	int oldNumParts;
	int c, l;
	int optimalSize(int imgH, int imgW, int parts);
	static int BORDER;
};
#endif
