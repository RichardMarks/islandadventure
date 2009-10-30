#ifndef __CLASS_RESOURCE_MANAGER_H__
#define __CLASS_RESOURCE_MANAGER_H__

#include <vector>
#include <map>

#include "class_imageresource.h"


struct ResourceManager
{
	std::vector <Resource*> resources_;
	std::map <std::string, unsigned int> table_;
	ResourceManager();
	~ResourceManager();
	void load(const char* resname);
	Resource* get(const char* resname);
};

#endif
