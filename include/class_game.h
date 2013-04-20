#ifndef __CLASS_GAME_H__
#define __CLASS_GAME_H__

#include <vector>
#include <string>

struct ResourceManager;
struct SceneManager;

struct Game
{
	ResourceManager* resmanager_;
	SceneManager* scenemanager_;
	SceneNavigator* navigator_;

	int total_; // how many objects you have found total
	int found_; // how many objects you have found in this scene
	int scene_; // which scene you are on

	// used by the startup routine
	std::vector <std::string> resourcelist_;
	std::vector <std::string> scenelist_;

	Game();
	~Game();
	bool update();
	void render(BITMAP* target);

	bool load_startup();
	bool load_resources();
	bool load_scenes();
};

static volatile bool mainthreadisrunning = true;

#endif
