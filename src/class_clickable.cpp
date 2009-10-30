
#include <allegro.h>
#include "support.h"
#include "class_clickable.h"


Clickable::Clickable() :
	x_(0), y_(0), w_(0), h_(0), lmbdown_(false), visible_(false)
{
	left_ = 0;
	top_ = 0;
	right_ = 0;
	bottom_ = 0;
}

Clickable::Clickable(int x, int y, int w, int h, int* bounds, bool vis) :
	x_(x), y_(y), w_(w), h_(h), lmbdown_(false), visible_(vis)
{
	left_ = bounds[0];
	top_ = bounds[1];
	right_ = bounds[2];
	bottom_ = bounds[3];
}

Clickable::~Clickable()
{
}

bool Clickable::hover()
{
	int box[]=
	{
		x_ + left_,
		y_ + top_,
		(x_ + left_) + (w_ - right_),
		(y_ + top_) + (h_ - bottom_)
	};
	if (mouse_x >= box[0] && mouse_x <= box[2] && mouse_y >= box[1] && mouse_y <= box[3])
	{
		return true;
	}
	return false;
}

bool Clickable::clicked()
{
	if (!visible_)
	{
		return false;
	}
	if (!this->hover())
	{
		return false;
	}

	if (mouse_b & 1)
	{
		if (!lmbdown_){lmbdown_=true;}
	}
	else
	{
		if (lmbdown_){lmbdown_=false;return true;}
	}
	return false;
}
