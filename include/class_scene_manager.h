#ifndef __CLASS_SCENE_MANAGER_H__
#define __CLASS_SCENE_MANAGER_H__

#include <vector>
#include <map>
#include <string>

struct ResourceManager;
struct Scene;

struct SceneManager
{
	ResourceManager* resources_;
	std::vector <Scene*> scenes_;
	std::map <std::string, unsigned int> table_;
	SceneManager();
	~SceneManager();
	void setup(ResourceManager* res);
	void add(Scene* scene, const char* name);
	Scene* get(const char* name);
};


#endif
