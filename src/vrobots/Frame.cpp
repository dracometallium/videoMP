#include "Frame.hpp"

Frame::Frame(IplImage * iframe)
 : Item()
{
	frame = iframe;
}

Frame::~Frame()
{
	delete frame;
}
