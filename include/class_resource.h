#ifndef __CLASS_RESOURCE_H__
#define __CLASS_RESOURCE_H__

#include <string>

struct Resource
{
	std::string name_;
	Resource(){}
	virtual ~Resource(){}
	virtual bool ok() = 0;
};

#endif
