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

#include "support.h"
#include "class_pickup.h"
#include "class_xit.h"
#include "class_resource_manager.h"
#include "class_scene.h"
#include "class_scene_manager.h"
#include "class_game.h"

Game* game = 0;

////////////////////////////////////////////////////////////////////////////////

// we need this for handling timing
static volatile int allegrotimerspeedcounter = 0;
static void my_allegro_timer_speed_controller()
{
	allegrotimerspeedcounter++;
}
END_OF_FUNCTION(my_allegro_timer_speed_controller)

// we need this to handle closing the window via the [X] button (you NEED this)

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

