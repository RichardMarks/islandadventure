#ifndef __CLASS_PICKUP_H__
#define __CLASS_PICKUP_H__

#include "class_clickable.h"
struct BITMAP;

struct Pickup : public Clickable
{
	BITMAP* picture_;

	Pickup();

	Pickup(const char* name, BITMAP* source, int x, int y, int* bounds);

	virtual ~Pickup();

	void render(BITMAP* target);
};

#endif
