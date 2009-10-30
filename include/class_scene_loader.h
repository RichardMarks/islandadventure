#ifndef __CLASS_SCENE_LOADER_H__
#define __CLASS_SCENE_LOADER_H__

struct SceneManager;
struct ResourceManager;
struct SceneNavigator;

struct SceneLoader
{
	SceneManager* scenemanager_;
	ResourceManager* resourcemanager_;
	SceneNavigator* scenenavigator_;

	SceneLoader(SceneManager* scenemanager, ResourceManager* resourcemanager, SceneNavigator* scenenavigator);
	~SceneLoader();

	bool load(const char* filename);
};

#endif
