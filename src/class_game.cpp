#include <allegro.h>
#include "support.h"
#include "class_scene_manager.h"
#include "class_resource_manager.h"
#include "class_scene_loader.h"
#include "class_scene.h"
#include "class_scene_navigator.h"
#include "class_game.h"


Game::Game()
{
	scenemanager_ = new SceneManager();
	resmanager_ = new ResourceManager();
	navigator_ = new SceneNavigator(scenemanager_);

	resmanager_->load("scene1.png");
	resmanager_->load("scene2.png");
	resmanager_->load("scene3.png");
	resmanager_->load("scene4.png");
	resmanager_->load("scene5.png");
	resmanager_->load("scene6.png");
	resmanager_->load("scene7.png");
	resmanager_->load("scene8.png");
	resmanager_->load("scene9.png");
	resmanager_->load("rope.png");
	resmanager_->load("candle.png");
	resmanager_->load("matches.png");
	resmanager_->load("north.png");
	resmanager_->load("south.png");
	resmanager_->load("east.png");
	resmanager_->load("west.png");

	SceneLoader loader(scenemanager_, resmanager_, navigator_);
	if (!loader.load("scene1.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene2.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene3.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene4.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene5.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene6.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene7.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene8.scn"))
	{
		mainthreadisrunning = false;
	}
	if (!loader.load("scene9.scn"))
	{
		mainthreadisrunning = false;
	}

	total_ = 0;
	found_ = 0;
	scene_ = 0;
}

Game::~Game()
{
	delete resmanager_;
	delete scenemanager_;
	delete navigator_;
}

void Game::update()
{
	if (!navigator_->update())
	{
		mainthreadisrunning = false;
	}
}

void Game::render(BITMAP* target)
{
	navigator_->render(target);
}
