#include <allegro.h>
#include "support.h"
#include "class_imageresource.h"
#include "class_resource_manager.h"
#include "class_pickup.h"
#include "class_xit.h"
#include "class_scene_navigator.h"
#include "class_scene.h"

Scene::Scene() : found_(0)
{
}

void Scene::set_resource_manager(ResourceManager* resmgr)
{
	resources_ = resmgr;
}

void Scene::set_background(const char* resname)
{
	ImageResource* res = (ImageResource*) resources_->get(resname);
	if (res)
	{
		if (res->ok())
		{
			background_ = *res;
		}
	}
}

void Scene::set_pickup_image(Pickup* pickup, const char* resname)
{
	ImageResource* res = (ImageResource*) resources_->get(resname);
	if (res)
	{
		if (res->ok())
		{
			pickup->picture_ = *res;
		}
	}
}

void Scene::set_exit_image(Xit* xit, const char* resname)
{
	ImageResource* res = (ImageResource*) resources_->get(resname);
	if (res)
	{
		if (res->ok())
		{
			xit->picture_ = *res;
		}
	}
}

void Scene::set_name(const char* name)
{
	name_ = name;
}

void Scene::set_total_pickups(int amount)
{
	totalpickups_ = amount;
}

void Scene::set_total_exits(int amount)
{
	totalexits_ = amount;
}

void Scene::add_pickup(Pickup* p)
{
	pickups_.push_back(p);
}
void Scene::add_pickup(const char* name, int x, int y, int* box)
{
	ImageResource* res = (ImageResource*) resources_->get(name);
	if (res)
	{
		if (res->ok())
		{
			pickups_.push_back(new Pickup(name, *res, 0, 0, box));
		}
	}
}
void Scene::add_exit(Xit* n)
{
	exits_.push_back(n);
}

void Scene::add_exit_north(int x, int y)
{
	ImageResource* res = (ImageResource*) resources_->get("north.bmp");
	if (res)
	{
		if (res->ok())
		{
			int box[4];makebox(box, 0, 0, 0, 0);
			this->add_exit(new Xit(*res, x, y, box));
		}
	}
}

void Scene::add_exit_south(int x, int y)
{
	ImageResource* res = (ImageResource*) resources_->get("south.bmp");
	if (res)
	{
		if (res->ok())
		{
			int box[4];makebox(box, 0, 0, 0, 0);
			this->add_exit(new Xit(*res, x, y, box));
		}
	}
}

void Scene::add_exit_east(int x, int y)
{
	ImageResource* res = (ImageResource*) resources_->get("east.bmp");
	if (res)
	{
		if (res->ok())
		{
			int box[4];makebox(box, 0, 0, 0, 0);
			this->add_exit(new Xit(*res, x, y, box));
		}
	}
}

void Scene::add_exit_west(int x, int y)
{
	ImageResource* res = (ImageResource*) resources_->get("west.bmp");
	if (res)
	{
		if (res->ok())
		{
			int box[4];makebox(box, 0, 0, 0, 0);
			this->add_exit(new Xit(*res, x, y, box));
		}
	}
}

void Scene::releasepickups()
{
	std::vector <Pickup*>::iterator iter = pickups_.begin();
	while(iter != pickups_.end())
	{
		if (*iter)
		{
			delete *iter;
			pickups_.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void Scene::releaseexits()
{
	std::vector <Xit*>::iterator iter = exits_.begin();
	while(iter != exits_.end())
	{
		if (*iter)
		{
			delete *iter;
			exits_.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

Scene::~Scene()
{
	this->releasepickups();
	this->releaseexits();
}

void Scene::update_pickups()
{
	std::vector <Pickup*>::iterator iter;
	for (iter = pickups_.begin(); iter != pickups_.end(); iter++)
	{
		if ((*iter)->clicked())
		{
			fprintf(stderr, "found %s\n", (*iter)->name_.c_str());
			(*iter)->make_invisible();
			found_++;
		}
	}
}

std::string Scene::update_exits()
{
	std::string result = "idle";

	std::vector <Xit*>::iterator iter;
	for (iter = exits_.begin(); iter != exits_.end(); iter++)
	{
		Xit* xit = *iter;
		if (xit->hover())
		{
			xit->make_visible();
		}
		else
		{
			xit->make_invisible();
		}

		if (xit->clicked())
		{
			result = xit->destname_.c_str();
		}
	}

	return result;
}

bool Scene::update()
{
	this->update_pickups();
	std::string dest = this->update_exits();
	if ("idle" != dest)
	{
		navigator_->travel(dest.c_str());
	}

	return false;
}

void Scene::render_pickups(BITMAP* target)
{
	std::vector <Pickup*>::iterator iter;
	for (iter = pickups_.begin(); iter != pickups_.end(); iter++)
	{
		(*iter)->render(target);
	}
}

void Scene::render_exits(BITMAP* target)
{
	std::vector <Xit*>::iterator iter;
	for (iter = exits_.begin(); iter != exits_.end(); iter++)
	{
		(*iter)->render(target);
	}
}

void Scene::render(BITMAP* target)
{
	blit(background_, target, 0, 0, 0, 0, target->w, target->h);

	this->render_pickups(target);
	this->render_exits(target);
}
