#include <allegro.h>
#include "support.h"
#include "class_imageresource.h"


ImageResource::~ImageResource()
{
	if(image_)
	{
		destroy_bitmap(image_);
	}
}
