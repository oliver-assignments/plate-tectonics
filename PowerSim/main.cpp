#include "Game.h"
#include "utility.h"

#include "allegro_engine.h"
#include "settings.h"

int main()
{
	SettingsManager::LoadConfigFile();

	AllegroEngine::InitializeAllegro();
	AllegroEngine::InitializeScreen();
	Game game;

	game->CreateWorld();

	bool done= false;
	float FPS=0;
	float total_frames=0;
	float previous_tick_timestamp =0;
	float elapsed_time=0;
	float old_time=0;

	float dt = .05;
	float accumulator = 0.0;

	while(true)
	{
		game->Update();
	}

	return 0;
}