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
#include <stdio.h>

class AllegroEngine
{
public:
	static void InitializeAllegro();

	static void InitializeFonts();

	//Creates the screen
	static void InitializeScreen(int myScreenWidth, int myScreenHeight);

	//Altered version of allegro's screenshot functionality
	static int FlushScreenshot(const char *destination_path, std::string myWorldName,
		int myCurrentYear,int myCurrentMonth, int myCurrentDay);

	//This make be better for 
	static void DrawTextC(std::string myText,int myXDestination, int myYDestination, ALLEGRO_FONT* myFont, std::string myAlignment);

	static void FreeMemory();

	static ALLEGRO_FONT* arial24;
	static ALLEGRO_FONT* arial16;
	static ALLEGRO_FONT* arial12;
	static ALLEGRO_FONT* arial8;

	

private:
	static ALLEGRO_DISPLAY* main_display;
	static int screen_width;
	static int screen_height;

};


#endif