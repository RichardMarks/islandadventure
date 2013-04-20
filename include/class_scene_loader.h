#ifndef __CLASS_SCENE_LOADER_H__
#define __CLASS_SCENE_LOADER_H__

#include <vector>
#include <string>

struct SceneManager;
struct ResourceManager;
struct SceneNavigator;
struct Scene;
struct Pickup;
struct Xit;

struct SceneLoader
{
	SceneManager* scenemanager_;
	ResourceManager* resourcemanager_;
	SceneNavigator* scenenavigator_;

	Scene* scene_;
	Pickup* pickup_;
	Xit* xit_;
	bool withinpickup_;
	bool withinexit_;

	SceneLoader(SceneManager* scenemanager, ResourceManager* resourcemanager, SceneNavigator* scenenavigator);
	~SceneLoader();

	bool load(const char* filename);

	bool parse_tokens(const char* filename, std::string& line, std::vector <std::string>& tokens);

	bool parse_blocks(const char* filename, std::string& line, std::string& firsttoken, std::string& secondtoken);

	bool parse_property(const char* filename, std::string& line, std::string& property, std::string& value);
};

#endif
