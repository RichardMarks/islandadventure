
#include <allegro.h>
#include "support.h"
#include "class_pickup.h"

Pickup::Pickup() :
	picture_(0)
{
}

Pickup::Pickup(const char* name, BITMAP* source, int x, int y, int* bounds) :
	Clickable(x, y, source->w, source->h, bounds, true),
	picture_(source)
{
	name_ = name;
}

Pickup::~Pickup()
{
}

void Pickup::render(BITMAP* target)
{
	if (!visible_)
	{
		return;
	}
	draw_sprite(target, picture_, x_, y_);

	if (this->hover())
	{
	rect(target,
		x_ + left_,
		y_ + top_,
		x_ + left_ + (w_ - (left_ + right_)),
		y_ + top_ + (h_ - (top_ + bottom_)),
		 makecol(255,0,0));
	textprintf_ex(target, font, x_, y_, makecol(255,255,255), -1, "Take %s", name_.c_str());
	}
}
