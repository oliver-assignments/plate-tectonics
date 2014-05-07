
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

	ui_state = FOREIGN;
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
	CreateProvinces();
	CreateResources(30);
	CreatePeople(5,200,75);

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
			Province* province = new Province(province_id,top_left,top_right,bottom_right,bottom_left,70+ rand()%(30));
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

			//5int province_area = provinces[province_y][province_x]->getArea();
			Vector2 province_center = provinces[province_y][province_x]->getCenter();

			Person* person = new Person(player_id,
				10+(1+rand()%50),
				3+(1+rand()%50),
				1,
				-10+foreign_origin_x+(rand()%20), 
				-10+foreign_origin_y+(rand()%20),
				province_x, 
				province_y,
				province_center.x -(province_width/2) + rand()%province_width,
				province_center.y -(province_height/2) + rand()%province_height);

			player_id++;

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
	color_house[0] = 200;color_house[1] = 200;color_house[2] = 200;
	color_province[0] = 0;color_province[1] =100;color_province[2] = 0;

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
		if(i>=0 && i <people.size()){
			//Time of the day people get hungry

			if(people[i]->hunger>=hunger_death_level)
			{
				people[i]->dead = true;
				delete(people[i]);
				people.erase(people.begin()+i);
				i--;
				continue;
			}

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
		SeekFood(person);
	}
	else
	{

		BuildResources(person);
	}

	//if(person->hunger<hunger_seek_level)
	//{
	//	//Look for food
	//}
	//else
	//{

	//}
};
void Game::SeekFood(Person* person)
{
	//Currently eating -whatever amount- takes an hour

	if(person->food_carried>0)//
	{
		//Eat all your food
		int amount_eaten = person->food_carried-person->hunger;
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
	//First off: do you have a home?
	if(person->home ==NULL)
	{
		int province_x = person->province_x;
		int province_y = person->province_y;

		//Calculating the power buy in for the home
		int province_total_power = 0;
		if(provinces[province_y][province_x]->homes_on_province.size()!=0)
		{
			for (int h = 0; h < provinces[province_y][province_x]->homes_on_province.size(); h++)
			{
				province_total_power += provinces[province_y][province_x]->homes_on_province[h]->owner->power;
			}
			province_total_power = province_total_power/provinces[province_y][province_x]->homes_on_province.size();
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
};
void Game::BuildHome(Person* person)
{
	House* home = new House(house_id,person,person->province_x,person->province_y,
		provinces[person->province_y][person->province_x]->getCenter().x-((province_width)/2)+(rand()% (province_width)),
		provinces[person->province_y][person->province_x]->getCenter().y-((province_height)/2)+(rand()%(province_height)));

	person->home = home;
	houses.push_back(home);
	provinces[person->province_y][person->province_x]->homes_on_province[house_id] = home;
};
void Game::SeekInteraction(Person* person)
{

};
void Game::SeekDominion(Person* person)
{

};
void Game::MoveRandomDirection(Person* person)
{
	int direction = rand()%20;

	switch (direction)
	{
	case 0 :
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
		break;
	case 1 :
		person->province_x = person->province_x+1;
		if(person->province_x<provinces_num_columns)
		{
			UpdatePersonPositionToProvince(person);

		}
		else
		{
			person->province_x = person->province_x-1; 
		}

		break;
	case 2 :
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
		break;
	case 3 :
		//Left
		person->province_x = person->province_x-1;
		if(person->province_x>=0)
		{
			UpdatePersonPositionToProvince(person);
		}
		else
		{
			person->province_x = person->province_x+1; 
		}

		break;
	default:
		break;
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

			switch (province->biome)
			{
			case GRASSLAND:
				province->food_in_province=hunger_seek_level*10;
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

					int amountTaken = hunger_seek_level + ((next_in_line->strength*next_in_line->power)/hunger_seek_level);

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
					MoveRandomDirection(line[p]);
				}
			}
			prov->people_in_line.clear();
		}
		provinces_with_hungry_people.clear();
	}
};


void Game::UpdatePersonPositionToProvince(Person* person)
{
	Vector2 province_center = provinces[person->province_y][person->province_x]->getCenter();
	person->position_x = province_center.x -(province_width/2) + rand()%province_width;
	person->position_y =province_center.y -(province_height/2) + rand()%province_height;
}
//void Game::UpdateProvinceFood()
//{
//
//}

void Game::Draw()
{
	if(provinces_drawn)
		DrawProvinces();

	DrawHouses();

	DrawPeople();

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

			//Provicne color Seperation
			color[0] = ((double)province->arability/arability_max) * color_province[0];
			color[1] = ((double)province->arability/arability_max) * color_province[1];
			color[2] = ((double)province->arability/arability_max) * color_province[2];

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
			else{
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
	int color[3];

	int average_arability = 0;

	//Top left vertex
	{
		int total_arability = 0;
		int provinces_used = 0;

		int bottomRightArability;
		int topLeftArability;
		int topRightArability;
		int bottomLeftArability;

		if(y-1>=0 && x-1>=0)//Topleft
		{
			bottomRightArability = provinces[y-1][x-1]->arability;
			total_arability+= bottomRightArability;
			provinces_used++;
		}

		if(y-1>=0)//Topright
		{
			topLeftArability = provinces[y-1][x]->arability;
			total_arability+=topLeftArability;
			provinces_used++;
		}
		if(true)//bottomright
		{
			topRightArability = provinces[y][x]->arability;
			total_arability+= topRightArability;
			provinces_used++;
		}

		if(x-1>=0)//bottomleft
		{
			bottomLeftArability = provinces[y][x-1]->arability;
			total_arability+= bottomLeftArability;
			provinces_used++;
		}
		average_arability = total_arability/provinces_used;

		//Top left vertex
		color[0] = ((double)average_arability/arability_max) * color_province[0];
		color[1] = ((double)average_arability/arability_max) * color_province[1];
		color[2] = ((double)average_arability/arability_max) * color_province[2];

		myVertices[0].color = al_map_rgb(color[0],color[1],color[2]);
	}
	//Top right vertex
	{
		int total_arability = 0;
		int provinces_used = 0;

		int bottomRightArability;
		int topLeftArability;
		int topRightArability;
		int bottomLeftArability;

		if(y-1>=0)//Topleft
		{
			bottomRightArability = provinces[y-1][x]->arability;
			total_arability+= bottomRightArability;
			provinces_used++;
		}

		if(y-1>=0 && x+1<provinces_num_columns)//Topright
		{
			topLeftArability = provinces[y-1][x+1]->arability;
			total_arability+=topLeftArability;
			provinces_used++;
		}
		if(x+1<provinces_num_columns)//bottomright
		{
			topRightArability = provinces[y][x+1]->arability;
			total_arability+= topRightArability;
			provinces_used++;
		}

		if(true)//bottomleft
		{
			bottomLeftArability = provinces[y][x]->arability;
			total_arability+= bottomLeftArability;
			provinces_used++;
		}
		average_arability = total_arability/provinces_used;

		//Top right vertex
		color[0] = ((double)average_arability/arability_max) * color_province[0];
		color[1] = ((double)average_arability/arability_max) * color_province[1];
		color[2] = ((double)average_arability/arability_max) * color_province[2];

		myVertices[1].color = al_map_rgb(color[0],color[1],color[2]);
	}
	//Bottomright vertex
	{
		int total_arability = 0;
		int provinces_used = 0;

		int bottomRightArability;
		int topLeftArability;
		int topRightArability;
		int bottomLeftArability;

		if(true)//Topleft
		{
			bottomRightArability = provinces[y][x]->arability;
			total_arability+= bottomRightArability;
			provinces_used++;
		}

		if(x+1<provinces_num_columns)//Topright
		{
			topLeftArability = provinces[y][x+1]->arability;
			total_arability+=topLeftArability;
			provinces_used++;
		}
		if(x+1<provinces_num_columns&&y+1<provinces_num_rows)//bottomright
		{
			topRightArability = provinces[y+1][x+1]->arability;
			total_arability+= topRightArability;
			provinces_used++;
		}

		if(y+1<provinces_num_rows)//bottomleft
		{
			bottomLeftArability = provinces[y+1][x]->arability;
			total_arability+= bottomLeftArability;
			provinces_used++;
		}
		average_arability = total_arability/provinces_used;

		//Bottomright vertex
		color[0] = ((double)average_arability/arability_max) * color_province[0];
		color[1] = ((double)average_arability/arability_max) * color_province[1];
		color[2] = ((double)average_arability/arability_max) * color_province[2];

		myVertices[2].color = al_map_rgb(color[0],color[1],color[2]);
	}
	//Bottomleft vertex
	{
		int total_arability = 0;
		int provinces_used = 0;

		int bottomRightArability;
		int topLeftArability;
		int topRightArability;
		int bottomLeftArability;

		if(x-1>=0)//Topleft
		{
			bottomRightArability = provinces[y][x-1]->arability;
			total_arability+= bottomRightArability;
			provinces_used++;
		}

		if(true)//Topright
		{
			topLeftArability = provinces[y][x]->arability;
			total_arability+=topLeftArability;
			provinces_used++;
		}
		if(y+1<provinces_num_rows)//bottomright
		{
			topRightArability = provinces[y+1][x]->arability;
			total_arability+= topRightArability;
			provinces_used++;
		}

		if(y+1<provinces_num_rows && x-1>=0)//bottomleft
		{
			bottomLeftArability = provinces[y+1][x-1]->arability;
			total_arability+= bottomLeftArability;
			provinces_used++;
		}
		average_arability = total_arability/provinces_used;

		//Bottomleft vertex
		color[0] = ((double)average_arability/arability_max) * color_province[0];
		color[1] = ((double)average_arability/arability_max) * color_province[1];
		color[2] = ((double)average_arability/arability_max) * color_province[2];

		myVertices[3].color = al_map_rgb(color[0],color[1],color[2]);
	}

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
			else if(ui_state == OCCUPATION)
			{
				if(people[i]->occupation == FARMER)
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

				if(provinces[y][x]->homes_on_province.size()>500000000000)
				{
					DrawMegalopolis(position_x,position_y);
				}
				else if(provinces[y][x]->homes_on_province.size()>400000)
				{
					DrawConurbation(position_x,position_y);
				}
				else if(provinces[y][x]->homes_on_province.size()>300000)
				{
					DrawMetropolis(position_x,y);
				}
				else if(provinces[y][x]->homes_on_province.size()>30000)
				{
					DrawCity(position_x,position_y);
				}
				else if(provinces[y][x]->homes_on_province.size()>2000)
				{
					DrawTown(position_x,position_y);
				}
				else if(provinces[y][x]->homes_on_province.size()>100)
				{
					DrawVillage(position_x,position_y);
				}
				else if(provinces[y][x]->homes_on_province.size()>10)
				{
					DrawHamlet(position_x,position_y);
				}
				else if(provinces[y][x]->homes_on_province.size()>0)
				{
					DrawHouse(position_x,position_y);
				}
				else//Nothing
				{

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
	al_draw_line(x-2, y+1,   x+2, y+1, al_map_rgb(0,0,0),2);

	//Roof
	al_draw_line(x-1,y-3,x,y-6,al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
	al_draw_line(x,y-6,x+1, y-3,al_map_rgb(color_house[0],color_house[1],color_house[2]),1);

	//Walls
	al_draw_line(x-1, y,   x-1, y-3, al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
	al_draw_line(x-1, y-3, x+1, y-3, al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
	al_draw_line(x+1, y-3, x+1, y,   al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
	al_draw_line(x-1, y,   x+1, y,   al_map_rgb(color_house[0],color_house[1],color_house[2]),1);
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
	for(std::vector<House*>::size_type i = 0; i != houses.size(); i++) 
	{
		delete houses[i];
	}
	for(std::vector<Province*>::size_type i = 0; i != provinces.size(); i++) 
	{
		for(std::vector<Province*>::size_type x = 0; x != provinces[i].size(); x++) 
		{
			delete provinces[i][x];
		}
	}
};