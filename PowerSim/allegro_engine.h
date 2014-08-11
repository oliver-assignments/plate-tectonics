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

		InitializeFonts();
	};

	static void InitializeFonts()
	{
		arial24 = al_load_font("arial.ttf",24,0);
		arial16 = al_load_font("arial.ttf",16,0);
		arial12 = al_load_font("arial.ttf",12,0);
		arial8 = al_load_font("arial.ttf",8,0);
	};

	
	//Creates the screen
	static void InitializeScreen(int myScreenWidth, int myScreenHeight);

	//Altered version of allegro's screenshot functionality
	static int FlushScreenshot(const char *destination_path, const char *myWorldName,
						int myCurrentYear,int myCurrentMonth, int myCurrentDay);

	//This make be better for 
	static void DrawTextC(std::string myText,int myXDestination, int myYDestination, ALLEGRO_FONT* myFont, std::string myAlignment);

	static void FreeMemory()
	{
		 al_destroy_display(main_display);
		 al_destroy_font(arial24);
		 al_destroy_font(arial16);
		 al_destroy_font(arial12);
		 al_destroy_font(arial8);
	};

	static int screen_width;
	static int screen_height;

	static ALLEGRO_FONT* arial24;
	static ALLEGRO_FONT* arial16;
	static ALLEGRO_FONT* arial12;
	static ALLEGRO_FONT* arial8;

private:
	static ALLEGRO_DISPLAY* main_display;

	
};


#endif