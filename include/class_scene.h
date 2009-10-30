#ifndef __CLASS_SCENE_H__
#define __CLASS_SCENE_H__

#include <vector>
#include <string>

struct SceneNavigator;
struct ResourceManager;
struct Pickup;
struct Xit;

struct Scene
{
	ID id_;
	int found_;
	ResourceManager* resources_;
	SceneNavigator* navigator_;
	BITMAP* background_;
	std::vector <Pickup*> pickups_;
	std::vector <Xit*> exits_;
	std::string name_;

	int totalpickups_;
	int totalexits_;

	Scene();

	void set_resource_manager(ResourceManager* resmgr);

	void set_scene_navigator(SceneNavigator* navigator);

	void set_background(const char* resname);

	void set_pickup_image(Pickup* pickup, const char* resname);

	void set_exit_image(Xit* xit, const char* resname);

	void set_pickup_name(Pickup* pickup, const char* name);

	void set_exit_name(Xit* xit, const char* name);

	void set_name(const char* name);

	void set_total_pickups(int amount);

	void set_total_exits(int amount);

	void add_pickup(Pickup* p);
	void add_pickup(const char* name, int x, int y, int* box);
	void add_exit(Xit* n);

	void add_exit_north(int x, int y);

	void add_exit_south(int x, int y);

	void add_exit_east(int x, int y);

	void add_exit_west(int x, int y);

	void releasepickups();

	void releaseexits();

	~Scene();

	void update_pickups();

	std::string update_exits();

	bool update();

	void render_pickups(BITMAP* target);

	void render_exits(BITMAP* target);

	void render(BITMAP* target);
};


#endif
