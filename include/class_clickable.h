#ifndef __CLASS_CLICKABLE_H__
#define __CLASS_CLICKABLE_H__

#include <string>

struct Clickable
{
	std::string name_;
	int x_;
	int y_;
	int w_;
	int h_;
	bool lmbdown_;
	int left_, top_, right_, bottom_;
	bool visible_;

	Clickable();
	Clickable(int x, int y, int w, int h, int* bounds, bool vis);
	virtual ~Clickable();

	void set_x(int x){ x_ = x; }

	void set_y(int y){ y_ = y; }

	void set_collision_bounds(int l, int t, int r, int b)
	{
		left_ = l;
		top_ = t;
		right_ = r;
		bottom_ = b;
	}

	virtual bool hover();

	virtual bool clicked();

	void make_invisible()
	{
		visible_ = false;
	}

	void make_visible()
	{
		visible_ = true;
	}
};

#endif
