
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
	al_init_primitives_addon();

	al_reserve_samples(10);
};

void Game::InitializeGame()
{	
	province_width = 20;
	province_height = 15;

	province_jiggle_width = (int)(province_width*0.8);
	province_jiggle_height = (int)(province_height*0.8);

	ui_state = POWER;
	resources_drawn = true;
	provinces_drawn = true;
	color_province_blending = false;
	draw_every_hour = true;

	arability_max = 100;

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
	hunger_seek_level = hunger_death_level/9;

	foreign_max=100;

	generation_youngest=1;

	player_id = 0;
	house_id = 0;
	province_id=0;

	CreateWorld();

};

void Game::CreateWorld()
{
	srand(time(NULL));
	CreateProvinces();
	//CreateResources(30);


	CreateGrassland();
	CreateForests();
	CreateDeserts();
	CreateEquator();
	//CreateMountains();
	CreateFrozenPoles();
	//CreateRivers();

	CreatePeople(10,100,75);

	generation_youngest=1;
	power_highest_person = NULL;
	strength_highest_person = NULL;
	intelligence_highest_person = NULL;
}

void Game::CreateProvinces()
{
	provinces_num_columns = (int)(screen_width/province_width);
	provinces_num_rows= (int)(screen_height/province_height);

	for (int i = 0; i < ((provinces_num_columns+1)*(provinces_num_rows+1)); i++)
	{
		province_vertices.push_back(NULL);
	}

	for (int y = 0; y < provinces_num_rows;y++)
	{
		std::vector<Province*> row;
		provinces.push_back(row);
		for (int x = 0; x < provinces_num_columns; x++)
		{
			Vector2* top_left;
			Vector2* top_right;
			Vector2* bottom_left;
			Vector2* bottom_right;

			//TOP LEFT
			int index = (y*(provinces_num_columns+1))+x;
			if(province_vertices[index]!=NULL)
			{
				top_left = province_vertices[index];
			}
			else
			{
				top_left = new Vector2((x*province_width), (y*province_height));
				province_vertices[index] = top_left;
			}

			//TOP RIGHT
			index = (y*(provinces_num_columns+1))+(x+1);
			if(province_vertices[index]!=NULL)
			{
				top_right = province_vertices[index];
			}
			else
			{
				top_right = new Vector2((x*province_width)+province_width, y*province_height);
				province_vertices[index] = top_right;
			}

			//BOTTOM LEFT
			index = ((y+1)*(provinces_num_columns+1))+x;
			if(province_vertices[index]!=NULL)
			{
				bottom_left = province_vertices[index];
			}
			else
			{
				bottom_left = new Vector2((x*province_width), (y*province_height) + province_height);
				province_vertices[index] = bottom_left;
			}

			//BOTTOM RIGHT
			index = ((y+1)*(provinces_num_columns+1))+(x+1);
			if(province_vertices[index]!=NULL)
			{
				bottom_right = province_vertices[index];
			}
			else
			{
				bottom_right = new Vector2((x*province_width)+province_width, (y*province_height) + province_height);
				province_vertices[index] = bottom_right;
			}
			Province* province = new Province(province_id,x,y,top_left,top_right,bottom_right,bottom_left);
			province->arability=70+ rand()%(30);
			province_id++;

			provinces[y].push_back(province);
		}
	}

	//Jiggle
	for(std::vector<Vector2*>::size_type v = 0; v != province_vertices.size(); v++) 
	{
		int x = v% (provinces_num_columns+1);
		int y = v/ (provinces_num_columns+1);
		if(x>0 && y>0 && x<provinces_num_columns && y<provinces_num_rows)
		{
			province_vertices[v]->x = province_vertices[v]->x + 5 - (province_jiggle_width/2) + (rand()%province_jiggle_width);
			province_vertices[v]->y = province_vertices[v]->y + 5 - (province_jiggle_height/2) + (rand()%province_jiggle_height);
		}
	}
};

void Game::CreateGrassland()
{
	for (int f = 0; f <3; f++)
	{
		int cluster_origin_province_x = rand()%provinces_num_columns;
		int cluster_origin_province_y = 10+rand()%(provinces_num_rows-20);

		int radius = 10+rand()%5;

		std::vector<Province*> grassland_blob = GetBlobOfProvinces(cluster_origin_province_x, cluster_origin_province_y, radius);
		for (int p = 0; p < grassland_blob.size(); p++)
		{
			grassland_blob[p]->SetBiome(GRASSLAND);
		}

		for (int w = 0; w < 10; w++)
		{
			int smallerRadius = ((rand()%(radius)));

			int location_x = cluster_origin_province_x + (-(radius*1)+(rand()%(radius*2)));
			int location_y = cluster_origin_province_y + (-(radius*1)+(rand()%(radius*2)));

			std::vector<Province*> grassland_smaller_blob = GetBlobOfProvinces(location_x, location_y, smallerRadius);
			for (int p = 0; p < grassland_smaller_blob.size(); p++)
			{
				grassland_smaller_blob[p]->SetBiome(GRASSLAND);
			}
		}
	}
};
void Game::CreateForests()
{
	for (int f = 0; f < 50; f++)
	{
		int cluster_origin_province_x = rand()%provinces_num_columns;
		int cluster_origin_province_y = rand()%provinces_num_rows;

		std::vector<Province*> forest_blob = GetBlobOfProvinces(cluster_origin_province_x, cluster_origin_province_y, 1+rand()%3);
		for (int p = 0; p < forest_blob.size(); p++)
		{
			if(forest_blob[p]->biome!=WATER)
			{
				forest_blob[p]->biome = FOREST;
			}
		}
	}
};
void Game::CreateDeserts()
{
	for (int f = 0; f < 10; f++)
	{
		int cluster_origin_province_x = rand()%provinces_num_columns;
		int cluster_origin_province_y = ((provinces_num_rows/5)*2) + rand()%(provinces_num_rows/5);

		std::vector<Province*> desert_blob = GetBlobOfProvinces(cluster_origin_province_x, cluster_origin_province_y, 2+rand()%3);
		for (int p = 0; p < desert_blob.size(); p++)
		{
			if(desert_blob[p]->biome!=WATER)
			{
				desert_blob[p]->biome = DESERT;
			}
		}
	}
};
void Game::CreateMountains()
{
	for (int m = 0; m < 5; m++)
	{
		int slope_x = 0;
		while(slope_x ==0)
		{
			slope_x = -1+rand()%5;
		}
		int slope_y = rand()%7;


		int starting_x = rand()%provinces_num_columns;
		int starting_y = 0;

		int waveLength = rand()%10;
		int current_wave = 0;

		int location_x = starting_x;
		int location_y = starting_y;
		while(true)
		{
			if(current_wave>=0)
			{
				if(location_y >=0 && location_y < provinces_num_rows && location_x >=0 && location_x < provinces_num_columns)
				{
					provinces[location_y][location_x]->SetBiome(ALPINE);
				}
				else
				{
					break;
				}
			}
			current_wave++;
			if(current_wave > waveLength)
			{
				current_wave = -waveLength;
			}

			location_y += 1;
			location_x += slope_x;
		}
	}
};
void Game::CreateEquator()
{
	for (int y = (provinces_num_columns/3)-1; y <= (provinces_num_columns/3)+1; y++)
	{
		for (int x = 0; x < provinces_num_columns; x++)
		{
			if(provinces[y][x]->biome!=WATER)
			{
				provinces[y][x]->biome = JUNGLE;
			}
		}
	}
};
void Game::CreateRivers()
{

};
void Game::CreateFrozenPoles()
{
	/*for (int y = 0; y < 2; y++)
	{
	for (int x = 0; x < provinces_num_columns; x++)
	{
	provinces[y][x]->biome = TUNDRA;
	}
	}*/

	for (int y = provinces_num_rows-1; y > provinces_num_rows-3; y--)
	{
		for (int x = 0; x < provinces_num_columns; x++)
		{
			provinces[y][x]->biome = TUNDRA;
		}
	}
};
void Game::CreateSeas()
{
	for (int f = 0; f <20; f++)
	{
		int cluster_origin_province_x = rand()%provinces_num_columns;
		int cluster_origin_province_y = rand()%provinces_num_rows;

		int radius = 5+rand()%5;

		std::vector<Province*> water_blob = GetBlobOfProvinces(cluster_origin_province_x, cluster_origin_province_y, radius);
		for (int p = 0; p < water_blob.size(); p++)
		{
			water_blob[p]->SetBiome(WATER);
		}

		for (int w = 0; w < 5; w++)
		{
			int smallerRadius = ((double)radius/2);

			int location_x = cluster_origin_province_x + (-radius+(rand()%(radius*2)));
			int location_y = cluster_origin_province_y + (-radius+(rand()%(radius*2)));

			std::vector<Province*> water_smaller_blob = GetBlobOfProvinces(location_x, location_y, smallerRadius);
			for (int p = 0; p < water_smaller_blob.size(); p++)
			{
				water_smaller_blob[p]->SetBiome(WATER);
			}
		}
	}
};

//This wraps by x but not y
std::vector<Province*> Game::GetBlobOfProvinces(int province_x, int province_y, int radius)
{
	std::vector<Province*> blob;

	//Center
	if(province_y >=0 && province_y < provinces_num_rows && province_x >=0 && province_x < provinces_num_columns)
	{ 
		blob.push_back(provinces[province_y][province_x]);
	}

	//Axis
	for (int r = 1; r <= radius; r++)
	{
		if(province_y-r >=0 && province_y-r < provinces_num_rows)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=provinces_num_columns;}
			if(province_x >= provinces_num_columns){wrapped_x-=provinces_num_columns;}

			blob.push_back(provinces[province_y-r][wrapped_x]);
		}
		if(province_y >=0 && province_y < provinces_num_rows)
		{ 
			int wrapped_x = province_x+r;
			if(province_x+r <0){wrapped_x+=provinces_num_columns;}
			if(province_x+r >= provinces_num_columns){wrapped_x-=provinces_num_columns;}

			blob.push_back(provinces[province_y][wrapped_x]);
		}
		if(province_y+r >=0 && province_y+r < provinces_num_rows)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=provinces_num_columns;}
			if(province_x >= provinces_num_columns){wrapped_x-=provinces_num_columns;}

			blob.push_back(provinces[province_y+r][wrapped_x]);
		}
		if(province_y >=0 && province_y < provinces_num_rows)
		{ 
			int wrapped_x = province_x-r;
			if(province_x-r <0){wrapped_x+=provinces_num_columns;}
			if(province_x-r >= provinces_num_columns){wrapped_x-=provinces_num_columns;}

			blob.push_back(provinces[province_y][wrapped_x]);
		}
	}

	//Making circles
	int location_x = 0;
	int location_y = 0;
	for (int r = radius; r > 1; r--)
	{
		location_x = province_x;
		location_y = province_y-r;

		while(location_x != province_x+r)
		{
			location_x++;
			location_y++;
			if(location_y >=0 && location_y < provinces_num_rows)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=provinces_num_columns;}
				if(location_x >= provinces_num_columns){wrapped_location-=provinces_num_columns;}

				blob.push_back(provinces[location_y][wrapped_location]);
			}
		}
		while(location_x != province_x)
		{
			location_x--;
			location_y++;
			if(location_y >=0 && location_y < provinces_num_rows)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=provinces_num_columns;}
				if(location_x >= provinces_num_columns){wrapped_location-=provinces_num_columns;}

				blob.push_back(provinces[location_y][wrapped_location]);
			}
		}
		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;
			if(location_y >=0 && location_y < provinces_num_rows)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=provinces_num_columns;}
				if(location_x >= provinces_num_columns){wrapped_location-=provinces_num_columns;}

				blob.push_back(provinces[location_y][wrapped_location]);
			}
		}
		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;
			if(location_y >=0 && location_y < provinces_num_rows)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=provinces_num_columns;}
				if(location_x >= provinces_num_columns){wrapped_location-=provinces_num_columns;}
				blob.push_back(provinces[location_y][wrapped_location]);
			}
		}
	}

	return blob;
}

void Game::CreateResources(int myNumber)
{
	for (int i = 0; i < myNumber; i++)
	{
		int resource_province_x = rand()%provinces_num_columns;
		int resource_province_y = rand()%provinces_num_rows;

		//Province location
		resources.push_back(new Resource(resource_province_x,resource_province_y,
			provinces[resource_province_y][resource_province_x]->getCenter().x -5 + rand()%10,
			provinces[resource_province_y][resource_province_x]->getCenter().y -5 + rand()%10,10,5));

		//Random location
		//resources.push_back(new Resource(100+(rand()%(screen_width-200)),100+(rand()%(screen_height-200)),10));
	}
};
void Game::CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius)
{
	for(int c = 0 ; c<myNumberClusters;c++)
	{
		//Province Spawn
		int cluster_origin_province_x =0;
		int cluster_origin_province_y =0;
		do
		{
			cluster_origin_province_x = rand()%provinces_num_columns;
			cluster_origin_province_y = rand()%provinces_num_rows;
		} while (provinces[cluster_origin_province_y][cluster_origin_province_x]->biome == WATER ||
			provinces[cluster_origin_province_y][cluster_origin_province_x]->biome == TUNDRA);


		int foreign_origin_x = cos(c*(2*3.14)/myNumberClusters) * myForeignRadius;
		int foreign_origin_y = sin(c*(2*3.14)/myNumberClusters) * myForeignRadius;

		for(int p = 0 ; p < myPeoplePerCluster;p++)
		{
			int province_x = cluster_origin_province_x;
			int province_y = cluster_origin_province_y;

			//5int province_area = provinces[province_y][province_x]->getArea();
			Vector2 province_center = provinces[province_y][province_x]->getCenter();

			Person* person = new Person(player_id,
				(rand()%hunger_seek_level),
				3+(1+rand()%50),
				1,
				-10+foreign_origin_x+(rand()%20), 
				-10+foreign_origin_y+(rand()%20),
				province_x,
				province_y,
				province_center.x -(province_width/2) + rand()%province_width,
				province_center.y -(province_height/2) + rand()%province_height);

			Province* prov = provinces[province_y][province_x];
			prov->people_on_province[player_id] = person;

			player_id = player_id+1;

			person->power+=rand()%40;

			people.push_back(person);
		}
	}
};

void Game::LoadContent()
{
	arial24 = al_load_font("arial.ttf",24,0);
	arial16 = al_load_font("arial.ttf",16,0);
	arial12 = al_load_font("arial.ttf",12,0);
	DefineColors();
};
void Game::DefineColors()
{
	color_base_value = 0.5;

	color_text[0] = 255;color_text[1] = 255;color_text[2] = 255;

	color_resource[0] = 200;color_resource[1] = 200;color_resource[2] = 200;
	color_house[0] = 150;color_house[1] = 150;color_house[2] = 150;

	color_grassland[0] = 75;color_grassland[1] =150;color_grassland[2] = 60;
	color_jungle[0] = 130;color_jungle[1] =140;color_jungle[2] = 70;
	color_desert[0] = 245;color_desert[1] =245;color_desert[2] = 100;
	color_water[0] = 0;color_water[1] =0;color_water[2] = 150;
	color_tundra[0] = 235;color_tundra[1] =255;color_tundra[2] = 235;
	color_alpine[0] = 190;color_alpine[1] =190;color_alpine[2] = 170;
	color_forest[0] = 140;color_forest[1] =200;color_forest[2] = 130;


	color_occupation_farmer[0] = 5;color_occupation_farmer[1] = 102;color_occupation_farmer[2] = 100;
	color_occupation_artisan[0] = 252;color_occupation_artisan[1] = 50;color_occupation_artisan[2] = 4;

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
	double delta_time_for_hour = .0001;
	double current_timer = 0;

	Draw();
	while(!done)
	{
		//Delta time handling
		double current_timestamp = al_get_time();
		double seconds_since_last_tick = current_timestamp - previous_tick_timestamp;
		current_timer+= seconds_since_last_tick;
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
			if(current_timer>delta_time_for_hour)
			{
				current_timer-=delta_time_for_hour;
				RunTime();
			}
			accumulator -= dt;
		}


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

	/*//WASD
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
	}*/

	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_P))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_P))
		{
			ui_state = POWER;
			//Draw();
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_I))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_I))
		{
			ui_state = INTELLIGENCE;
			//Draw();
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_S))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_S))
		{
			ui_state = STRENGTH;
			//Draw();
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_H))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_H))
		{
			ui_state = HUNGER;
			//Draw();
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_F))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_F))
		{
			ui_state = FOREIGN;
			//Draw();
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_G))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_G))
		{
			ui_state = GENERATION;
			//Draw();
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_O))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_O))
		{
			ui_state = OCCUPATION;
			//Draw();
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_L))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_L))
		{
			//Draw();
			if(provinces_drawn)
				provinces_drawn=false;
			else
				provinces_drawn = true;
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_D))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_D))
		{
			if(draw_every_hour)
				draw_every_hour=false;
			else
				draw_every_hour = true;
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_R))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_R))
		{
			//Draw();
			if(resources_drawn)
				resources_drawn=false;
			else
				resources_drawn = true;
		}
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_B))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_B))
		{
			//Draw();
			if(color_province_blending)
				color_province_blending=false;
			else
				color_province_blending = true;
		}
	}

	old_keyboard_state = new_keyboard_state;
};

void Game::RunTime()
{
	//Do all your hourly logic here
	ProcessPeople();

	current_hour++;

	DivvyUpFood();


	if(draw_every_hour || current_hour==12)
		Draw();


	if(current_hour>hours_in_day)
	{
		current_hour=0;
		current_day++;

		//Do all your daily logic here
		UpdateProvinceFood();

		if(current_day>days_in_year)
		{
			//Do all your yearly logic here
			current_day = 0;
			current_year++;
		}

	}
}

void Game::ProcessPeople()
{
	for(std::vector<Person*>::size_type i = 0; i < people.size(); i++) 
	{
		if(i>=0 && i <people.size())
		{
			/*if(people[i]->hunger>=hunger_death_level)
			{
			House* home = people[i]->home;
			if(home!=NULL)
			{
			home->abandoned = true;
			provinces[home->province_y][home->province_x]->homes_on_province.erase(home->id);
			}

			people[i]->dead = true;
			provinces[people[i]->province_y][people[i]->province_x]->people_on_province.erase(people[i]->id);
			delete(people[i]);
			people.erase(people.begin()+i);
			i--;
			continue;
			}*/

			if(current_hour==11 || current_hour==16 || current_hour==20)
			{
				people[i]->hunger= people[i]->hunger + hunger_seek_level;
			}

			ProcessPersonAI(people[i]);
		}
	}
};
void Game::ProcessPersonAI(Person* person)
{
	/*Priority Order
	Food
	Sleep
	Building up food, shelter, crafts
	Interaction
	Dominion
	Philisophical
	*/


	if(person->hunger>=hunger_seek_level)
	{
		//Hungry
		SeekFood(person);
	}
	//else if(current_hour < 6 || current_hour > 20)
	//{
	//	//Sleepy
	//	if(person->home!=NULL)
	//	{
	//		if(person->province_x != person->home->province_x || person->province_y != person->home->province_y)
	//		{
	//			//Go home
	//			MoveToCoordinates(person,person->home->province_x,person->home->province_y);
	//		}
	//	}
	//}
	/*else if( current_hour == 9 || current_hour ==19 ||current_hour ==20 || current_hour==21)
	{
	SeekInteraction(person);
	}*/
	else if(person->home == NULL)
	{
		int province_x = person->province_x;
		int province_y = person->province_y;

		//Calculating the power buy in for the home
		Province* prov = provinces[province_y][province_x];
		int province_total_power = 0;

		if(prov->homes_on_province.size()!=0)
		{
			for (auto h = prov->homes_on_province.begin(); h != prov->homes_on_province.end(); ++h)
			{
				House* home = h->second;
				province_total_power += home->owner->power;
			}
			province_total_power = province_total_power/prov->homes_on_province.size();
		}

		//Can you afford it?
		if(person->power>=province_total_power)
		{
			person->power-=province_total_power;
			BuildHome(person);
		}
		else
		{
			MoveRandomDirection(person);
		}
	}
	else
	{
		if(person->occupation == FARMER)
		{
			////If there isnt a house here
			//if(provinces[person->province_y][person->province_x]->homes_on_province.size()!=0)
			//{
			//	//Work the land
			//	provinces[person->home->province_y][person->home->province_x] ->food_in_province += person->strength;
			//}
			//else
			//{
			//	MoveRandomDirection(person);
			//}
		}
		else if (person->occupation == ARTISAN)
		{

		}
	}
};
void Game::SeekFood(Person* person)
{
	//Currently eating -whatever amount- takes an hour

	if(person->food_carried>0)//
	{
		//Eat all your food
		int amount_eaten = person->food_carried - person->hunger;
		if(amount_eaten<=0)
		{
			person->food_carried = 0;
			person->hunger-=person->food_carried;
		}
		else if(amount_eaten>0)
		{
			person->hunger-=amount_eaten;
			person->food_carried-=amount_eaten;
		}
		//If you are more than food add the food back to what you carried
		if(person->hunger<0)
		{
			person->food_carried-=person->hunger;
			person->hunger = 0;
		}
	}
	else//You don't have any food
	{
		GetInLineForFood(person->province_x,person->province_y,person);
	}
	//else if(person->home->food_stored!=0)//If you have food in your home
	//{
	//	if(person->province_x == person->home->province_x &&person->province_x == person->home->province_x )
	//	{
	//		//You are on your houses tile
	//	} 
	//	else
	//	{
	//		//You are away from home
	//	}
	//}



};
void Game::Sleep(Person* person)
{

};
void Game::BuildResources(Person* person)
{

};
void Game::BuildHome(Person* person)
{
	House* home = new House(house_id,person,person->province_x,person->province_y,
		provinces[person->province_y][person->province_x]->getCenter().x-((province_width)/2)+(rand()% (province_width)),
		provinces[person->province_y][person->province_x]->getCenter().y-((province_height)/2)+(rand()%(province_height)));

	person->home = home;
	Province* prov = provinces[person->province_y][person->province_x];
	prov->homes_on_province[house_id] = home;//home;
	house_id=house_id+1;
};
void Game::SeekInteraction(Person* person)
{
	std::vector<Province*> adjacent_provinces = GetAllAdjacentProvinces(person->province_x,person->province_y);

	Province* most_populated_neighbor = provinces[person->province_y][person->province_x];

	for (int p = 0; p < adjacent_provinces.size(); p++)
	{
		int prov_size =  most_populated_neighbor->people_on_province.size();
		int adj_size =adjacent_provinces[p]->people_on_province.size();

		if(adj_size >prov_size)
		{
			most_populated_neighbor = adjacent_provinces[p];
		}
	}
	MoveToCoordinates(person,most_populated_neighbor->province_x,most_populated_neighbor->province_y);

	if(person->province_x == most_populated_neighbor->province_x &&person->province_y == most_populated_neighbor->province_y)
	{
		if(most_populated_neighbor->people_on_province.size() >0)
		{
			int randomPersonIndex = rand()%most_populated_neighbor->people_on_province.size();
			Person* conversation_partner = most_populated_neighbor->people_on_province[randomPersonIndex];
			if(conversation_partner !=NULL)
			{
				int power_transfer = 1;

				//Whoever is stronger gets more powerful
				if(person->strength>conversation_partner->strength)
				{
					person->power = person->power+power_transfer;
					conversation_partner->power = conversation_partner->power-power_transfer;
				}
				else
				{
					person->power = person->power-power_transfer;
					conversation_partner->power = conversation_partner->power+power_transfer;
				}

				//Whoever is smarter gets more powerful
				if(person->intelligence>conversation_partner->intelligence)
				{
					person->power = person->power+power_transfer;
					conversation_partner->power = conversation_partner->power-power_transfer;
				}
				else
				{
					person->power = person->power-power_transfer;
					conversation_partner->power = conversation_partner->power+power_transfer;
				}

				//They cant be less than human
				if(person->power <0)
					person->power =0;
				if(conversation_partner->power <0)
					conversation_partner->power = 0;

				//Foreigness pulling//
			}
			else{most_populated_neighbor->people_on_province.erase(randomPersonIndex);}
		}
	}
};
void Game::SeekDominion(Person* person)
{

};

void Game::MoveRandomDirection(Person* person)
{
	int direction = rand()%20;

	provinces[person->province_y][person->province_x]->people_on_province.erase(person->id);

	switch (direction)
	{
	case 0 :
		//Up
		if(person->province_y-1>=0)
		{
			if(provinces[person->province_y-1][person->province_x]->biome !=WATER)
			{
				person->province_y = person->province_y-1;

				UpdatePersonPositionToProvince(person);
			}
			else
			{
				MoveRandomDirection(person);
			}
		}
		break;
	case 1 :
		if(person->province_x+1<provinces_num_columns)
		{
			if(provinces[person->province_y][person->province_x+1]->biome !=WATER)
			{
				person->province_x = person->province_x+1;

				UpdatePersonPositionToProvince(person);
			}
			else
			{
				MoveRandomDirection(person);
			}
		}
		else
		{
			if(provinces[person->province_y][person->province_x+1-provinces_num_columns]->biome !=WATER)
			{
				person->province_x = person->province_x+1-provinces_num_columns;

				UpdatePersonPositionToProvince(person);
			}
			else
			{
				MoveRandomDirection(person);
			}
		}
		break;
	case 2 :
		//Down
		if(person->province_y+1<provinces_num_rows)
		{
			if(provinces[person->province_y+1][person->province_x]->biome !=WATER)
			{
				person->province_y = person->province_y+1;

				UpdatePersonPositionToProvince(person);
			}
			else
			{
				MoveRandomDirection(person);
			}
		}
		break;
	case 3 :
		//Left
		if(person->province_x-1>=0)
		{
			if(provinces[person->province_y][person->province_x-1]->biome !=WATER)
			{
				person->province_x = person->province_x-1;

				UpdatePersonPositionToProvince(person);
			}
			else
			{
				MoveRandomDirection(person);
			}
		}
		else
		{
			if(provinces[person->province_y][person->province_x-1+provinces_num_columns]->biome !=WATER)
			{
				person->province_x = person->province_x-1+provinces_num_columns;

				UpdatePersonPositionToProvince(person);
			}
			else
			{
				MoveRandomDirection(person);
			}
		}
		break;
	default:
		break;
	}
	provinces[person->province_y][person->province_x]->people_on_province[person->id] = person;
};
void Game::MoveToCoordinates(Person* person, int x,int y)
{
	//If you are within bounds
	if(x != person->province_x && y!=person->province_y)
	{
		if(x>=0 && x<provinces_num_columns && y>=0 && y<provinces_num_rows)
		{
			provinces[person->province_y][person->province_x]->people_on_province.erase(person->id);
			int location_difference_x = x - person->province_x;
			int location_difference_y = y - person->province_y;

			if(std::abs(location_difference_x)>std::abs(location_difference_y))
			{
				//X difference is greater than y difference
				//move in x diretion
				if(location_difference_x>0)
				{
					//Move right
					person->province_x = person->province_x+1;
					if(person->province_x<provinces_num_columns)
					{
						UpdatePersonPositionToProvince(person);

					}
					else
					{
						person->province_x = person->province_x-1; 
					}
				}
				else
				{
					//Move left
					person->province_x = person->province_x-1;
					if(person->province_x>=0)
					{
						UpdatePersonPositionToProvince(person);
					}
					else
					{
						person->province_x = person->province_x+1; 
					}
				}
			}
			else if(std::abs(location_difference_x)==std::abs(location_difference_y))
			{
				if(location_difference_x>0)
				{
					//Move right
					person->province_x = person->province_x+1;
					if(person->province_x<provinces_num_columns)
					{
						UpdatePersonPositionToProvince(person);

					}
					else
					{
						person->province_x = person->province_x-1; 
					}
				}
				else
				{
					//Move left
					person->province_x = person->province_x-1;
					if(person->province_x>=0)
					{
						UpdatePersonPositionToProvince(person);
					}
					else
					{
						person->province_x = person->province_x+1; 
					}
				}
				if(location_difference_y>0)
				{
					//Down
					person->province_y = person->province_y+1;
					if(person->province_y<provinces_num_rows)
					{
						UpdatePersonPositionToProvince(person);

					}
					else
					{
						person->province_y = person->province_y-1; 
					}

				}
				else
				{
					//Up
					person->province_y = person->province_y-1;
					if(person->province_y>=0)
					{
						UpdatePersonPositionToProvince(person);
					}
					else
					{
						person->province_y = person->province_y+1; 
					}
				}
			}
			else
			{
				// ydifference is greater than x difference
				//move in y diretion

				if(location_difference_y>0)
				{
					//Down
					person->province_y = person->province_y+1;
					if(person->province_y<provinces_num_rows)
					{
						UpdatePersonPositionToProvince(person);

					}
					else
					{
						person->province_y = person->province_y-1; 
					}

				}
				else
				{
					//Up
					person->province_y = person->province_y-1;
					if(person->province_y>=0)
					{
						UpdatePersonPositionToProvince(person);
					}
					else
					{
						person->province_y = person->province_y+1; 
					}
				}
			}
		}
		provinces[person->province_y][person->province_x]->people_on_province[person->id] = person;
	}
};

void Game::GetInLineForFood(int x,int y,Person* myPerson)
{
	Province* prov = provinces[y][x];
	prov->people_in_line.push_back(myPerson);

	for (int i = 0; i < provinces_with_hungry_people.size(); i++)
	{
		if(prov->id == provinces_with_hungry_people[i]->id)
		{
			return;
		}
	}
	provinces_with_hungry_people.push_back(prov);

};
void Game::UpdateProvinceFood()
{
	for (int y = 0; y < provinces_num_rows; y++)
	{
		for (int x = 0; x < provinces_num_columns; x++)
		{
			Province* province = provinces[y][x];

			province->food_in_province = 0;
			switch (province->biome)
			{
			case GRASSLAND:
				province->food_in_province=hunger_seek_level*20;
				break;
			case DESERT:
				province->food_in_province=hunger_seek_level*5;
				break;
			case JUNGLE:
				province->food_in_province=hunger_seek_level*7;
				break; 
			case FOREST: 
				province->food_in_province=hunger_seek_level*10;
				break; 
			case TUNDRA:
				province->food_in_province=hunger_seek_level*5;
				break;
			case ALPINE:
				province->food_in_province=hunger_seek_level*10;
				break;
			default:
				break;
			}
		}

	}
};
void Game::DivvyUpFood()
{
	if(provinces_with_hungry_people.size()!=0)
	{
		for (int i = 0; i < provinces_with_hungry_people.size(); i++)
		{
			if(i<0 || i >=provinces_with_hungry_people.size()){
				return;}
			Province* prov = provinces_with_hungry_people[i];

			std::vector<Person*> line = prov->people_in_line;

			//std::sort(line.begin(),line.end());
			for (int p =  line.size()-1; p >-1; p--)
			{
				if(prov->food_in_province>0)
				{
					if(p<0 || p >=line.size()){
						return;}
					Person* next_in_line = line[p];

					int amountTaken = ((next_in_line->strength*next_in_line->power)/hunger_seek_level);

					if(amountTaken<prov->food_in_province)
					{
						prov->food_in_province-=amountTaken;

					}
					else
					{
						amountTaken = prov->food_in_province;
						prov->food_in_province=0;

					}
					line[p]->food_carried+=amountTaken;
				}
				else
				{
					//Find place with more food
					std::vector<Province*> adjacent_provinces = GetAllAdjacentProvinces(line[p]->province_x,line[p]->province_y);

					Province* least_populated_neighbor = provinces[line[p]->province_y][line[p]->province_x];

					for (int p = 0; p < adjacent_provinces.size(); p++)
					{
						int prov_size =  least_populated_neighbor->people_on_province.size();
						int adj_size =adjacent_provinces[p]->people_on_province.size();

						if(adj_size <prov_size)
						{
							least_populated_neighbor = adjacent_provinces[p];
						}
					}
					//MoveToCoordinates(line[p],least_populated_neighbor->province_x,least_populated_neighbor->province_y);

					MoveRandomDirection(line[p]);
				}
			}
			prov->people_in_line.clear();
		}
		provinces_with_hungry_people.clear();
	}
};
std::vector<Province*> Game::GetAllAdjacentProvinces(int x,int y)
{
	std::vector<Province*> adjacentProvinces;
	if(y-1>=0 && x-1>=0)
		adjacentProvinces.push_back(provinces[y-1][x-1]);
	if(y-1>=0 && x+1<provinces_num_columns)
		adjacentProvinces.push_back(provinces[y-1][x+1]);
	if(y+1<provinces_num_rows && x+1<provinces_num_columns)
		adjacentProvinces.push_back(provinces[y+1][x+1]);
	if(y+1<provinces_num_rows && x-1>=0)
		adjacentProvinces.push_back(provinces[y+1][x-1]);


	if(y-1>=0)
		adjacentProvinces.push_back(provinces[y-1][x]);
	if(x+1<provinces_num_columns)
		adjacentProvinces.push_back(provinces[y][x+1]);
	if(y+1<provinces_num_rows)
		adjacentProvinces.push_back(provinces[y+1][x]);
	if(x-1>=0)
		adjacentProvinces.push_back(provinces[y][x-1]);

	return adjacentProvinces;
}
std::vector<Province*> Game::GetFourAdjacentProvinces(int x,int y)
{
	std::vector<Province*> adjacentProvinces;
	if(y-1>=0)
		adjacentProvinces.push_back(provinces[y-1][x]);
	if(x+1<provinces_num_columns)
		adjacentProvinces.push_back(provinces[y][x+1]);
	if(y+1<provinces_num_rows)
		adjacentProvinces.push_back(provinces[y+1][x]);
	if(x-1>=0)
		adjacentProvinces.push_back(provinces[y][x-1]);
	return adjacentProvinces;
}

void Game::UpdatePersonPositionToProvince(Person* person)
{
	Vector2 province_center = provinces[person->province_y][person->province_x]->getCenter();
	person->position_x = province_center.x -(province_width/2) + rand()%province_width;
	person->position_y =province_center.y -(province_height/2) + rand()%province_height;
};

void Game::Draw()
{
	if(provinces_drawn)
		DrawProvinces();

	DrawPeople();

	DrawHouses();

	if(resources_drawn)
		DrawResources();

	DrawDate();
	DrawPopulation();

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
};

void Game::DrawProvinces()
{
	for(std::vector<std::vector<Province*>>::size_type y = 0; y < provinces_num_rows; y++) 
	{
		for(std::vector<Province*>::size_type x = 0; x < provinces_num_columns; x++) 
		{
			Province* province = (provinces[y][x]);

			int color[3];

			switch (province->biome)
			{
			case GRASSLAND:
				color[0] = color_grassland[0];
				color[1] = color_grassland[1];
				color[2] = color_grassland[2];
				break;
			case DESERT:
				color[0] = color_desert[0];
				color[1] = color_desert[1];
				color[2] = color_desert[2];
				break;
			case JUNGLE:
				color[0] = color_jungle[0];
				color[1] = color_jungle[1];
				color[2] = color_jungle[2];
				break;
			case FOREST:
				color[0] = color_forest[0];
				color[1] = color_forest[1];
				color[2] = color_forest[2];
				break;
			case TUNDRA:
				color[0] = color_tundra[0];
				color[1] = color_tundra[1];
				color[2] = color_tundra[2];
				break;
			case ALPINE:
				color[0] = color_alpine[0];
				color[1] = color_alpine[1];
				color[2] = color_alpine[2];
				break;
			case WATER:
				color[0] = color_water[0];
				color[1] = color_water[1];
				color[2] = color_water[2];
				break;
			}

			//Provicne color Seperation
			color[0] = ((double)province->arability/arability_max) * color[0];
			color[1] = ((double)province->arability/arability_max) * color[1];
			color[2] = ((double)province->arability/arability_max) * color[2];

			/*color[0] = ((91 + (rand()%20))/100) * color[0];
			color[1] = ((91 + (rand()%20))/100) * color[1];
			color[2] = ((91 + (rand()%20))/100) * color[2];*/

			ALLEGRO_VERTEX vertices[] = 
			{
				{province->p0->x, province->p0->y, 0},
				{province->p1->x,province->p1->y, 0},
				{province->p2->x,province->p2->y,0},
				{province->p3->x,province->p3->y,0},
			};
			if(color_province_blending)
			{
				CalculateVertexColor(x,y,vertices);
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					vertices[i].color = al_map_rgb(color[0],color[1],color[2]);
				}
			}
			al_draw_prim(vertices, NULL, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN );

			////Draw border
			//al_draw_line(province->p0->x,province->p0->y,province->p1->x,province->p1->y,
			//	al_map_rgb(color[0],color[1],color[2]),2);
			//al_draw_line(province->p1->x,province->p1->y,province->p3->x,province->p3->y,
			//	al_map_rgb(color[0],color[1],color[2]),2);
			//al_draw_line(province->p3->x,province->p3->y,province->p2->x,province->p2->y,
			//	al_map_rgb(color[0],color[1],color[2]),2);
			//al_draw_line(province->p2->x,province->p2->y,province->p0->x,province->p0->y,
			//	al_map_rgb(color[0],color[1],color[2]),2);

			/*al_draw_line(provinces[y][x]->vertices[0],provinces[y][x]->vertices[1],provinces[y][x]->vertices[2],provinces[y][x]->vertices[3],
			al_map_rgb(255,255,255),1);

			al_draw_line(provinces[y][x]->vertices[2],provinces[y][x]->vertices[3],provinces[y][x]->vertices[4],provinces[y][x]->vertices[5],
			al_map_rgb(255,255,255),1);

			al_draw_line(provinces[y][x]->vertices[4],provinces[y][x]->vertices[5],provinces[y][x]->vertices[6],provinces[y][x]->vertices[7],
			al_map_rgb(255,255,255),1);

			al_draw_line(provinces[y][x]->vertices[6],provinces[y][x]->vertices[7],provinces[y][x]->vertices[0],provinces[y][x]->vertices[1],
			al_map_rgb(255,255,255),1);*/
		}
	}
};
void Game::CalculateVertexColor(int x, int y, ALLEGRO_VERTEX* myVertices)
{
	//int color[3];

	//int average_arability = 0;

	////Top left vertex
	//{
	//	int total_arability = 0;
	//	int provinces_used = 0;

	//	int bottomRightArability;
	//	int topLeftArability;
	//	int topRightArability;
	//	int bottomLeftArability;

	//	if(y-1>=0 && x-1>=0)//Topleft
	//	{
	//		bottomRightArability = provinces[y-1][x-1]->arability;
	//		total_arability+= bottomRightArability;
	//		provinces_used++;
	//	}

	//	if(y-1>=0)//Topright
	//	{
	//		topLeftArability = provinces[y-1][x]->arability;
	//		total_arability+=topLeftArability;
	//		provinces_used++;
	//	}
	//	if(true)//bottomright
	//	{
	//		topRightArability = provinces[y][x]->arability;
	//		total_arability+= topRightArability;
	//		provinces_used++;
	//	}

	//	if(x-1>=0)//bottomleft
	//	{
	//		bottomLeftArability = provinces[y][x-1]->arability;
	//		total_arability+= bottomLeftArability;
	//		provinces_used++;
	//	}
	//	average_arability = total_arability/provinces_used;

	//	//Top left vertex
	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
	//	color[2] = ((double)average_arability/arability_max) * color_province[2];

	//	myVertices[0].color = al_map_rgb(color[0],color[1],color[2]);
	//}
	////Top right vertex
	//{
	//	int total_arability = 0;
	//	int provinces_used = 0;

	//	int bottomRightArability;
	//	int topLeftArability;
	//	int topRightArability;
	//	int bottomLeftArability;

	//	if(y-1>=0)//Topleft
	//	{
	//		bottomRightArability = provinces[y-1][x]->arability;
	//		total_arability+= bottomRightArability;
	//		provinces_used++;
	//	}

	//	if(y-1>=0 && x+1<provinces_num_columns)//Topright
	//	{
	//		topLeftArability = provinces[y-1][x+1]->arability;
	//		total_arability+=topLeftArability;
	//		provinces_used++;
	//	}
	//	if(x+1<provinces_num_columns)//bottomright
	//	{
	//		topRightArability = provinces[y][x+1]->arability;
	//		total_arability+= topRightArability;
	//		provinces_used++;
	//	}

	//	if(true)//bottomleft
	//	{
	//		bottomLeftArability = provinces[y][x]->arability;
	//		total_arability+= bottomLeftArability;
	//		provinces_used++;
	//	}
	//	average_arability = total_arability/provinces_used;

	//	//Top right vertex
	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
	//	color[2] = ((double)average_arability/arability_max) * color_province[2];

	//	myVertices[1].color = al_map_rgb(color[0],color[1],color[2]);
	//}
	////Bottomright vertex
	//{
	//	int total_arability = 0;
	//	int provinces_used = 0;

	//	int bottomRightArability;
	//	int topLeftArability;
	//	int topRightArability;
	//	int bottomLeftArability;

	//	if(true)//Topleft
	//	{
	//		bottomRightArability = provinces[y][x]->arability;
	//		total_arability+= bottomRightArability;
	//		provinces_used++;
	//	}

	//	if(x+1<provinces_num_columns)//Topright
	//	{
	//		topLeftArability = provinces[y][x+1]->arability;
	//		total_arability+=topLeftArability;
	//		provinces_used++;
	//	}
	//	if(x+1<provinces_num_columns&&y+1<provinces_num_rows)//bottomright
	//	{
	//		topRightArability = provinces[y+1][x+1]->arability;
	//		total_arability+= topRightArability;
	//		provinces_used++;
	//	}

	//	if(y+1<provinces_num_rows)//bottomleft
	//	{
	//		bottomLeftArability = provinces[y+1][x]->arability;
	//		total_arability+= bottomLeftArability;
	//		provinces_used++;
	//	}
	//	average_arability = total_arability/provinces_used;

	//	//Bottomright vertex
	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
	//	color[2] = ((double)average_arability/arability_max) * color_province[2];

	//	myVertices[2].color = al_map_rgb(color[0],color[1],color[2]);
	//}
	////Bottomleft vertex
	//{
	//	int total_arability = 0;
	//	int provinces_used = 0;

	//	int bottomRightArability;
	//	int topLeftArability;
	//	int topRightArability;
	//	int bottomLeftArability;

	//	if(x-1>=0)//Topleft
	//	{
	//		bottomRightArability = provinces[y][x-1]->arability;
	//		total_arability+= bottomRightArability;
	//		provinces_used++;
	//	}

	//	if(true)//Topright
	//	{
	//		topLeftArability = provinces[y][x]->arability;
	//		total_arability+=topLeftArability;
	//		provinces_used++;
	//	}
	//	if(y+1<provinces_num_rows)//bottomright
	//	{
	//		topRightArability = provinces[y+1][x]->arability;
	//		total_arability+= topRightArability;
	//		provinces_used++;
	//	}

	//	if(y+1<provinces_num_rows && x-1>=0)//bottomleft
	//	{
	//		bottomLeftArability = provinces[y+1][x-1]->arability;
	//		total_arability+= bottomLeftArability;
	//		provinces_used++;
	//	}
	//	average_arability = total_arability/provinces_used;

	//	//Bottomleft vertex
	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
	//	color[2] = ((double)average_arability/arability_max) * color_province[2];

	//	myVertices[3].color = al_map_rgb(color[0],color[1],color[2]);
	//}

};
void Game::DrawPeople()
{
	//Maintianing highest of power strength and intel
	if(power_highest_person != NULL)
		if(power_highest_person->dead)
			power_highest_person = NULL;
	if(strength_highest_person != NULL)
		if(strength_highest_person->dead)
			strength_highest_person = NULL;
	if(intelligence_highest_person != NULL)
		if(intelligence_highest_person->dead)
			intelligence_highest_person=NULL;


	//Processing highest
	for(std::vector<Person*>::size_type i = 0; i != people.size(); i++) 
	{
		if(!people[i]->dead)
		{
			if(power_highest_person == NULL)
				power_highest_person = people[i];
			if(strength_highest_person== NULL)
				strength_highest_person  = people[i];
			if(intelligence_highest_person== NULL)
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
		Person* person = people[i];
		if(person->dead == false){
			ALLEGRO_COLOR person_color;
			double color [3]; 
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;

			if(ui_state == POWER)
			{
				if(power_context == HISTORY)
				{
					double ratio =(double) person->power/power_highest_historical;
					color [0]= color_power[0]*(ratio);
				}
				else //CURRENT
				{
					color[0] = color_power[0]*((double)person->power/power_highest_current);
				}
			}
			else if(ui_state == HUNGER)
			{
				color [0] =color_hunger[0]*(double)person->hunger/hunger_death_level;
				color [1] =color_hunger[1]*(double)person->hunger/hunger_death_level;
				color [2] =color_hunger[2]*(double)person->hunger/hunger_death_level;
			}
			else if(ui_state == STRENGTH)
			{
				if(strength_context == HISTORY)
				{
					color [0] =color_strength[0]*(double)person->strength/strength_highest_historical;
					color [1] =color_strength[1]*(double)person->strength/strength_highest_historical;
					color [2] =color_strength[2]*(double)person->strength/strength_highest_historical;
				}
				else//CURRENT
				{
					color [0] =color_strength[0]*(double)person->strength/strength_highest_current;
					color [1] =color_strength[1]*(double)person->strength/strength_highest_current;
					color [2] =color_strength[2]*(double)person->strength/strength_highest_current;
				}
			}
			else if(ui_state == INTELLIGENCE)
			{
				if(intelligence_context == HISTORY)
				{
					color [0] =color_intelligence[0]*(double)person->intelligence/intelligence_highest_historical;
					color [1] =color_intelligence[1]*(double)person->intelligence/intelligence_highest_historical;
					color [2] =color_intelligence[2]*(double)person->intelligence/intelligence_highest_historical;
				}
				else//CURRENT
				{
					color [0] = color_intelligence[0]*(double)person->intelligence/intelligence_highest_current;
					color [1] = color_intelligence[1] *(double)person->intelligence/intelligence_highest_current;
					color [2] = color_intelligence[2] *(double)person->intelligence/intelligence_highest_current;
				}
			}
			else if(ui_state == FOREIGN)
			{
				if(person->foreign_x>=0 && person->foreign_y>=0)
				{
					//first quadrant

					color [0] =((double)person->foreign_x/foreign_max * color_foreign_east[0])+((double)person->foreign_y/foreign_max * color_foreign_north[0]);
					color [1] =((double)person->foreign_x/foreign_max * color_foreign_east[1])+((double)person->foreign_y/foreign_max * color_foreign_north[1]);
					color [2] =((double)person->foreign_x/foreign_max * color_foreign_east[2])+((double)person->foreign_y/foreign_max * color_foreign_north[2]);
				}
				else if(person->foreign_x<=0 && person->foreign_y>=0)
				{
					//second quad
					color [0] =((double)person->foreign_x/-foreign_max * color_foreign_west[0])+((double)person->foreign_y/foreign_max * color_foreign_north[0]);
					color [1] =((double)person->foreign_x/-foreign_max * color_foreign_west[1])+((double)person->foreign_y/foreign_max * color_foreign_north[1]);
					color [2] =((double)person->foreign_x/-foreign_max * color_foreign_west[2])+((double)person->foreign_y/foreign_max * color_foreign_north[2]);
				}
				else if(person->foreign_x<=0 && person->foreign_y<=0)
				{
					//third quad
					color [0] =((double)person->foreign_x/-foreign_max * color_foreign_west[0])+((double)person->foreign_y/-foreign_max * color_foreign_south[0]);
					color [1] =((double)person->foreign_x/-foreign_max * color_foreign_west[1])+((double)person->foreign_y/-foreign_max * color_foreign_south[1]);
					color [2] =((double)person->foreign_x/-foreign_max * color_foreign_west[2])+((double)person->foreign_y/-foreign_max * color_foreign_south[2]);
				}
				else
				{
					//fourth quadrant
					color [0] =((double)person->foreign_x/foreign_max * color_foreign_east[0])+((double)person->foreign_y/-foreign_max * color_foreign_south[0]);
					color [1] =((double)person->foreign_x/foreign_max * color_foreign_east[1])+((double)person->foreign_y/-foreign_max * color_foreign_south[1]);
					color [2] =((double)person->foreign_x/foreign_max * color_foreign_east[2])+((double)person->foreign_y/-foreign_max * color_foreign_south[2]);
				}
			}
			else if(ui_state == OCCUPATION)
			{
				if(person->occupation == FARMER)
				{
					color[0] = color_occupation_farmer[0];
					color[1] = color_occupation_farmer[1];
					color[2] = color_occupation_farmer[2];
				}
				else
				{
					color[0] = color_occupation_artisan[0];
					color[1] = color_occupation_artisan[1];
					color[2] = color_occupation_artisan[2];
				}
			}
			else //Generation
			{
				color [0] =((double)person->generation/ generation_youngest * color_generation[0]);
				color [1] =((double)person->generation/ generation_youngest * color_generation[1]);
				color [2] =((double)person->generation/ generation_youngest * color_generation[2]);
			}

			for (int c = 0; c < 3; c++)
			{
				if(color[c]>255)
				{
					color[c]=255;
				}
			}

			DrawBlade(person->position_x,person->position_y,color[0],color[1],color[2]);
		}
	}
};
void Game::DrawResources()
{
	for(std::vector<Resource*>::size_type i = 0; i != resources.size(); i++) 
	{
		DrawCluster(resources[i]->position_x,
			resources[i]->position_y,
			50+ (resources[i]->necessity_value/10*(double)color_resource[0]),
			50+ (resources[i]->necessity_value/10*(double)color_resource[1]),
			50+ (resources[i]->necessity_value/10*(double)color_resource[2]));
	}
};

void Game::DrawHouses()
{
	for (int y = 0; y < provinces_num_rows; y++)
	{
		for (int x = 0; x < provinces_num_columns; x++)
		{
			if(provinces[y][x]->homes_on_province.size()>0)
			{
				int position_x=provinces[y][x]->getCenter().x;
				int position_y=provinces[y][x]->getCenter().y;

				int pop = provinces[y][x]->homes_on_province.size();

				if(pop>500000000000)
				{
					DrawMegalopolis(position_x,position_y);
				}
				else if(pop>400000)
				{
					DrawConurbation(position_x,position_y);
				}
				else if(pop>300000)
				{
					DrawMetropolis(position_x,y);
				}
				else if(pop>30000)
				{
					DrawCity(position_x,position_y);
				}
				else if(pop>2000)
				{
					DrawTown(position_x,position_y);
				}
				else if(pop>100)
				{
					DrawVillage(position_x,position_y);
				}
				else if(pop>10)
				{
					DrawHamlet(position_x,position_y);
				}
				else if(pop>0)
				{
					DrawHouse(position_x,position_y);
				}
				else//Nothing
				{
					return;
				}
			}
		}
	}

	//Draw every house
	/*for(std::vector<House*>::size_type i = 0; i < houses.size(); i++) 
	{
	int position_x=houses[i]->position_x;
	int position_y=houses[i]->position_y;

	DrawHouse(position_x,position_y);
	}*/
};
void Game::DrawHouse(int x, int y)
{
	//Shadow border
	al_draw_line(x-3, y+1, x+3, y+1, al_map_rgb(0,0,0),2);

	ALLEGRO_VERTEX vertices[] = 
	{
		{x-3, y, 0},
		{x-3, y-4, 0},
		{x+3, y-4,0},
		{x+3, y,0},
	};
	for (int i = 0; i < 4; i++)
	{
		vertices[i].color = al_map_rgb(color_house[0],color_house[1],color_house[2]);
	}
	al_draw_prim(vertices, NULL, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN );

	//Walls
	//al_draw_line(x-1, y,   x-1, y-2, al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
	//al_draw_line(x-1, y-2, x+1, y-2, al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
	//al_draw_line(x+1, y-2, x+1, y,   al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
	//al_draw_line(x-1, y,   x+1, y,   al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
};
void Game::DrawHamlet(int x, int y)
{
	DrawHouse(x-5,y);
	DrawHouse(x+5,y);
};
void Game::DrawVillage(int x, int y)
{
	DrawHouse(x-5,y+2);
	DrawHouse(x+5,y+2);
	DrawHouse(x,y-1);
};
void Game::DrawTown(int x, int y)
{
	DrawHouse(x-5,y+5);
	DrawHouse(x+5,y+5);
	DrawHouse(x+5,y-5);
	DrawHouse(x-5,y-5);
};
void Game::DrawCity(int x, int y)
{
	DrawHouse(x,y);
	DrawHouse(x-5,y+5);
	DrawHouse(x+5,y+5);
	DrawHouse(x+5,y-5);
	DrawHouse(x-5,y-5);
};
void Game::DrawMetropolis(int x, int y)
{
	DrawHouse(x,y);
	DrawHouse(x-5,y+5);
	DrawHouse(x+5,y+5);
	DrawHouse(x+5,y-5);
	DrawHouse(x-5,y-5);

	DrawHouse(x+10,y);
	DrawHouse(x-10,y);
};
void Game::DrawConurbation(int x, int y)
{
	DrawHouse(x,y);
	DrawHouse(x-5,y+5);
	DrawHouse(x+5,y+5);
	DrawHouse(x+5,y-5);
	DrawHouse(x-5,y-5);

	DrawHouse(x+10,y);
	DrawHouse(x-10,y);

	DrawHouse(x,y+10);
	DrawHouse(x,y-10);
};
void Game::DrawMegalopolis(int x, int y)
{
	DrawHouse(x,y);
	DrawHouse(x-5,y+5);
	DrawHouse(x+5,y+5);
	DrawHouse(x+5,y-5);
	DrawHouse(x-5,y-5);

	DrawHouse(x+10,y);
	DrawHouse(x-10,y);

	DrawHouse(x,y+10);
	DrawHouse(x,y-10);

	DrawHouse(x+10,y+10);
	DrawHouse(x-10,y-10);

	DrawHouse(x+10,y-10);
	DrawHouse(x-10,y+10);
};

void Game::DrawCluster(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_line(x-2,y+2,x+2,y+2,al_map_rgb(0,0,0),1);

	al_draw_line(x-2,y,x+2,y,al_map_rgb(r,g,b),1);
	al_draw_pixel(x,y-1,al_map_rgb(r,g,b));
};
void Game::DrawBlade(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_line(x,y+1,x,y-4,al_map_rgb(0,0,0),1);//Shadow
	al_draw_line(x,y-1,x,y-4,al_map_rgb(r,g,b),2);//Actual
};
void Game::DrawDate()
{
	std::string string_date = "Year: " + std::to_string(current_year) + " Day: " + std::to_string(current_day) + " Hour: " + std::to_string(current_hour);
	const char * date = string_date.c_str();
	al_draw_text(arial16,al_map_rgb(color_text[0],color_text[1],color_text[1]), (screen_width/2)-80, 0, 0, date);
};
void Game::DrawPopulation()
{
	std::string string_population= "Population: " + std::to_string(people.size());
	const char * population = string_population.c_str();
	al_draw_text(arial16,al_map_rgb(color_text[0],color_text[1],color_text[1]), (screen_width/2)-80, 20, 0, population);
};

void Game::FreeMemory()
{
	al_destroy_font(arial24);
	al_destroy_font(arial16);
	al_destroy_font(arial12);

	for(std::vector<Person*>::size_type i = 0; i != people.size(); i++) 
	{
		delete people[i];
	}

	for(std::vector<Resource*>::size_type i = 0; i != resources.size(); i++) 
	{
		delete resources[i];
	}
	for(std::vector<Province*>::size_type i = 0; i != provinces.size(); i++) 
	{
		for(std::vector<Province*>::size_type x = 0; x != provinces[i].size(); x++) 
		{
			delete provinces[i][x];
		}
	}
};