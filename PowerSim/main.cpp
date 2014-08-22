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

	if(atoi(Settings::GetSetting("create_screen").c_str()))
	{
		AllegroEngine::InitializeScreen(game.context->screen_width,game.context->screen_height);
		AllegroEngine::RenameSceen(game.context->world_name);
	}
	else
	{
		AllegroEngine::InitializeBitmap(game.context->screen_width,game.context->screen_height);
	}

	game.Update();

	AllegroEngine::FreeMemory();

	return 0;
}