#ifndef __CLASS_SCENE_LOADER_H__
#define __CLASS_SCENE_LOADER_H__

struct SceneManager;
struct ResourceManager;

struct SceneLoader
{
	SceneManager* scenemanager_;
	ResourceManager* resourcemanager_;

	SceneLoader(SceneManager* scenemanager, ResourceManager* resourcemanager);
	~SceneLoader();

	bool load(const char* filename);
};

#endif
