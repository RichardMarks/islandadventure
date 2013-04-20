#include <allegro.h>
#include "support.h"
#include "class_xit.h"

Xit::Xit() :
	picture_(0)
{
}

Xit::Xit(BITMAP* source, int x, int y, int* bounds) :
	Clickable(x, y, source->w, source->h, bounds, true),
	picture_(source)
{

}

Xit::~Xit()
{
}

void Xit::render(BITMAP* target)
{
	if (!visible_)
	{
		return;
	}
	draw_sprite(target, picture_, x_, y_);

	textprintf_ex(target, font, x_, y_, makecol(255,255,255), -1, "%s", name_.c_str());
}

