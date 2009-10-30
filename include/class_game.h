#ifndef __CLASS_GAME_H__
#define __CLASS_GAME_H__

struct ResourceManager;
struct SceneManager;

struct Game
{
	ResourceManager* resmanager_;
	SceneManager* scenemanager_;

	int total_; // how many objects you have found total
	int found_; // how many objects you have found in this scene
	int scene_; // which scene you are on

	Game();
	~Game();
	void update();
	void render(BITMAP* target);
};

static volatile bool mainthreadisrunning = true;

#endif
