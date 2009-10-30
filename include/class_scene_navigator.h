#ifndef __CLASS_SCENE_NAVIGATOR_H__
#define __CLASS_SCENE_NAVIGATOR_H__

struct SceneManager;
struct BITMAP;

struct SceneNavigator
{
	int found_;
	unsigned int scene_;
	SceneManager* scenemanager_;
	SceneNavigator(SceneManager* scenemanager);

	~SceneNavigator();

	void travel(const char* destination);

	bool update();
	void render(BITMAP* target);
};


#endif
