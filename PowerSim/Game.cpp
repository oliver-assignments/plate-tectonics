
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

	province_width = 20;
	province_height = 20;
	province_jiggle_width = (int)(province_width*0.8);
	province_jiggle_height = (int)(province_height*0.8);

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
	ui_state = FOREIGN;
	resources_drawn = true;

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
	hunger_seek_level = hunger_death_level/20;

	foreign_max=100;

	generation_youngest=1;

	CreateWorld();

};

void Game::CreateWorld()
{
	CreateProvinces();
	CreateResources(30);
	CreatePeople(20,25,75);

	generation_youngest=1;
	power_highest_person = people[0];
	strength_highest_person = people[0];
	intelligence_highest_person = people[0];
}
void Game::CreateProvinces()
{
	provinces_num_columns = (int)(screen_width/province_width)+2;
	provinces_num_rows= (int)(screen_height/province_height)+2;

	std::vector<Vector2*> vertices;
	for (int i = 0; i < ((provinces_num_columns+1)*(provinces_num_rows+1)); i++)
	{
		vertices.push_back(NULL);
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
			if(vertices[index]!=NULL)
			{
				top_left = vertices[index];
			}
			else
			{
				top_left = new Vector2((x*province_width) , (y*province_height));
				vertices[index] = top_left;
			}

			//TOP RIGHT
			index = (y*(provinces_num_columns+1))+(x+1);
			if(vertices[index]!=NULL)
			{
				top_right = vertices[index];
			}
			else
			{
				top_right = new Vector2((x*province_width)+province_width, y*province_height);
				vertices[index] = top_right;
			}

			//BOTTOM LEFT
			index = ((y+1)*(provinces_num_columns+1))+x;
			if(vertices[index]!=NULL)
			{
				bottom_left = vertices[index];
			}
			else
			{
				bottom_left = new Vector2((x*province_width), (y*province_height) + province_height);
				vertices[index] = bottom_left;
			}

			//BOTTOM RIGHT
			index = ((y+1)*(provinces_num_columns+1))+(x+1);
			if(vertices[index]!=NULL)
			{
				bottom_right = vertices[index];
			}
			else
			{
				bottom_right = new Vector2((x*province_width)+province_width, (y*province_height) + province_height);
				vertices[index] = bottom_right;
			}
			Province* province = new Province(top_left,top_right,bottom_left,bottom_right);

			provinces[y].push_back(province);
		}
	}

	//Jiggle
	for(std::vector<Vector2*>::size_type v = 0; v != vertices.size(); v++) 
	{
		vertices[v]->x = vertices[v]->x + 5 - (province_jiggle_width/2) + (rand()%province_jiggle_width);
		vertices[v]->y = vertices[v]->y + 5 - (province_jiggle_height/2) + (rand()%province_jiggle_height);

	}
};
void Game::CreateResources(int myNumber)
{
	for (int i = 0; i < myNumber; i++)
	{
		int resource_province_x = rand()%provinces_num_columns;
		int resource_province_y = rand()%provinces_num_rows;

		//Province location
		resources.push_back(new Resource(resource_province_x,resource_province_y,
			provinces[resource_province_y][resource_province_x]->getCenter().x -5 + rand()%10,
			provinces[resource_province_y][resource_province_x]->getCenter().y -5 + rand()%10,10));

		//Random location
		//resources.push_back(new Resource(100+(rand()%(screen_width-200)),100+(rand()%(screen_height-200)),10));
	}
};
void Game::CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius)
{
	srand(time(NULL));
	for(int c = 0 ; c<myNumberClusters;c++)
	{
		//Anyway spawn
		/*int cluster_origin_x = 100+(rand()%(screen_width-200));
		int cluster_origin_y = 100+(rand()%(screen_height-200));

		int foreign_origin_x = cos(c*(2*3.14)/myNumberClusters) * myForeignRadius;
		int foreign_origin_y = sin(c*(2*3.14)/myNumberClusters) * myForeignRadius;

		for(int p = 0 ; p < myPeoplePerCluster;p++)
		{
		Person* person = new Person (10+(1+rand()%50),3+(1+rand()%50),1,-10+foreign_origin_x+(rand()%20),-10+foreign_origin_y+(rand()%20),cluster_origin_x+(-50+(rand()%100)),cluster_origin_y+(-50+(rand()%100)));
		people.push_back(person);
		}*/

		//Province Spawn
		int cluster_origin_province_x = rand()%provinces_num_columns;
		int cluster_origin_province_y = rand()%provinces_num_rows;

		int foreign_origin_x = cos(c*(2*3.14)/myNumberClusters) * myForeignRadius;
		int foreign_origin_y = sin(c*(2*3.14)/myNumberClusters) * myForeignRadius;

		for(int p = 0 ; p < myPeoplePerCluster;p++)
		{
			int province_x = cluster_origin_province_x;
			int province_y = cluster_origin_province_y;

			//int province_area = provinces[province_y][province_x]->getArea();
			Vector2 province_center = provinces[province_y][province_x]->getCenter();

			Person* person = new Person(10+(1+rand()%50),
				3+(1+rand()%50),
				1,
				-10+foreign_origin_x+(rand()%20), 
				-10+foreign_origin_y+(rand()%20),
				province_x, 
				province_y,
				province_center.x -(province_width/2) + rand()%province_width,
				province_center.y -(province_height/2) + rand()%province_height);
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
	color_base_value = 0.5;

	color_resource[0] = 200;color_resource[1] = 200;color_resource[2] = 200;
	color_province[0] = 20;color_province[1] = 20;color_province[2] = 20;

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

		Draw();

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
		Draw();
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_I))
	{
		ui_state = INTELLIGENCE;
		Draw();
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_S))
	{
		ui_state = STRENGTH;
		Draw();
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_H))
	{
		ui_state = HUNGER;
		Draw();
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_F))
	{
		ui_state = FOREIGN;
		Draw();
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_G))
	{
		ui_state = GENERATION;
		Draw();
	}
	if(al_key_down(&new_keyboard_state,ALLEGRO_KEY_R))
	{
		if(!al_key_down(&old_keyboard_state,ALLEGRO_KEY_R))
		{
			Draw();
			if(resources_drawn)
				resources_drawn=false;
			else
				resources_drawn = true;
		}
	}

	old_keyboard_state = new_keyboard_state;
};

void Game::RunTime()
{
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

void Game::ProcessPeople()
{
	int starting_index = rand()%people.size();

	for(std::vector<Person*>::size_type i = starting_index; i < people.size(); i++) 
	{
		ProcessPersonAI(people[i]);

		if(i==people.size()-1)
		{
			for (int o = 0; o < starting_index; o++)
			{
				ProcessPersonAI(people[i]);
			}
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


	if(person->hunger<hunger_seek_level)
	{
		//Look for food
	}
	else
	{

	}
};

void Game::Draw()
{
	DrawProvinces();
	DrawResources();
	DrawPeople();
	if(resources_drawn)
		DrawResources();

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
			al_draw_line(province->p0->x,province->p0->y,province->p1->x,province->p1->y,
				al_map_rgb(color_province[0],color_province[1],color_province[2]),1);
			al_draw_line(province->p1->x,province->p1->y,province->p3->x,province->p3->y,
				al_map_rgb(color_province[0],color_province[1],color_province[2]),1);
			al_draw_line(province->p3->x,province->p3->y,province->p2->x,province->p2->y,
				al_map_rgb(color_province[0],color_province[1],color_province[2]),1);
			al_draw_line(province->p2->x,province->p2->y,province->p0->x,province->p0->y,
				al_map_rgb(color_province[0],color_province[1],color_province[2]),1);

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
					color [0] = color_intelligence[0]*(double)people[i]->intelligence/intelligence_highest_current;
					color [1] = color_intelligence[1] *(double)people[i]->intelligence/intelligence_highest_current;
					color [2] = color_intelligence[2] *(double)people[i]->intelligence/intelligence_highest_current;
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

void Game::DrawCluster(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_line(x-2,y,x+2,y,al_map_rgb(r,g,b),1);
	al_draw_pixel(x,y-1,al_map_rgb(r,g,b));
};
void Game::DrawBlade(int x, int y, unsigned char r,unsigned char g,unsigned char b)
{
	al_draw_line(x,y,x,y-3,al_map_rgb(r,g,b),1);

};

void Game::FreeMemory()
{
	for(std::vector<Person*>::size_type i = 0; i != people.size(); i++) 
	{
		delete people[i];
	}
};