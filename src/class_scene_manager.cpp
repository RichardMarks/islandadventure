#include <allegro.h>
#include "support.h"
#include "class_scene.h"
#include "class_scene_manager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	std::vector <Scene*>::iterator iter = scenes_.begin();
	while(iter != scenes_.end())
	{
		if (*iter)
		{
			fprintf(stderr,"destroying scene %s\n", (*iter)->name_.c_str());
			delete *iter;
			scenes_.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	table_.clear();
}

void SceneManager::setup(ResourceManager* res)
{
	fprintf(stderr,"setting up scene manager\n");
	resources_ = res;
	if (!res)
	{
		fprintf(stderr,"caution..resource manager pointer failure!\n");
	}
}

void SceneManager::add(Scene* scene, const char* name)
{
	fprintf(stderr,"adding scene %s\n", name);
	scene->resources_ = resources_;
	scene->name_ = name;
	unsigned int id = scenes_.size();
	scene->id_ = id;
	scenes_.push_back(scene);
	table_[name] = id;
}

Scene* SceneManager::get(const char* name)
{
	// very dangerous if the name is not right
	return scenes_[table_[name]];
}

void SceneManager::reset()
{
	std::vector <Scene*>::iterator iter;
	for (iter = scenes_.begin(); iter != scenes_.end(); iter++)
	{
		(*iter)->reset();
	}
}

