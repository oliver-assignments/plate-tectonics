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

	game.Update();

	al_rest(3.0);

	AllegroEngine::FreeMemory();

	return 0;
}