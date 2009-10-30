#ifndef __CLASS_SCENE_NAVIGATOR_H__
#define __CLASS_SCENE_NAVIGATOR_H__

struct SceneManager;

struct SceneNavigator
{
	unsigned int scene_;
	SceneManager* scenemanager_;
	SceneNavigator(SceneManager* scenemanager);

	~SceneNavigator();

	void travel(const char* destination);
};


#endif
