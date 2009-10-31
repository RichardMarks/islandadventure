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

	if (!this->load_startup())
	{
		mainthreadisrunning = false;
	}

	if (!this->load_resources())
	{
		mainthreadisrunning = false;
	}

	if (!this->load_scenes())
	{
		mainthreadisrunning = false;
	}

#if 0
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
	resmanager_->load("contestlogo.png");

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
#endif
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

bool Game::update()
{
	if (!navigator_->update())
	{
		return false;
	}
	return true;
}

void Game::render(BITMAP* target)
{
	navigator_->render(target);

	ImageResource* watermark = (ImageResource*) resmanager_->resources_.at(16);
	draw_sprite(target, *watermark, target->w - (*watermark).image_->w, target->h - (*watermark).image_->h);
}

bool Game::load_startup()
{
	const char* filename = "startup.dat";

	fprintf(stderr, "preparsing %s\n", filename);
	std::vector <std::string> lines = txt2lines(filename, "#", false);
	if (!lines.size())
	{
		fprintf(stderr, "preparse failed on %s\n", filename);
		return false;
	}

	std::vector <std::string> ::iterator iter;
	for (iter = lines.begin(); iter != lines.end(); iter++)
	{
		std::string& line = *iter;
		fprintf(stderr, "> %s\n", line.c_str());
		std::vector <std::string> tokens = string2tokens(line, " \t");
		unsigned int tokencount = tokens.size();

		if (tokencount >= 3)
		{
			std::string property = tokens.at(0);
			std::string op = tokens.at(1);
			std::string value = tokens.at(2);
			std::transform(property.begin(), property.end(), property.begin(), tolower);

			if ("=" != op)
			{
				fprintf(stderr, "error in dat file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}

			if (tokencount > 3)
			{
				for (unsigned int index = 3; index < tokencount; index++)
				{
					value = value + " " + tokens.at(index);
				}
			}

			if ("resource" == property)
			{
				resourcelist_.push_back(value.c_str());
			}
			else if ("scene" == property)
			{
				scenelist_.push_back(value.c_str());
			}
			else
			{
				fprintf(stderr, "error in dat file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
		}
		else
		{
			fprintf(stderr, "error in dat file: %s in line: \"%s\"\n", filename, line.c_str());
			return false;
		}
	}

	return true;
}

bool Game::load_resources()
{
	std::vector <std::string>::iterator iter;
	for (iter = resourcelist_.begin(); iter != resourcelist_.end(); iter++)
	{
		if (!resmanager_->load((*iter).c_str()))
		{
			return false;
		}
	}
	resourcelist_.clear();
	return true;
}

bool Game::load_scenes()
{
	SceneLoader loader(scenemanager_, resmanager_, navigator_);

	std::vector <std::string>::iterator iter;
	for (iter = scenelist_.begin(); iter != scenelist_.end(); iter++)
	{
		if (!loader.load((*iter).c_str()))
		{
			return false;
		}
	}
	scenelist_.clear();
	return true;
}


