#ifndef __CLASS_IMAGERESOURCE_H__
#define __CLASS_IMAGERESOURCE_H__

#include "class_resource.h"
struct BITMAP;

struct ImageResource : public Resource
{
	BITMAP* image_;
	ImageResource(){}
	virtual ~ImageResource();
	operator BITMAP*(){return image_;}
	virtual bool ok(){ return (0 != image_); }
};

#endif
