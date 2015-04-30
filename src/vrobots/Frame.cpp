#include "Frame.hpp"

Frame::Frame(IplImage * iframe)
 : Item()
{
	frame = iframe;
	data=cdata;
}

Frame::~Frame()
{
	delete frame;
}
