#include <allegro.h>
#include "support.h"
#include "class_scene_manager.h"
#include "class_resource_manager.h"
#include "class_scene_loader.h"
#include "class_scene.h"
#include "class_game.h"


Game::Game()
{
	scenemanager_ = new SceneManager();
	resmanager_ = new ResourceManager();

	resmanager_->load("scene1.bmp");
	resmanager_->load("scene2.bmp");
	resmanager_->load("scene3.bmp");
	resmanager_->load("scene4.bmp");
	resmanager_->load("scene5.bmp");
	resmanager_->load("scene6.bmp");
	resmanager_->load("scene7.bmp");
	resmanager_->load("scene8.bmp");
	resmanager_->load("scene9.bmp");
	resmanager_->load("rope.bmp");
	resmanager_->load("candle.bmp");
	resmanager_->load("matches.bmp");
	resmanager_->load("north.bmp");
	resmanager_->load("south.bmp");
	resmanager_->load("east.bmp");
	resmanager_->load("west.bmp");

	SceneLoader loader(scenemanager_, resmanager_);
	if (!loader.load("scene1.scn"))
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
}

void Game::update()
{
	if (scene_ >= (int)scenemanager_->scenes_.size())
	{
		return;
	}
	scenemanager_->scenes_.at(scene_)->update();
}

void Game::render(BITMAP* target)
{
	if (scene_ >= (int)scenemanager_->scenes_.size())
	{
		return;
	}
	Scene* s = scenemanager_->scenes_.at(scene_);
	s->update();
	found_ = s->found_;

	std::string scenename = s->name_;
	textprintf_ex(target,font,0,0,makecol(0,0,0),-1,"Found %d/%d items. Scene %d [%s]",
	found_,s->totalpickups_, 1+scene_,scenename.c_str());
	textprintf_ex(target,font,1,1,makecol(0,0,0),-1,"Found %d/%d items. Scene %d [%s]",found_,s->totalpickups_, 1+scene_,scenename.c_str());
	textprintf_ex(target,font,2,2,makecol(255,255,255),-1,"Found %d/%d items. Scene %d [%s]",found_,s->totalpickups_, 1+scene_,scenename.c_str());
}
