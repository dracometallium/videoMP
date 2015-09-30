#ifndef __Item_HPP
#define __Item_HPP

class Item {
 public:
	Item();
	virtual ~ Item();
	virtual int delPart();
	double time;
};

#endif
