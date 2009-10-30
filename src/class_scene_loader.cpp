#include <allegro.h>
#include "support.h"
#include "class_scene.h"
#include "class_pickup.h"
#include "class_xit.h"
#include "class_scene_manager.h"
#include "class_scene_loader.h"


SceneLoader::SceneLoader(SceneManager* scenemanager, ResourceManager* resourcemanager) :
	scenemanager_(scenemanager),
	resourcemanager_(resourcemanager)
{
}

SceneLoader::~SceneLoader()
{
}

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

	Scene* scene = 0;
	Pickup* pickup = 0;
	Xit* xit = 0;
	bool withinpickup = false;
	bool withinexit = false;

	std::vector <std::string> ::iterator iter;
	for (iter = lines.begin(); iter != lines.end(); iter++)
	{

		std::string& line = *iter;
		fprintf(stderr, "> %s\n", line.c_str());
		std::vector <std::string> tokens = string2tokens(line, " \t");
		unsigned int tokencount = tokens.size();

		// is this a begin or end statement ?
		if (0x2 == tokencount)
		{
			std::string firsttoken = tokens.at(0);
			std::string secondtoken = tokens.at(1);
			std::transform(firsttoken.begin(), firsttoken.end(), firsttoken.begin(), tolower);
			std::transform(secondtoken.begin(), secondtoken.end(), secondtoken.begin(), tolower);

			if ("begin" == firsttoken)
			{
				if ("scene" == secondtoken)
				{
					scene = new Scene;
					scene->set_resource_manager(resourcemanager_);
				}
				else if ("pickup" == secondtoken)
				{
					pickup = new Pickup;
					withinpickup = true;
				}
				else if ("exit" == secondtoken)
				{
					xit = new Xit;
					withinexit = true;
				}
				else
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
			}
			else if ("end" == firsttoken)
			{
				if ("scene" == secondtoken)
				{
					scenemanager_->add(scene, scene->name_.c_str());
				}
				else if ("pickup" == secondtoken)
				{
					scene->add_pickup(pickup);
					withinpickup = false;
				}
				else if ("exit" == secondtoken)
				{
					scene->add_exit(xit);
					withinexit = false;
				}
				else
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
			}
			else
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
		}
		// this is a single value assignment Property = Value
		else if (0x3 == tokencount)
		{
			std::string property = tokens.at(0);
			std::string op = tokens.at(1);
			std::string value = tokens.at(2);
			std::transform(property.begin(), property.end(), property.begin(), tolower);

			if ("=" != op)
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}

			if ("name" == property)
			{
				fprintf(stderr, "set scene name to \"%s\"\n", value.c_str());
				scene->set_name(value.c_str());
			}
			else if ("background" == property)
			{
				fprintf(stderr, "set scene background to \"%s\"\n", value.c_str());
				scene->set_background(value.c_str());
			}
			else if ("pickups" == property)
			{
				fprintf(stderr, "set scene pickup count to %d\n", atoi(value.c_str()));
				scene->set_total_pickups(atoi(value.c_str()));
			}
			else if ("exits" == property)
			{
				fprintf(stderr, "set scene exit count to %d\n", atoi(value.c_str()));
				scene->set_total_exits(atoi(value.c_str()));
			}
			else if ("image" == property)
			{
				//fprintf(stderr, "TODO: set scene background to \"%s\"\n", value.c_str());
				if (withinpickup)
				{
					scene->set_pickup_image(pickup, value.c_str());
				}
				else if (withinexit)
				{
					scene->set_exit_image(xit, value.c_str());
				}
				else
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
			}
			else if ("x" == property)
			{
				//fprintf(stderr, "TODO: set x to %d\n", atoi(value.c_str()));
				if (withinpickup)
				{
					pickup->set_x(atoi(value.c_str()));
				}
				else if (withinexit)
				{
					xit->set_x(atoi(value.c_str()));
				}
				else
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
			}
			else if ("y" == property)
			{
				//fprintf(stderr, "TODO: set y to %d\n", atoi(value.c_str()));
				if (withinpickup)
				{
					pickup->set_y(atoi(value.c_str()));
				}
				else if (withinexit)
				{
					xit->set_y(atoi(value.c_str()));
				}
				else
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
			}
			else
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}

		}
		// this is a multi value assignment Property = Value1 Value2 Value3 ValueN ...
		else if (tokencount > 3)
		{
			std::string property = tokens.at(0);
			std::string op = tokens.at(1);
			std::string value = tokens.at(2);
			std::transform(property.begin(), property.end(), property.begin(), tolower);

			if ("=" != op)
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}

			for (unsigned int index = 3; index < tokencount; index++)
			{
				value = value + " " + tokens.at(index);
			}

			if ("bounds" == property)
			{
				if (5 > tokencount)
				{
					fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
					return false;
				}
				fprintf(stderr,
					"set pickup image collision bounds to [%d,%d,%d,%d]\n",
					atoi(tokens.at(2).c_str()),
					atoi(tokens.at(3).c_str()),
					atoi(tokens.at(4).c_str()),
					atoi(tokens.at(5).c_str())
				);

				pickup->set_collision_bounds(
					atoi(tokens.at(2).c_str()),
					atoi(tokens.at(3).c_str()),
					atoi(tokens.at(4).c_str()),
					atoi(tokens.at(5).c_str())
				);
			}
			else if ("dest" == property)
			{
				fprintf(stderr, "set exit destination to \"%s\"\n", value.c_str());
				xit->set_destination(value.c_str());
			}
			else if ("name" == property)
			{
				fprintf(stderr, "set scene name to \"%s\"\n", value.c_str());
				scene->set_name(value.c_str());
			}
			else
			{
				fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
				return false;
			}
		}
		else
		{
			fprintf(stderr, "error in scn file: %s in line: \"%s\"\n", filename, line.c_str());
			return false;
		}
	}

	fprintf(stderr, "parsing finished\n");

	return true;
}
