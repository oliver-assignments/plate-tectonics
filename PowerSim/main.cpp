#include "utility.h"
#include <allegro5/allegro.h>

class Settings;
class AllegroEngine;

#include "Game.h"

int main()
{
	srand(time(NULL));

	Settings::LoadConfigFile();

	AllegroEngine::InitializeAllegro();

	Game game;

	game.ProcessSettings();

	//Loading the game
	if(atoi(Settings::GetSetting("create_new_game").c_str()))
	{
		game.CreateWorld();
	}
	else
	{
		//game.LoadWorld();
	}

	AllegroEngine::InitializeScreen(game.context->screen_width,game.context->screen_height);

	game.UpdateDeepestWater();
	game.Draw(TERRAIN);
	

	AllegroEngine::FlushScreenshot(Settings::GetSetting("picture_output_folder").c_str(),game.context->world_name,10,10,1000);

	al_flip_display();

	al_rest(3.0);

	AllegroEngine::FreeMemory();

	/*if (!al_init()) {return 1;}
	al_init_image_addon();

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
	ALLEGRO_DISPLAY* display = al_create_display(200,200);

	ALLEGRO_BITMAP* testbmp = al_load_bitmap("test.png");

	al_save_bitmap("testsave.png" , testbmp);

	al_set_target_backbuffer(display);
	al_clear_to_color(al_map_rgb(0,255,0));
	al_draw_bitmap(testbmp , 0 , 0 , 0);

	al_save_bitmap("testbackbuffersave.png" , al_get_backbuffer(display));

	al_flip_display();

	al_rest(3.0);*/

	return 0;
}