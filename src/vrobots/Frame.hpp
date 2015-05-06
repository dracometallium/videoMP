#ifndef __Frame_HPP
#define __Frame_HPP
#include "../Item.hpp"
#include <cv.h>
#include "datastruct.h"
class Frame:public Item {
 public:
	Frame(IplImage * iframe);
	~Frame();
	IplImage *frame;
	static std::vector < sData * >bdata;
	static int Init(IplImage * img);
	 std::vector < sData * >*data;
};
#endif
