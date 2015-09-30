#ifndef __Frame_HPP
#define __Frame_HPP
#include "../Item.hpp"
#include <cv.h>
#include "datastruct.h"
class Frame:public Item {
 public:
	Frame(IplImage * iframe);
	virtual ~ Frame();
	virtual int delPart();
	IplImage *frame;
	static std::vector < std::vector < sData * >*>dpool;
	static std::vector < sData * >*newData(IplImage * img);
	static CvSize minSize;
	void resetData();
	int initData();
	 std::vector < sData * >*data;
};
#endif
