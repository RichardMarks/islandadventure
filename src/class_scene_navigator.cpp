#include <allegro.h>
#include "support.h"
#include "class_scene.h"
#include "class_scene_manager.h"
#include "class_scene_navigator.h"

SceneNavigator::SceneNavigator(SceneManager* scenemanager) :
	scenemanager_(scenemanager)
{
}

SceneNavigator::~SceneNavigator()
{
}

void SceneNavigator::travel(const char* destination)
{
	Scene* scene = scenemanager_->get(destination);

	scene_ = scene->id_;
}
