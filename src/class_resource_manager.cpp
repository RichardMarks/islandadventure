#include <allegro.h>
#include "support.h"
#include "class_resource_manager.h"


////////////////////////////////////////////////////////////////////////////////
bool is_image(const char* name)
{
	std::string n = name;
	n = n.substr(n.size() - 3);
	std::transform(n.begin(), n.end(), n.begin(), tolower);
	return "bmp" == n;
}
////////////////////////////////////////////////////////////////////////////////

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	std::vector <Resource*>::iterator iter = resources_.begin();
	while(iter != resources_.end())
	{
		if (*iter)
		{
			fprintf(stderr,"releasing resource %s\n", (*iter)->name_.c_str());
			delete *iter;
			resources_.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	table_.clear();
}

void ResourceManager::load(const char* resname)
{
	unsigned int id = resources_.size();

	if (is_image(resname))
	{
		ImageResource* resource = new ImageResource;
		resource->image_ = load_bitmap(resname, 0);
		if (resource)
		{
			resource->name_ = resname;
			resources_.push_back(resource);
			fprintf(stderr, "loaded %s\n", resname);
		}
		else
		{
			fprintf(stderr, "error cannot load %s\n", resname);
			return;
		}
	}
	else
	{
		fprintf(stderr, "error loading %s - not an image\n", resname);
		return;
	}

	table_[resname] = id;
}

Resource* ResourceManager::get(const char* resname)
{
	// very dangerous if the resource name is not right
	return resources_[table_[resname]];
}
