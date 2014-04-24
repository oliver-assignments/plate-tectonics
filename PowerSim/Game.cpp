#include "Game.h"

#include <iostream>

Game::Game()
{
	Initialize();
};

void Game::Initialize()
{
	screen_width = 1350;
	screen_height = 690;

	done= false;
	FPS=0;
	total_frames=0;
	previous_tick_timestamp =0;
	elapsed_time=0;
	old_time=0;

	dt = .05;
	accumulator = 0.0;

	InitializeAllegro();
	LoadContent();

	InitializeGame();

	Update();

};

void Game::InitializeAllegro()
{
	al_init(); //allegro-5.0.10-monolith-md-debug.lib

	//Anti Aliasing
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	//Creating screen
	display = al_create_display(screen_width,screen_height);
	al_set_window_position(display,0,0);
	al_set_window_title(display,"Allegro Template");

	//Initializing Addons
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(10);
};

void Game::InitializeGame()
{
	ui_state = FOREIGN;

	//Setuping up our values
	hours_in_day =24;
	days_in_year=365;

	current_hour=0;
	current_day=0;
	current_year=0;

	power_highest_historical=0;
	power_highest_current=0;
	power_context = CURRENT;

	strength_highest_historical =0;
	strength_highest_current=0;
	strength_context= CURRENT;

	intelligence_highest_historical=0;
	intelligence_highest_current=0;
	intelligence_context = CURRENT;

	hunger_death_level = 100;

	foreign_max=100;

	generation_youngest=1;

	//Creating people
	CreatePeople(10,25,75);

	generation_youngest=1;
	power_highest_person = people[0];
	strength_highest_person = people[0];
	intelligence_highest_person = people[0];
};

void Game::CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius)
{
	srand(time(NULL));
	for(int c = 0 ; c<myNumberClusters;c++)
	{
		int cluster_origin_x = 100+(rand()%(screen_width-200));
		int cluster_origin_y = 100+(rand()%(screen_height-200));

		int foreign_origin_x = cos(c*(2*3.14)/myNumberClusters) * myForeignRadius;
		int foreign_origin_y = sin(c*(2*3.14)/myNumberClusters) * myForeignRadius;

		for(int p = 0 ; p < myPeoplePerCluster;p++)
		{
			Person* person = new Person (10+(1+rand()%50),3+(1+rand()%50),1,-10+foreign_origin_x+(rand()%20),-10+foreign_origin_y+(rand()%20),cluster_origin_x+(-50+(rand()%100)),cluster_origin_y+(-50+(rand()%100)));
			people.push_back(person);
		}

	}
};

void Game::LoadContent()
{
	DefineColors();
};

void Game::DefineColors()
{
	color_power[0] = 255; color_power[1] = 0; color_power[2] = 0;
	color_hunger[0] = 139; color_hunger[1] = 69; color_hunger[2] = 19;

	color_strength[0] = 255; color_strength[1] = 165; color_strength[2] = 0;
	color_intelligence[0] = 100; color_intelligence[1] = 149; color_intelligence[2] = 237;

	color_foreign_north[0] = 255; color_foreign_north[1] = 255; color_foreign_north[2] = 0;
	color_foreign_east[0] = 204; color_foreign_east[1] = 0; color_foreign_east[2] = 0;
	color_foreign_south[0] = 51; color_foreign_south[1] = 51; color_foreign_south[2] = 153;
	color_foreign_west[0] = 51; color_foreign_west[1] = 153; color_foreign_west[2] = 0;

	color_generation[0] = 192; color_generation[1] = 192; color_generation[2] = 192;
};

void Game::Update()
{
	while(true)
	{
		//Delta time handling
		double current_timestamp = al_get_time();
		double seconds_since_last_tick = current_timestamp - previous_tick_timestamp;
		if(seconds_since_last_tick > .25)
		{
			seconds_since_last_tick = 0.25;
		}
		previous_tick_timestamp = current_timestamp;
		accumulator+=seconds_since_last_tick;

		//Do this before drawing
		while ( accumulator >= dt )
		{
			TakeInput();

			accumulator -= dt;
		}

		Draw();
	}
	FreeMemory();
};

void Game::TakeInput()
{
	al_get_keyboard_state(&new_keyboard_state);

	//ESCAPE
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_ESCAPE))
	{
		done = true;
	}

	//WASD
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_W))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_D))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_S))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_A))
	{

	}

	//ARROW KEYS
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_UP))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_UP))
		{

		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_RIGHT))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_RIGHT))
		{

		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_DOWN))
	{

	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_LEFT))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_LEFT))
		{

		}
	}

	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_P))
	{
		ui_state = POWER;
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_I))
	{
		ui_state = INTELLIGENCE;
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_S))
	{
		ui_state = STRENGTH;
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_H))
	{
		ui_state = HUNGER;
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_F))
	{
		ui_state = FOREIGN;
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_G))
	{
		ui_state = GENERATION;
	}

	old_keyboard_state = new_keyboard_state;
};

void Game::RunTime()
{
	while(true)
	{
		{
			std::lock_guard<std::mutex> lck(mtx);
			if(done) break;
		}
		//Do all your hourly logic here
		ProcessPeople();

		current_hour++;

		if(current_hour>hours_in_day)
		{
			current_hour=0;
			current_day++;

			//Do all your daily logic here
			Draw();

			if(current_day>days_in_year)
			{
				//Do all your yearly logic here

				current_day = 0;
				current_year++;
			}
		}
	}
}

void Game::ProcessPeople()
{
	int starting_index = rand()%people.size();
	
	for(std::vector<Person*>::size_type i = starting_index; i < people.size(); i++) 
	{
		ProcessPersonAI();

		if(i==people.size()-1)
		{
			for (int o = 0; o < starting_index; o++)
			{
				ProcessPersonAI();
			}
		}
	}
};

void Game::ProcessPersonAI()
{

};

void Game::Draw()
{
	DrawPeople();

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
};

void Game::DrawPeople()
{
	//Maintianing highest of power strength and intel
	if(power_highest_person->dead)
		power_highest_person = nullptr;
	if(strength_highest_person->dead)
		strength_highest_person = nullptr;
	if(intelligence_highest_person->dead)
		intelligence_highest_person=nullptr;

	for(std::vector<Person*>::size_type i = 0; i != people.size(); i++) 
	{
		if(!people[i]->dead)
		{
			if(power_highest_person == nullptr)
				power_highest_person = people[i];
			if(strength_highest_person== nullptr)
				strength_highest_person  = people[i];
			if(intelligence_highest_person== nullptr)
				intelligence_highest_person = people[i];

			//Power
			if(power_highest_historical<people[i]->power)
			{
				power_highest_historical = people[i]->power;
			}
			if(power_highest_current<people[i]->power)
			{
				power_highest_current = people[i]->power;
				power_highest_person = people[i];
			}
			//Strength
			if(strength_highest_historical<people[i]->strength)
			{
				strength_highest_historical = people[i]->strength;
			}
			if(strength_highest_current<people[i]->strength)
			{
				strength_highest_current = people[i]->strength;
				strength_highest_person = people[i];
			}
			//Intelligence
			if(intelligence_highest_historical<people[i]->intelligence)
			{
				intelligence_highest_historical = people[i]->intelligence;
			}
			if(intelligence_highest_current<people[i]->intelligence)
			{
				intelligence_highest_current = people[i]->intelligence;
				intelligence_highest_person = people[i];
			}
		}
	}

	//Drawing alle
	for(std::vector<Person*>::size_type i = 0; i != people.size(); i++) 
	{
		if(people[i]->dead == false){
			ALLEGRO_COLOR person_color;
			double color [3]; 
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;

			mtx.lock();
			if(ui_state == POWER)
			{
				if(power_context == HISTORY)
				{
					double ratio =(double) people[i]->power/power_highest_historical;
					color [0]= color_power[0]*(ratio);
				}
				else //CURRENT
				{
					color[0] = color_power[0]*((double)people[i]->power/power_highest_current);
				}
			}
			else if(ui_state == HUNGER)
			{
				color [0] =color_hunger[0]*(double)people[i]->hunger/hunger_death_level;
				color [1] =color_hunger[1]*(double)people[i]->hunger/hunger_death_level;
				color [2] =color_hunger[2]*(double)people[i]->hunger/hunger_death_level;
			}
			else if(ui_state == STRENGTH)
			{
				if(strength_context == HISTORY)
				{
					color [0] =color_strength[0]*(double)people[i]->strength/strength_highest_historical;
					color [1] =color_strength[1]*(double)people[i]->strength/strength_highest_historical;
					color [2] =color_strength[2]*(double)people[i]->strength/strength_highest_historical;
				}
				else//CURRENT
				{
					color [0] =color_strength[0]*(double)people[i]->strength/strength_highest_current;
					color [1] =color_strength[1]*(double)people[i]->strength/strength_highest_current;
					color [2] =color_strength[2]*(double)people[i]->strength/strength_highest_current;
				}
			}
			else if(ui_state == INTELLIGENCE)
			{
				if(intelligence_context == HISTORY)
				{
					color [0] =color_intelligence[0]*(double)people[i]->intelligence/intelligence_highest_historical;
					color [1] =color_intelligence[1]*(double)people[i]->intelligence/intelligence_highest_historical;
					color [2] =color_intelligence[2]*(double)people[i]->intelligence/intelligence_highest_historical;
				}
				else//CURRENT
				{
					color [0] =color_intelligence[0]*(double)people[i]->intelligence/intelligence_highest_current;
					color [1] =color_intelligence[1]*(double)people[i]->intelligence/intelligence_highest_current;
					color [2] =color_intelligence[2]*(double)people[i]->intelligence/intelligence_highest_current;
				}
			}
			else if(ui_state == FOREIGN)
			{
				if(people[i]->foreign_x>=0 && people[i]->foreign_y>=0)
				{
					//first quadrant

					color [0] =((double)people[i]->foreign_x/foreign_max * color_foreign_east[0])+((double)people[i]->foreign_y/foreign_max * color_foreign_north[0]);
					color [1] =((double)people[i]->foreign_x/foreign_max * color_foreign_east[1])+((double)people[i]->foreign_y/foreign_max * color_foreign_north[1]);
					color [2] =((double)people[i]->foreign_x/foreign_max * color_foreign_east[2])+((double)people[i]->foreign_y/foreign_max * color_foreign_north[2]);
				}
				else if(people[i]->foreign_x<=0 && people[i]->foreign_y>=0)
				{
					//second quad
					color [0] =((double)people[i]->foreign_x/-foreign_max * color_foreign_west[0])+((double)people[i]->foreign_y/foreign_max * color_foreign_north[0]);
					color [1] =((double)people[i]->foreign_x/-foreign_max * color_foreign_west[1])+((double)people[i]->foreign_y/foreign_max * color_foreign_north[1]);
					color [2] =((double)people[i]->foreign_x/-foreign_max * color_foreign_west[2])+((double)people[i]->foreign_y/foreign_max * color_foreign_north[2]);
				}
				else if(people[i]->foreign_x<=0 && people[i]->foreign_y<=0)
				{
					//third quad
					color [0] =((double)people[i]->foreign_x/-foreign_max * color_foreign_west[0])+((double)people[i]->foreign_y/-foreign_max * color_foreign_south[0]);
					color [1] =((double)people[i]->foreign_x/-foreign_max * color_foreign_west[1])+((double)people[i]->foreign_y/-foreign_max * color_foreign_south[1]);
					color [2] =((double)people[i]->foreign_x/-foreign_max * color_foreign_west[2])+((double)people[i]->foreign_y/-foreign_max * color_foreign_south[2]);
				}
				else
				{
					//fourth quadrant
					color [0] =((double)people[i]->foreign_x/foreign_max * color_foreign_east[0])+((double)people[i]->foreign_y/-foreign_max * color_foreign_south[0]);
					color [1] =((double)people[i]->foreign_x/foreign_max * color_foreign_east[1])+((double)people[i]->foreign_y/-foreign_max * color_foreign_south[1]);
					color [2] =((double)people[i]->foreign_x/foreign_max * color_foreign_east[2])+((double)people[i]->foreign_y/-foreign_max * color_foreign_south[2]);
				}
			}
			else //Generation
			{
				color [0] =((double)people[i]->generation/ generation_youngest * color_generation[0]);
				color [1] =((double)people[i]->generation/ generation_youngest * color_generation[1]);
				color [2] =((double)people[i]->generation/ generation_youngest * color_generation[2]);
			}

			mtx.unlock();

			//if(ui_state == POWER)
			//{
			//	if(power_context = HISTORY)
			//	{
			//		person_color = al_map_rgb(color_power.r*(people[i]->power/power_highest_historical),0,0);
			//	}
			//	else//CURRENT
			//	{
			//		person_color = al_map_rgb(color_power.r*(people[i]->power/power_highest_current),0,0);
			//	}
			//}
			//else if(ui_state == HUNGER)
			//{
			//	person_color = al_map_rgb(
			//		color_hunger.r*(people[i]->hunger/hunger_death_level * color_hunger.r),
			//		color_hunger.g*(people[i]->hunger/hunger_death_level * color_hunger.g),
			//		color_hunger.b*(people[i]->hunger/hunger_death_level * color_hunger.b));
			//}
			//else if(ui_state == STRENGTH)
			//{
			//	if(strength_context = HISTORY)
			//	{
			//		person_color = al_map_rgb(
			//			color_strength.r*(people[i]->strength/strength_highest_historical * color_strength.r),
			//			color_strength.g*(people[i]->strength/strength_highest_historical * color_strength.g),
			//			color_strength.b*(people[i]->strength/strength_highest_historical * color_strength.b));
			//	}
			//	else//CURRENT
			//	{
			//		person_color = al_map_rgb(
			//			color_strength.r*(people[i]->strength/strength_highest_current * color_strength.r),
			//			color_strength.g*(people[i]->strength/strength_highest_current * color_strength.g),
			//			color_strength.b*(people[i]->strength/strength_highest_current * color_strength.b));
			//	}
			//}
			//else if(ui_state == INTELLIGENCE)
			//{
			//	if(intelligence_context = HISTORY)
			//	{
			//		person_color = al_map_rgb(
			//			color_intelligence.r*(people[i]->intelligence/intelligence_highest_historical * color_intelligence.r),
			//			color_intelligence.g*(people[i]->intelligence/intelligence_highest_historical * color_intelligence.g),
			//			color_intelligence.b*(people[i]->intelligence/intelligence_highest_historical * color_intelligence.b));
			//	}
			//	else//CURRENT
			//	{
			//		person_color = al_map_rgb(
			//			color_intelligence.r*(people[i]->intelligence/intelligence_highest_current * color_intelligence.r),
			//			color_intelligence.g*(people[i]->intelligence/intelligence_highest_current * color_intelligence.g),
			//			color_intelligence.b*(people[i]->intelligence/intelligence_highest_current * color_intelligence.b));
			//	}
			//}
			//else if(ui_state == FOREIGN)
			//{
			//	if(people[i]->foreign_x>=0 && people[i]->foreign_y>=0)
			//	{
			//		//first quadrant
			//		person_color = al_map_rgb(
			//			color_intelligence.r*((people[i]->foreign_x/foreign_max * color_foreign_east.r)+(people[i]->foreign_y/foreign_max * color_foreign_north.r) ),
			//			color_intelligence.g*((people[i]->foreign_x/foreign_max * color_foreign_east.g)+(people[i]->foreign_y/foreign_max * color_foreign_north.g)),
			//			color_intelligence.b*((people[i]->foreign_x/foreign_max * color_foreign_east.b)+(people[i]->foreign_y/foreign_max * color_foreign_north.b)));
			//	}
			//	else if(people[i]->foreign_x>=0 && people[i]->foreign_y>=0)
			//	{
			//		//second quadrant
			//		person_color = al_map_rgb(
			//			color_intelligence.r*((people[i]->foreign_x/-foreign_max * color_foreign_west.r)+(people[i]->foreign_y/foreign_max * color_foreign_north.r) ),
			//			color_intelligence.g*((people[i]->foreign_x/-foreign_max * color_foreign_west.g)+(people[i]->foreign_y/foreign_max * color_foreign_north.g)),
			//			color_intelligence.b*((people[i]->foreign_x/-foreign_max * color_foreign_west.b)+(people[i]->foreign_y/foreign_max * color_foreign_north.b)));
			//	}
			//	else if(people[i]->foreign_x>=0 && people[i]->foreign_y>=0)
			//	{
			//		//third quadrant
			//		person_color = al_map_rgb(
			//			color_intelligence.r*((people[i]->foreign_x/-foreign_max * color_foreign_west.r)+(people[i]->foreign_y/-foreign_max * color_foreign_south.r) ),
			//			color_intelligence.g*((people[i]->foreign_x/-foreign_max * color_foreign_west.g)+(people[i]->foreign_y/-foreign_max * color_foreign_south.g)),
			//			color_intelligence.b*((people[i]->foreign_x/-foreign_max * color_foreign_west.b)+(people[i]->foreign_y/-foreign_max * color_foreign_south.b)));
			//	}
			//	else
			//	{
			//		//fourth quadrant
			//		person_color = al_map_rgb(
			//			color_intelligence.r*((people[i]->foreign_x/foreign_max * color_foreign_east.r)+(people[i]->foreign_y/-foreign_max * color_foreign_south.r) ),
			//			color_intelligence.g*((people[i]->foreign_x/foreign_max * color_foreign_east.g)+(people[i]->foreign_y/-foreign_max * color_foreign_south.g)),
			//			color_intelligence.b*((people[i]->foreign_x/foreign_max * color_foreign_east.b)+(people[i]->foreign_y/-foreign_max * color_foreign_south.b)));
			//	}
			//}
			//else //Generation
			//{
			//	person_color = al_map_rgb(
			//		color_intelligence.r*(people[i]->generation/ generation_youngest * color_generation.r),
			//		color_intelligence.g*(people[i]->generation/ generation_youngest * color_generation.g),
			//		color_intelligence.b*(people[i]->generation/ generation_youngest * color_generation.b));
			//}
			for (int c = 0; c < 3; c++)
			{
				if(color[c]>255)
				{
					color[c]=255;
				}
			}
			DrawBlade(people[i]->position_x,people[i]->position_y,color[0],color[1],color[2]);

		}
		DrawCluster(100,100,100,100,100);
	}
}

void Game::DrawCluster(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_pixel(x,y,al_map_rgb(r,g,b));
	al_draw_pixel(x+1,y,al_map_rgb(r,g,b));
	al_draw_pixel(x-1,y,al_map_rgb(r,g,b));
	al_draw_pixel(x,y+1,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-1,al_map_rgb(r,g,b));
};
void Game::DrawBlade(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_pixel(x,y,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-1,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-2,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-3,al_map_rgb(r,g,b));
	al_draw_pixel(x,y-4,al_map_rgb(r,g,b));
};

void Game::FreeMemory()
{
	for(std::vector<Person*>::size_type i = 0; i != people.size(); i++) 
	{
		delete people[i];
	}
};