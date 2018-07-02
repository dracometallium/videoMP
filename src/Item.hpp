#ifndef __Item_HPP
#define __Item_HPP

class Item {
 public:
	Item();
	virtual ~ Item();
	double time;
	static bool ignore;
	int num;
};

#endif
