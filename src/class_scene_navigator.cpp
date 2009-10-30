#include <allegro.h>
#include "support.h"
#include "class_scene.h"
#include "class_scene_manager.h"
#include "class_scene_navigator.h"

SceneNavigator::SceneNavigator(SceneManager* scenemanager) :
	scenemanager_(scenemanager)
{
	scene_ = 0;
	found_ = 0;
}

SceneNavigator::~SceneNavigator()
{
}

void SceneNavigator::travel(const char* destination)
{
	fprintf(stderr, "traveling to %s...\n", destination);
	Scene* scene = scenemanager_->get(destination);

	fprintf(stderr, "entering scene %u from scene %u\n", scene->id_, scene_);
	scene_ = scene->id_;

}

bool SceneNavigator::update()
{
	// reset demo
	if (key[KEY_R])
	{
		scene_ = 0;
	}

	if (scene_ >= scenemanager_->scenes_.size())
	{
		return false;
	}

	scenemanager_->scenes_.at(scene_)->update();

	return true;
}

void SceneNavigator::render(BITMAP* target)
{
	if (scene_ >= scenemanager_->scenes_.size())
	{
		return;
	}

	Scene* s = scenemanager_->scenes_.at(scene_);
	s->render(target);
	found_ = s->found_;

	std::string scenename = s->name_;
	textprintf_ex(target,font,0,0,makecol(0,0,0),-1,"Found %d/%d items. Scene %d [%s]",
	found_,s->totalpickups_, 1+scene_,scenename.c_str());
	textprintf_ex(target,font,1,1,makecol(0,0,0),-1,"Found %d/%d items. Scene %d [%s]",found_,s->totalpickups_, 1+scene_,scenename.c_str());
	textprintf_ex(target,font,2,2,makecol(255,255,255),-1,"Found %d/%d items. Scene %d [%s]",found_,s->totalpickups_, 1+scene_,scenename.c_str());
}
