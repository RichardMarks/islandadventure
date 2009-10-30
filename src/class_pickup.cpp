
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
}
