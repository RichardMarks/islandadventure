// main.cpp
// Project: Allegro Game Template v6.2
// Author: Richard Marks <ccpsceo@gmail.com>
/*
  ****************************************************************************
  * Copyright (c) 2009, Richard Marks, CCPS Solutions,                       *
  * Undefined Aeon Software.                                                 *
  *                                                                          *
  * Permission is hereby granted, free of charge, to any person obtaining a  *
  * copy of this software and associated documentation files (the            *
  * "Software"), to deal in the Software without restriction, including      *
  * without limitation the rights to use, copy, modify, merge, publish,      *
  * distribute, distribute with modifications, sub-license, and/or sell      *
  * copies of the Software, and to permit persons to whom the Software is    *
  * furnished to do so, subject to the following conditions:                 *
  *                                                                          *
  * The above copyright notice and this permission notice shall be included  *
  * in all copies or substantial portions of the Software.                   *
  *                                                                          *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
  * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  *
  * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
  * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
  * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
  * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
  *                                                                          *
  * Except as contained in this notice, the name(s) of the above copyright   *
  * holders shall not be used in advertising or otherwise to promote the     *
  * sale, use or other dealings in this Software without prior written       *
  * authorization.                                                           *
  ****************************************************************************
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <allegro.h>

////////////////////////////////////////////////////////////////////////////////

// we need this for handling timing
static volatile int allegrotimerspeedcounter = 0;
static void my_allegro_timer_speed_controller()
{
	allegrotimerspeedcounter++;
}
END_OF_FUNCTION(my_allegro_timer_speed_controller)

// we need this to handle closing the window via the [X] button (you NEED this)
static volatile bool mainthreadisrunning = true;
static void my_allegro_close_button_handler()
{
	mainthreadisrunning = false;
}
END_OF_FUNCTION(my_allegro_close_button_handler)

////////////////////////////////////////////////////////////////////////////////

bool setup_game();
void update_game();
void render_game();
void shutdown_game();

////////////////////////////////////////////////////////////////////////////////

// constants for the screen resolution
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int WINDOW_COLOR_DEPTH = 16;
const bool WINDOW_USE_FULLSCREEN = false;

// the text in the window caption bar
const char* WINDOW_CAPTION = "Island Adventure - NOGDUS MGDC 10-2009 Entry -- Richard Marks <ccpsceo@gmail.com>";

// if we want to enable the mouse
const bool ENABLE_MOUSE_SUPPORT = true;

// the FPS (frames per second) we should lock to
#define FRAME_LOCK_RATE 30

// bitmap for drawing on
BITMAP* backbuffer = 0;

////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <list>
#include <map>

////////////////////////////////////////////////////////////////////////////////

void makebox(int* target, int left, int top, int right, int bottom)
{
	target[0] = left;
	target[1] = top;
	target[2] = right;
	target[3] = bottom;
}
////////////////////////////////////////////////////////////////////////////////

struct Clickable
{
	std::string name_;
	int x_;
	int y_;
	int w_;
	int h_;
	bool lmbdown_;
	int left_, top_, right_, bottom_;
	bool visible_;
	Clickable(int x, int y, int w, int h, int* bounds, bool vis) :
		x_(x), y_(y), w_(w), h_(h), lmbdown_(false), visible_(vis)
	{
		left_ = bounds[0];
		top_ = bounds[1];
		right_ = bounds[2];
		bottom_ = bounds[3];
	}
	virtual ~Clickable()
	{
	}
	virtual bool hover()
	{
		int box[]=
		{
			x_ + left_,
			y_ + top_,
			(x_ + left_) + (w_ - right_),
			(y_ + top_) + (h_ - bottom_)
		};
		if (mouse_x >= box[0] && mouse_x <= box[2] && mouse_y >= box[1] && mouse_y <= box[3])
		{
			return true;
		}
		return false;
	}
	virtual bool clicked()
	{
		if (!visible_)
		{
			return false;
		}
		if (!this->hover())
		{
			return false;
		}

		if (mouse_b & 1)
		{
			if (!lmbdown_){lmbdown_=true;}
		}
		else
		{
			if (lmbdown_){lmbdown_=false;return true;}
		}
		return false;
	}
	void make_invisible()
	{
		visible_ = false;
	}
	void make_visible()
	{
		visible_ = true;
	}
};
////////////////////////////////////////////////////////////////////////////////

struct Pickup : public Clickable
{
	BITMAP* picture_;

	Pickup(const char* name, BITMAP* source, int x, int y, int* bounds) :
		Clickable(x, y, source->w, source->h, bounds, true),
		picture_(source)
	{
		name_ = name;
	}

	virtual ~Pickup()
	{
	}

	void render(BITMAP* target)
	{
		if (!visible_)
		{
			return;
		}
		draw_sprite(target, picture_, x_, y_);
	}
};
////////////////////////////////////////////////////////////////////////////////

struct NavButton : public Clickable
{
	BITMAP* picture_;

	NavButton(BITMAP* source, int x, int y, int* bounds) :
		Clickable(x, y, source->w, source->h, bounds, true),
		picture_(source)
	{

	}

	virtual ~NavButton()
	{
	}

	void render(BITMAP* target)
	{
		if (!visible_)
		{
			return;
		}
		draw_sprite(target, picture_, x_, y_);
	}
};

////////////////////////////////////////////////////////////////////////////////

struct Resource
{
	std::string name_;
	Resource(){}
	virtual ~Resource(){}
	virtual bool ok() = 0;
};
////////////////////////////////////////////////////////////////////////////////

struct ImageResource : public Resource
{
	BITMAP* image_;
	ImageResource(){}
	virtual ~ImageResource(){if(image_){destroy_bitmap(image_);}}
	operator BITMAP*(){return image_;}
	virtual bool ok(){ return (0 != image_); }
};
////////////////////////////////////////////////////////////////////////////////
bool is_image(const char* name)
{
	std::string n = name;
	n = n.substr(n.size() - 3);
	std::transform(n.begin(), n.end(), n.begin(), tolower);
	return "bmp" == n;
}
////////////////////////////////////////////////////////////////////////////////

struct ResourceManager
{
	std::vector <Resource*> resources_;
	std::map <std::string, unsigned int> table_;
	ResourceManager()
	{
	}
	~ResourceManager()
	{
		std::vector <Resource*>::iterator iter = resources_.begin();
		while(iter != resources_.end())
		{
			if (*iter)
			{
				fprintf(stderr,"releasing resource %s\n", (*iter)->name_.c_str());
				delete *iter;
				resources_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		table_.clear();
	}
	void load(const char* resname)
	{
		unsigned int id = resources_.size();

		if (is_image(resname))
		{
			ImageResource* resource = new ImageResource;
			resource->image_ = load_bitmap(resname, 0);
			if (resource)
			{
				resource->name_ = resname;
				resources_.push_back(resource);
				fprintf(stderr, "loaded %s\n", resname);
			}
			else
			{
				fprintf(stderr, "error cannot load %s\n", resname);
				return;
			}
		}
		else
		{
			fprintf(stderr, "error loading %s - not an image\n", resname);
			return;
		}

		table_[resname] = id;
	}
	Resource* get(const char* resname)
	{
		// very dangerous if the resource name is not right
		return resources_[table_[resname]];
	}
};
////////////////////////////////////////////////////////////////////////////////

struct Scene
{
	int found_;
	ResourceManager* resources_;
	BITMAP* background_;
	std::vector <Pickup*> pickups_;
	std::vector <NavButton*> exits_;
	std::string name_;
	Scene() : found_(0)
	{
	}

	void set_background(const char* resname)
	{
		ImageResource* res = (ImageResource*) resources_->get(resname);
		if (res)
		{
			if (res->ok())
			{
				background_ = *res;
			}
		}
	}
	void add_pickup(const char* name, int x, int y, int* box)
	{
		ImageResource* res = (ImageResource*) resources_->get(name);
		if (res)
		{
			if (res->ok())
			{
				pickups_.push_back(new Pickup(name, *res, 0, 0, box));
			}
		}
	}
	void add_exit(NavButton* n)
	{
		exits_.push_back(n);
	}

	void add_exit_north(int x, int y)
	{
		ImageResource* res = (ImageResource*) resources_->get("north.bmp");
		if (res)
		{
			if (res->ok())
			{
				int box[4];makebox(box, 0, 0, 0, 0);
				this->add_exit(new NavButton(*res, x, y, box));
			}
		}
	}

	void add_exit_south(int x, int y)
	{
		ImageResource* res = (ImageResource*) resources_->get("south.bmp");
		if (res)
		{
			if (res->ok())
			{
				int box[4];makebox(box, 0, 0, 0, 0);
				this->add_exit(new NavButton(*res, x, y, box));
			}
		}
	}

	void add_exit_east(int x, int y)
	{
		ImageResource* res = (ImageResource*) resources_->get("east.bmp");
		if (res)
		{
			if (res->ok())
			{
				int box[4];makebox(box, 0, 0, 0, 0);
				this->add_exit(new NavButton(*res, x, y, box));
			}
		}
	}

	void add_exit_west(int x, int y)
	{
		ImageResource* res = (ImageResource*) resources_->get("west.bmp");
		if (res)
		{
			if (res->ok())
			{
				int box[4];makebox(box, 0, 0, 0, 0);
				this->add_exit(new NavButton(*res, x, y, box));
			}
		}
	}


	void releasepickups()
	{
		std::vector <Pickup*>::iterator iter = pickups_.begin();
		while(iter != pickups_.end())
		{
			if (*iter)
			{
				delete *iter;
				pickups_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	void releaseexits()
	{
		std::vector <NavButton*>::iterator iter = exits_.begin();
		while(iter != exits_.end())
		{
			if (*iter)
			{
				delete *iter;
				exits_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	virtual ~Scene()
	{
		this->releasepickups();
		this->releaseexits();
	}

	virtual void setup() = 0;
	virtual bool update() = 0;
	virtual void render(BITMAP* target) = 0;
};
////////////////////////////////////////////////////////////////////////////////

struct SceneOne_Start : public Scene
{
	SceneOne_Start()
	{
		// NEVER do init here
	}

	~SceneOne_Start()
	{
	}

	virtual void setup()
	{
		int bounds[4] = {0, 0, 0, 0};

		this->set_background("scene1.bmp");

		makebox(bounds, 0, 0, 0, 0);
		this->add_pickup("rope.bmp", 0, 0, bounds);

		this->add_exit_west(200, SCREEN_H / 2);
		this->add_exit_east(SCREEN_W - 200, SCREEN_H / 2);
	}

	virtual bool update()
	{
		std::vector <Pickup*>::iterator iter;
		for (iter = pickups_.begin(); iter != pickups_.end(); iter++)
		{
			if ((*iter)->clicked())
			{
				fprintf(stderr, "found %s\n", (*iter)->name_.c_str());
				(*iter)->make_invisible();
				found_++;
			}
		}

		return false;
	}

	virtual void render(BITMAP* target)
	{
		blit(background_, target, 0, 0, 0, 0, target->w, target->h);

		std::vector <Pickup*>::iterator iter;
		for (iter = pickups_.begin(); iter != pickups_.end(); iter++)
		{
			(*iter)->render(target);
		}
	}
};
struct SceneTwo_BeachFront : public Scene {};
struct SceneThree_Death : public Scene {};
struct SceneFour_TempleBase : public Scene {};
struct SceneFive_Death : public Scene {};
struct SceneSix_Matches : public Scene {};
struct SceneSeven_Candle : public Scene {};
struct SceneEight_Treeline : public Scene {};
struct SceneNine_Death : public Scene {};
struct SceneTen_Win : public Scene {};
////////////////////////////////////////////////////////////////////////////////

struct SceneManager
{
	ResourceManager* resources_;
	std::vector <Scene*> scenes_;
	std::map <std::string, unsigned int> table_;
	SceneManager()
	{
	}
	~SceneManager()
	{
		std::vector <Scene*>::iterator iter = scenes_.begin();
		while(iter != scenes_.end())
		{
			if (*iter)
			{
				fprintf(stderr,"destroying scene %s\n", (*iter)->name_.c_str());
				delete *iter;
				scenes_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		table_.clear();
	}
	void setup(ResourceManager* res)
	{
		fprintf(stderr,"setting up scene manager\n");
		resources_ = res;
		if (!res)
		{
			fprintf(stderr,"caution..resource manager pointer failure!\n");
		}
	}
	void add(Scene* scene, const char* name)
	{
		fprintf(stderr,"adding scene %s\n", name);
		scene->resources_ = resources_;
		scene->name_ = name;
		unsigned int id = scenes_.size();
		scene->setup();
		scenes_.push_back(scene);
		table_[name] = id;
	}
	Scene* get(const char* name)
	{
		// very dangerous if the name is not right
		return scenes_[table_[name]];
	}
};

#define DEBUG_ROOM_SCENE 1984
struct Game
{
	ResourceManager resmanager_;
	SceneManager scenemanager_;

	int total_; // how many objects you have found total
	int found_; // how many objects you have found in this scene
	int scene_; // which scene you are on

	Game()
	{
		resmanager_.load("scene1.bmp");
		resmanager_.load("scene2.bmp");
		resmanager_.load("scene3.bmp");
		resmanager_.load("scene4.bmp");
		resmanager_.load("scene5.bmp");
		resmanager_.load("scene6.bmp");
		resmanager_.load("scene7.bmp");
		resmanager_.load("scene8.bmp");
		resmanager_.load("scene9.bmp");
		resmanager_.load("rope.bmp");
		resmanager_.load("candle.bmp");
		resmanager_.load("matches.bmp");
		resmanager_.load("north.bmp");
		resmanager_.load("south.bmp");
		resmanager_.load("east.bmp");
		resmanager_.load("west.bmp");

		scenemanager_.setup(&resmanager_);

		scenemanager_.add(new SceneOne_Start, "West Beach");

		total_ = 0;
		found_ = 0;
		scene_ = 0;
	}

	~Game()
	{
	}

	void update()
	{
		switch(scene_)
		{
			case 0:
			{
				scenemanager_.get("West Beach")->update();
			} break;
			case 1:
			{
			} break;
			case 2: {} break;
			case 3: {} break;
			case 4: {} break;
			case 5: {} break;
			case 6: {} break;
			case 7: {} break;
			case 8: {} break;
			case DEBUG_ROOM_SCENE:
			{

			} break;
			default: break;
		}
	}

	void render(BITMAP* target)
	{
		switch(scene_)
		{
			case 0:
			{
				Scene* s = scenemanager_.get("West Beach");
				s->render(target);
				found_ = s->found_;
			} break;
			case 1: {} break;
			case 2: {} break;
			case 3: {} break;
			case 4: {} break;
			case 5: {} break;
			case 6: {} break;
			case 7: {} break;
			case 8: {} break;
			case DEBUG_ROOM_SCENE:
			{
			} break;
		}

		textprintf_ex(target,font,0,0,makecol(0,0,0),-1,"Found %d items. Scene %d",found_,1+scene_);
		textprintf_ex(target,font,1,1,makecol(0,0,0),-1,"Found %d items. Scene %d",found_,1+scene_);
		textprintf_ex(target,font,2,2,makecol(255,255,255),-1,"Found %d items. Scene %d",found_,1+scene_);
	}
};
Game* game = 0;
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	// init allegro and add keyboard and optional mouse support
	allegro_init();
	install_timer();
	install_keyboard();
	if (ENABLE_MOUSE_SUPPORT)
	{
		install_mouse();
	}

	// set the video mode
	set_color_depth(WINDOW_COLOR_DEPTH);
	set_gfx_mode(
		(WINDOW_USE_FULLSCREEN) ?
			GFX_AUTODETECT_FULLSCREEN :
			GFX_AUTODETECT_WINDOWED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
	// set the window caption text
	set_window_title(WINDOW_CAPTION);

	// create the back buffer bitmap
	backbuffer = create_bitmap(SCREEN_W, SCREEN_H);

	// seed the random number generator
	srand(time(0));

	// lock the static functions and variables we need for handling timing and closing the window via the [X] button
	LOCK_FUNCTION(my_allegro_close_button_handler);
	LOCK_FUNCTION(my_allegro_timer_speed_controller);
	LOCK_VARIABLE(allegrotimerspeedcounter);

	// set the callback function for the close-button to our global handler function
	set_close_button_callback(my_allegro_close_button_handler);

	// set our FPS lock timing global function
	install_int_ex(my_allegro_timer_speed_controller, BPS_TO_TIMER(FRAME_LOCK_RATE));

	// setup the game
	if (!setup_game())
	{
		fprintf(stderr, "The game initialization has failed. Cannot continue!\n");
		exit(1);
	}

	// main loop
	bool gameover = false;
	while(!gameover)
	{
		// if our global is ever false
		if (!mainthreadisrunning)
		{
			gameover = true;
		}

		// we only draw when the FPS should be locked
		if (allegrotimerspeedcounter > 0)
		{
			// we only update during our FPS lock time
			while (allegrotimerspeedcounter > 0)
			{
				// ensure the keyboard data is current
				if (keyboard_needs_poll())
				{
					poll_keyboard();
				}

				// ensure the mosue data is current
				if (ENABLE_MOUSE_SUPPORT)
				{
					if (mouse_needs_poll())
					{
						poll_mouse();
					}
				}

				// update
				update_game();

				// decrement the global timing var so that we can leave the update loop!
				allegrotimerspeedcounter--;
			}

			// start rendering the scene
			render_game();

			if (ENABLE_MOUSE_SUPPORT)
			{
				show_mouse(backbuffer);
			}

			// make it all visible
			blit(backbuffer, screen, 0, 0, 0, 0, backbuffer->w, backbuffer->h);
		}
		else
		{
			// a little rest to keep CPU usage down ^-^
			rest(1);
		}
	}

	// shutdown the game
	shutdown_game();

	// clean up the back buffer
	if (backbuffer)
	{
		if (ENABLE_MOUSE_SUPPORT)
		{
			show_mouse(0);
		}
		destroy_bitmap(backbuffer);
	}

	return 0;
}
END_OF_MAIN();

////////////////////////////////////////////////////////////////////////////////

bool setup_game()
{
	// load game resources and stuff here
	// return false if something failed to load or init
	game = new Game;

	// return true to let the main() function know that we're good to go
	return true;
}

////////////////////////////////////////////////////////////////////////////////

void update_game()
{
	// TODO: *** update stuff here
	game->update();

	// pressing F12 will end the main loop
	if (key[KEY_F12])
	{
		mainthreadisrunning = false;
	}
}

////////////////////////////////////////////////////////////////////////////////

void render_game()
{
	clear_bitmap(backbuffer);

	// TODO: *** draw stuff here
	game->render(backbuffer);
}

////////////////////////////////////////////////////////////////////////////////

void shutdown_game()
{
	// unallocate anything you allocated for your game here
	delete game;
}

