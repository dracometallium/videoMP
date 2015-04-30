#ifndef __Frame_HPP
#define __Frame_HPP
#include "../Item.hpp"
#include <cv.h>
#include "datastruct.h"
class Frame:public Item {
 public:
	Frame(IplImage * iframe);
	~Frame();
	static int setdata(std::vector < sData * >*ndata) {
		cdata = ndata;
		return 0;
	};
	IplImage *frame;
	static std::vector < sData * >*cdata;
	std::vector < sData * >*data;
};
#endif
