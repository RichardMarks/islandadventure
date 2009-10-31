#include <allegro.h>
#include "support.h"
#include "class_scene.h"
#include "class_pickup.h"
#include "class_xit.h"
#include "class_scene_manager.h"
#include "class_scene_navigator.h"
#include "class_scene_loader.h"

////////////////////////////////////////////////////////////////////////////////

SceneLoader::SceneLoader(SceneManager* scenemanager, ResourceManager* resourcemanager, SceneNavigator* scenenavigator) :
	scenemanager_(scenemanager),
	resourcemanager_(resourcemanager),
	scenenavigator_(scenenavigator)
{
}

////////////////////////////////////////////////////////////////////////////////

SceneLoader::~SceneLoader()
{
}

////////////////////////////////////////////////////////////////////////////////

bool SceneLoader::parse_tokens(const char* filename, std::string& line, std::vector <std::string>& tokens)
{
	unsigned int tokencount = tokens.size();

	if (0x2 == tokencount)
	{
		std::string firsttoken = tokens.at(0);
		std::string secondtoken = tokens.at(1);
		std::transform(firsttoken.begin(), firsttoken.end(), firsttoken.begin(), tolower);
		std::transform(secondtoken.begin(), secondtoken.end(), secondtoken.begin(), tolower);
		return this->parse_blocks(filename, line, firsttoken, secondtoken);
	}
	else if (tokencount >= 3)
	{
		std::string property = tokens.at(0);
		std::string op = tokens.at(1);
		std::string value = tokens.at(2);
		std::transform(property.begin(), property.end(), property.begin(), tolower);

		if ("=" != op)
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\" (Bad Operator %s)\n", filename, line.c_str(), op.c_str());
			return false;
		}

		if (tokencount > 3)
		{
			for (unsigned int index = 3; index < tokencount; index++)
			{
				value = value + " " + tokens.at(index);
			}
		}

		return this->parse_property(filename, line, property, value);
	}
	else
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////


bool SceneLoader::parse_blocks(const char* filename, std::string& line, std::string& firsttoken, std::string& secondtoken)
{
	// begin blocks
	if ("begin" == firsttoken)
	{
		if ("scene" == secondtoken)
		{
			scene_ = new Scene;
			scene_->set_resource_manager(resourcemanager_);
			scene_->set_scene_navigator(scenenavigator_);
		}
		else if ("pickup" == secondtoken)
		{
			pickup_ = new Pickup;
			withinpickup_ = true;
		}
		else if ("exit" == secondtoken)
		{
			xit_ = new Xit;
			withinexit_ = true;
		}
		else
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\" (Unknown Block Identifier %s)\n", filename, line.c_str(), firsttoken.c_str());
			return false;
		}
	}
	// end blocks
	else if ("end" == firsttoken)
	{
		if ("scene" == secondtoken)
		{
			scenemanager_->add(scene_, scene_->name_.c_str());
		}
		else if ("pickup" == secondtoken)
		{
			scene_->add_pickup(pickup_);
			withinpickup_ = false;
		}
		else if ("exit" == secondtoken)
		{
			scene_->add_exit(xit_);
			withinexit_ = false;
		}
		else
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\" (Unknown Block Identifier %s)\n", filename, line.c_str(), firsttoken.c_str());
			return false;
		}
	}
	// bad block
	else
	{
		fprintf(stderr, "error in scn file: %s in line: \"%s\" (Expected Block)\n", filename, line.c_str());
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool SceneLoader::parse_property(const char* filename, std::string& line, std::string& property, std::string& value)
{
	// universal properties
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if ("name" == property)
	{
		if (withinpickup_)
		{
			scene_->set_pickup_name(pickup_, value.c_str());
		}
		else if (withinexit_)
		{
			scene_->set_exit_name(xit_, value.c_str());
		}
		else
		{
			fprintf(stderr, "set scene name to \"%s\"\n", value.c_str());
			scene_->set_name(value.c_str());
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("image" == property)
	{
		//fprintf(stderr, "TODO: set scene background to \"%s\"\n", value.c_str());
		if (withinpickup_)
		{
			scene_->set_pickup_image(pickup_, value.c_str());
		}
		else if (withinexit_)
		{
			scene_->set_exit_image(xit_, value.c_str());
		}
		else
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\"(image property not found in curent block)\n", filename, line.c_str());
			return false;
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("x" == property)
	{
		//fprintf(stderr, "TODO: set x to %d\n", atoi(value.c_str()));
		if (withinpickup_)
		{
			pickup_->set_x(atoi(value.c_str()));
		}
		else if (withinexit_)
		{
			xit_->set_x(atoi(value.c_str()));
		}
		else
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\"(X property not found in current block)\n", filename, line.c_str());
			return false;
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("y" == property)
	{
		//fprintf(stderr, "TODO: set y to %d\n", atoi(value.c_str()));
		if (withinpickup_)
		{
			pickup_->set_y(atoi(value.c_str()));
		}
		else if (withinexit_)
		{
			xit_->set_y(atoi(value.c_str()));
		}
		else
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\"(Y property not found in current block)\n", filename, line.c_str());
			return false;
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// scene properties
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("background" == property)
	{
		fprintf(stderr, "set scene background to \"%s\"\n", value.c_str());
		scene_->set_background(value.c_str());
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("death" == property)
	{
		std::transform(value.begin(), value.end(), value.begin(), tolower);
		if ("true" == value)
		{
			scene_->set_death_scene(true);
			scene_->set_win_scene(false);
		}
		else
		{
			scene_->set_death_scene(false);
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("win" == property)
	{
		std::transform(value.begin(), value.end(), value.begin(), tolower);
		if ("true" == value)
		{
			scene_->set_win_scene(true);
			scene_->set_death_scene(false);
		}
		else
		{
			scene_->set_win_scene(false);
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("screen" == property)
	{
		std::transform(value.begin(), value.end(), value.begin(), tolower);
		if ("true" == value)
		{
			scene_->set_screen_scene(true);
			scene_->set_death_scene(false);
			scene_->set_win_scene(false);
		}
		else
		{
			scene_->set_screen_scene(false);
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("pickups" == property)
	{
		fprintf(stderr, "set scene pickup count to %d\n", atoi(value.c_str()));
		scene_->set_total_pickups(atoi(value.c_str()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("exits" == property)
	{
		fprintf(stderr, "set scene exit count to %d\n", atoi(value.c_str()));
		scene_->set_total_exits(atoi(value.c_str()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// pickup properties
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("bounds" == property)
	{
		std::vector <std::string> tokens = string2tokens(value, " ");
		unsigned int tokencount = tokens.size();
		if (4 != tokencount)
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\"(not enough args for bounds property. Expected: Left, Top, Right, Bottom)\n", filename, line.c_str());
			return false;
		}

		fprintf(stderr,
			"set pickup image collision bounds to [%d,%d,%d,%d]\n",
			atoi(tokens.at(0).c_str()),
			atoi(tokens.at(1).c_str()),
			atoi(tokens.at(2).c_str()),
			atoi(tokens.at(3).c_str())
		);

		pickup_->set_collision_bounds(
			atoi(tokens.at(0).c_str()),
			atoi(tokens.at(1).c_str()),
			atoi(tokens.at(2).c_str()),
			atoi(tokens.at(3).c_str())
		);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// exit properties
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if ("dest" == property)
	{
		fprintf(stderr, "set exit destination to \"%s\"\n", value.c_str());
		xit_->set_destination(value.c_str());
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else
	{
		fprintf(stderr, "error in scn file: %s in line: \"%s\"(Unknown Property %s)\n", filename, line.c_str(), property.c_str());
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool SceneLoader::load(const char* filename)
{
	fprintf(stderr, "preparsing %s\n", filename);
	std::vector <std::string> lines = txt2lines(filename, "#", false);
	if (!lines.size())
	{
		fprintf(stderr, "preparse failed on %s\n", filename);
		return false;
	}
	fprintf(stderr, "preparse finished\n");
	fprintf(stderr, "parsing %s (%lu lines)\n", filename, lines.size());

	scene_ = 0;
	pickup_ = 0;
	xit_ = 0;
	withinpickup_ = false;
	withinexit_ = false;

	std::vector <std::string> ::iterator iter;
	for (iter = lines.begin(); iter != lines.end(); iter++)
	{
		std::string& line = *iter;
		fprintf(stderr, "> %s\n", line.c_str());
		std::vector <std::string> tokens = string2tokens(line, " \t");

		if (!this->parse_tokens(filename, line, tokens))
		{
			return false;
		}
	}

	fprintf(stderr, "parsing finished\n");

	return true;
}
