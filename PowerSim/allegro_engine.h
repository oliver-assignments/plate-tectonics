#ifndef __WS_ALLEGRO_ENGINE__
#define __WS_ALLEGRO_ENGINE__

#include "utility.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_primitives.h>


class AllegroEngine
{
public:
	static void InitializeAllegro()
	{
		al_init(); //allegro-5.0.10-monolith-md-debug.lib

		//Anti Aliasing
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

		//Initializing Addons
		al_init_image_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_install_keyboard();
		al_install_audio();
		al_init_acodec_addon();
		al_init_primitives_addon();

		al_reserve_samples(10);
	};

	//Creates the screen
	static void InitializeScreen(int myScreenWidth, int myScreenHeight)
	{
		//Creating screen
		ALLEGRO_DISPLAY* display = al_create_display(myScreenWidth, myScreenWidth);
		al_set_window_position(display,0,0);
		al_set_window_title(display,"World Simulation");//CreateName(7).c_str());

		main_display = display;
	};


	//Altered version of allegro's screenshot functionality
	int FlushScreenshot(const char *destination_path, const char *myWorldName,int myCurrentYear,int myCurrentMonth, int myCurrentDay)
	{
		ALLEGRO_PATH *path;
		char *filename, *filename_wp;
		const char *path_cstr;

		//Testing the destination given
		if(!destination_path)
			path = al_get_standard_path(ALLEGRO_USER_DOCUMENTS_PATH);
		else
			path = al_create_path_for_directory(destination_path);

		if(!path)
			return -1;//Where we want it is bad

		if(!myWorldName) {
			if( !(myWorldName = al_get_app_name()) ) {
				al_destroy_path(path);
				return -2;//We cant use app name or the given name
			}
		}

		//Length of gamename + length of ":YYYY-MM-DD" + NULL terminator
		if ( !(filename_wp = filename = (char *)malloc(strlen(myWorldName)+ 1 + 8 + 1 + 1 + 1)) ) {
			al_destroy_path(path);
			return -4;//We don't have enough space?
		}

		//Creating the file name
		std::string date = myCurrentYear+"/"+myCurrentMonth;
		date += "/" + myCurrentDay;

		//strcpy(filename, myWorldName);

		for(; *filename != '.' && *filename != 0; ++filename);
		*filename = ':';

		//strcpy(filename++, date.c_str());


		//Setting up file specificiations
		al_set_path_filename(path, filename);//Where are we writing this
		al_set_path_extension(path, ".png");//We are creating a png
		path_cstr = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

		//We've already printed out this file
		if (al_filename_exists(path_cstr))
		{
			//Saving the bitmap
			al_save_bitmap(path_cstr, al_get_target_bitmap());
			return 0;//It worked
		}
		free(filename);
		al_destroy_path(path);

		return -6;//We never placed a file
	}

private:
	static ALLEGRO_DISPLAY* main_display;
};


#endif