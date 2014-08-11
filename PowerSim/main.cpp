#include "Game.h"
#include "utility.h"

#include "allegro_engine.h"
#include "settings.h"

int main()
{
	srand(time(NULL));

	Settings::LoadConfigFile();

	AllegroEngine::InitializeAllegro();

	Game game;
	
	//Loading the game
	if(atoi(Settings::GetSetting("create_new_game").c_str()))
		game.CreateWorld();
	else
		//game.LoadWorld();

	AllegroEngine::InitializeScreen(game.context->screen_width,game.context->screen_height);
	
	while(true)
	{
		game.Update();
	}

	AllegroEngine::FreeMemory();

	return 0;
}