#ifndef __CLASS_XIT_H__
#define __CLASS_XIT_H__

#include <string>
#include "class_clickable.h"
struct BITMAP;

struct Xit : public Clickable
{
	std::string destname_;
	BITMAP* picture_;

	Xit();

	Xit(BITMAP* source, int x, int y, int* bounds);

	virtual ~Xit();

	void set_destination(const char* name)
	{
		destname_ = name;
	}

	void render(BITMAP* target);
};


#endif
