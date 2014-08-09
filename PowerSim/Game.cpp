
#include "Game.h"
#include "tectonic.h"
#include "people.h"
#include "plants.h"
#include "animals.h"

#include <iostream>

//Game::Game()
//{
//	Initialize();
//};
//
//void Game::Initialize()
//{
//	srand(time(NULL));
//
//	screen_width = 1350;
//	screen_height = 690;
//
//	screen_game_width = screen_width;// - 250;
//	screen_game_height = screen_height;
//
//	
//
//	InitializeAllegro();
//	LoadContent();
//
//	InitializeVariables();
//
//	Update();
//};


//void Game::InitializeVariables()
//{	
//	province_width = 10;
//	province_height = 10;
//
//	province_jiggle_width = (int)(province_width*0.8);
//	province_jiggle_height = (int)(province_height*0.8);
//
//	province_jiggle = false;
//	province_borders_drawn = false;
//	province_height_drawn = false;
//
//	total_water = 0;
//	total_land= 0;
//
//	ui_state = POWER;
//	resources_drawn = true;
//	provinces_drawn = true;
//	draw_every_hour = true;
//
//	province_highest_altitude = 1000;
//	province_deepest_depth = 10;
//
//	//Setuping up our values
//	hours_in_day = 24;
//	days_in_year = 365;
//
//	current_hour=0;
//	current_day=0;
//	current_year=0;
//
//	power_highest_historical=0;
//	power_highest_current=0;
//	power_context = CURRENT;
//
//	strength_highest_historical =0;
//	strength_highest_current=0;
//	strength_context= CURRENT;
//
//	intelligence_highest_historical=0;
//	intelligence_highest_current=0;
//	intelligence_context = CURRENT;
//
//	hunger_death_level = 100;
//	hunger_seek_level = hunger_death_level/9;
//
//	foreign_max=100;
//
//	generation_youngest=1;
//
//	player_id = 0;
//	house_id = 0;
//	province_id=0;
//
//	generation_youngest=1;
//	power_highest_person = NULL;
//	strength_highest_person = NULL;
//	intelligence_highest_person = NULL;
//
//	CreateWorld();
//};

Context* Game::CreateWorld()
{
	Context* context = new Context();

	CreateProvinces();
	CreateContinents();

	TectonicHandler::InitializeContext(context);
	
	TectonicHandler::CreateWater();
	TectonicHandler::CreateTectonicPlates();

	TectonicHandler::ResolveAllWater();
	
	PlantHandler::InitializeContext(context);
	PlantHandler::CreatePlants();

	AnimalHandler::InitializeContext(context);
	AnimalHandler::CreateAnimals();
	
	PeopleHandler::InitializeHandler(context);
	//Note we may not want to do this before we move the plates along
	PeopleHandler::CreatePeople();
};
void Game::LoadWorld()
{
	//Load the world from the text file
	Context* context;
};

void Game::CreateProvinces()
{
	//36,794,240,000 acres on earth
	times_drawn = 0;
	world_width = (int)(screen_game_width/province_width);
	world_height = (int)(screen_height/province_height);

	int equator_position = (int)(world_height/2);

	for (int i = 0; i < ((world_width+1)*(world_height+1)); i++)
	{
		province_vertices.push_back(NULL);
	}

	for (int y = 0; y < world_height;y++)
	{
		std::vector<Province*> row;
		provinces.push_back(row);
		for (int x = 0; x < world_width; x++)
		{
			Vector2* top_left;
			Vector2* top_right;
			Vector2* bottom_left;
			Vector2* bottom_right;

			//TOP LEFT
			int index = (y*(world_width+1))+x;
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
			index = (y*(world_width+1))+(x+1);
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
			index = ((y+1)*(world_width+1))+x;
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
			index = ((y+1)*(world_width+1))+(x+1);
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

			province->altitude+= rand()%15;
			province->distance_from_equator = abs(equator_position-y);
			province_id++;

			provinces[y].push_back(province);
		}
	}

	//Jiggle
	if(province_jiggle){
		for(std::vector<Vector2*>::size_type v = 0; v != province_vertices.size(); v++) 
		{
			int x = v% (world_width+1);
			int y = v/ (world_width+1);
			if(x>0 && y>0 && x<world_width && y<world_height)
			{
				province_vertices[v]->x = province_vertices[v]->x + 5 - (province_jiggle_width/2) + (rand()%province_jiggle_width);
				province_vertices[v]->y = province_vertices[v]->y + 5 - (province_jiggle_height/2) + (rand()%province_jiggle_height);
			}
		}
	}
};

void Game::CreateContinents()
{
	for (int f = 0; f <5; f++)
	{
		//Center_of_continent
		int cluster_origin_province_x=0;
		int cluster_origin_province_y=0;
		while(cluster_origin_province_x ==0 ||cluster_origin_province_y ==0)
		{
			int attempted_x = (world_width/2)+(rand()%(world_width/3))-(world_width/6);
			int attempted_y = (world_height/2)+(rand()%(world_height/2))-(world_height/4);

			if(provinces[attempted_y][attempted_x]->biome!=GRASSLAND)
			{
				cluster_origin_province_x=attempted_x;
				cluster_origin_province_y=attempted_y;
			}
		}
		//How far we venture from the continent to place a blob
		int radius = ((world_width+world_height)/2)/8;

		for (int i = 0; i < 15; i++)
		{
			std::vector<Province*> grassland_blob = GetDiamondOfProvinces(
				cluster_origin_province_x - (radius/2) + (rand()%(radius)),
				cluster_origin_province_y - (radius/2) + (rand()%(radius)), 
				radius/((rand()%3)+1),
				true);

			for (int p = 0; p < grassland_blob.size(); p++)
			{	
				grassland_blob[p]->altitude += 50 + rand()%50;
				grassland_blob[p]->biome = (GRASSLAND);
			}
		}
	}
};
void TectonicHandler::CreateWater()
{
	int amount_water_needed = world_width*world_height*100;
	for (int w = 0; w < 100; w++)
	{
		Province* prov = NULL;
		while(prov == NULL)
		{
			int x = (rand()%world_width);
			int y = (rand()%world_height);
			if(provinces[y][x]->biome != GRASSLAND)
			{
				prov = provinces[y][x];
			}
		}
		prov->water_depth +=amount_water_needed/80;
		prov->biome = WATER;
		province_water_unresolved.push_back(prov);
	}
	ResolveAllWater();
};
void TectonicHandler::CreateTectonicPlates()
{
	int provinces_without_plate = world_height *world_width;
	int plate_count = 0;

	//Initializing the grid for tectonic plates
	for (int y = 0; y < world_height; y++)
	{
		std::vector<bool> row_of_taken_provinces;
		provinces_has_plate.push_back(row_of_taken_provinces);

		std::vector<int> row_of_altitude_changes;
		provinces_pending_altitude_changes.push_back(row_of_altitude_changes);

		std::vector<int> row_of_astehnosphere_heat;
		asthenosphere_heat_map.push_back(row_of_astehnosphere_heat);

		std::vector<std::vector<int>> row_of_list_of_plates_on_province;
		plates_on_province.push_back(row_of_list_of_plates_on_province);

		for (int x = 0; x < world_width; x++)
		{
			provinces_has_plate[y].push_back(false);
			provinces_pending_altitude_changes[y].push_back(0);
			asthenosphere_heat_map[y].push_back(0);
			plates_on_province[y].push_back(*new std::vector<int>);
		}
	}

	int radius = ((world_width+world_height)/2)/4;
	int provinces_scanned =0;

	//Creating every plate
	while(provinces_without_plate>0)
	{
		TectonicPlate* tectonic_plate = new TectonicPlate();
		tectonic_plate->plate_number = plate_count;
		plate_count++;

		//Finding a good blob origin that isnt already taken
		int cluster_origin_province_x=rand()%world_width;
		int cluster_origin_province_y=rand()%world_height;

		while(cluster_origin_province_x==-1 && cluster_origin_province_y==-1)
		{
			int attempted_x = rand()%world_width;
			int attempted_y = rand()%world_height;

			//This hasnt been taken, go ahead and use it as your center
			if(provinces_has_plate[attempted_y][attempted_x] == false)
			{
				cluster_origin_province_x = attempted_x;
				cluster_origin_province_y = attempted_y;
			}
		}

		//Creating # of smaller diamonds near our center
		for (int q = 0; q < 60; q++)
		{
			//Wrap the x
			int piece_origin_x = cluster_origin_province_x - (radius) + (rand()%(radius*2));
			if(piece_origin_x<0)
				piece_origin_x+=world_width;
			if(piece_origin_x>=world_width)
				piece_origin_x-=world_width;

			//But not the y
			int piece_origin_y = cluster_origin_province_y - (radius/2) + (rand()%radius);
			if(piece_origin_y<0)
				continue;
			if(piece_origin_y>=world_height)
				continue;

			//A mildy random piece size
			int piece_radius = (radius/4) -(radius/15/2) + (rand()%(radius/15));

			//The coordinates of the piece
			std::vector<Province*> piece = GetDiamondOfProvinces(piece_origin_x,piece_origin_y,piece_radius,true);

			//Making sure this province is not taken by another plate or itself
			for (int m = 0; m < piece.size(); m++)
			{
				if(provinces_has_plate[piece[m]->province_y][piece[m]->province_x] == false)
				{
					//Add it to our plate!
					tectonic_plate->provinces_in_plate.push_back(new Vector2(piece[m]->province_x,piece[m]->province_y));

					//A province has been taken
					provinces_has_plate[piece[m]->province_y][piece[m]->province_x] = true;
					provinces_without_plate--;

					//Draw what we have taken!
					if(provinces_scanned == 50)
					{
						Draw();
						provinces_scanned = 0;
					}
					provinces_scanned++;
				}
			}
		}
		tectonic_plates.push_back(tectonic_plate);
	}

	Draw();

	/*//If a plate is too small we mesh it with other plates
	for (int t = 0; t < tectonic_plates.size(); t++)
	{
	if(tectonic_plates[t]->provinces_in_plate.size() < 10 && tectonic_plates[t]->provinces_in_plate.size() != 0)
	{
	for (int p = 0; p < tectonic_plates[t]->provinces_in_plate.size(); p++)
	{
	//All the neighboring provinces we could give this peice to
	std::vector<Province*> neighboring = GetSquareOfProvinces(tectonic_plates[t]->provinces_in_plate[p]->x,
	tectonic_plates[t]->provinces_in_plate[p]->y,
	1, false,true,false);

	//We try this neighbor
	int attempted_neighbor_index= NULL;
	while(attempted_neighbor_index==NULL)
	{
	int attempted_index= rand()%neighboring.size();
	std::vector<int> plates_on_this_province = plates_on_province[neighboring[attempted_neighbor_index]->province_y]
	[neighboring[attempted_neighbor_index]->province_x];

	if(plates_on_this_province.size() >1)
	{
	attempted_neighbor_index = attempted_index;
	}
	else if(plates_on_this_province.size() ==1)
	{
	if(plates_on_this_province[0] != t)
	{
	attempted_neighbor_index = attempted_index;
	}
	}
	}

	//The plates on the neighboring province
	std::vector<int> plates_on_this_province = plates_on_province[neighboring[attempted_neighbor_index]->province_y]
	[neighboring[attempted_neighbor_index]->province_x];

	//Add this province coordinate to our neighbors list
	int recipient_plate = -9999;
	while(recipient_plate == -9999)
	{
	int attempted_index = rand()%plates_on_this_province.size();
	if(attempted_index!=t)
	{
	recipient_plate = attempted_index;
	}
	}

	tectonic_plates[recipient_plate]->provinces_in_plate.push_back(
	new Vector2(tectonic_plates[t]->provinces_in_plate[p]->x,
	tectonic_plates[t]->provinces_in_plate[p]->y));
	Draw();
	}
	//Effectively delte this plate
	tectonic_plates[t]->provinces_in_plate.clear();
	}
	}*/
};
void Game::CreateRivers()
{
	int number_of_rivers = 0;
	for (int r = 0; r <number_of_rivers; r++)
	{
		int origin_province_x = rand()%world_width;
		int origin_province_y = rand()%world_height;

		if(provinces[origin_province_y][origin_province_x]->biome!=WATER)
		{
			std::vector<Vector2*> river;

			//First segment
			Province* current_prov = provinces[origin_province_y][origin_province_x];

			river.push_back(current_prov->p0);

			//Making other segments
			int river_length = 3+rand()%15;
			for (int l = 0; l < river_length; l++)
			{
				Province* old_prov = current_prov;

				while(old_prov == current_prov)
				{
					std::vector<Province*> surrounding = GetDiamondOfProvinces(current_prov->province_x,current_prov->province_y,1,true);

					int next_prov = rand()%4;
					switch (next_prov)
					{
					case 0:
						river.push_back(surrounding[1]->p0);
						current_prov = surrounding[1];
						break;
					case 1:
						river.push_back(surrounding[2]->p0);
						current_prov = surrounding[2];
						break;
					case 2:
						river.push_back(surrounding[3]->p0);
						current_prov = surrounding[3];
						break;
					case 3:
						river.push_back(surrounding[4]->p0);
						current_prov = surrounding[4];
						break;
					}

					for (int p = 0; p < river.size()-1; p++)
					{
						if(river[river.size()-1] == river[p])
						{
							//We already have this point, undo the last point
							river.pop_back();
							current_prov = old_prov;
						}
					}
					if(current_prov->biome == WATER)
					{
						river.pop_back();
						l = river_length;
					}
				}
			}
			rivers_points.push_back(river);
		}
		else
		{
			r--;
		}
	}
};

void Game::CreatePlants()
{
	for (int i = 0; i < 10; i++)
	{
		int plant_province_x = rand()%world_width;
		int plant_province_y = rand()%world_height;



		if(provinces[plant_province_y][plant_province_x]->biome != WATER)
		{
			Province* prov = provinces[plant_province_y][plant_province_x];
			Plant* plant = new Plant(CreateName(5), prov,5+(rand()%20),12,10);

			prov->plants_on_province[plant->GetID()] = plant;
		}
		else
		{
			i--;
		}
	}
};
void AnimalHandler::CreateAnimals()
{

};
void PeopleHandler::CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius)
{
	for(int c = 0 ; c<myNumberClusters;c++)
	{
		//Province Spawn
		int cluster_origin_province_x =0;
		int cluster_origin_province_y =0;
		do
		{
			cluster_origin_province_x = rand()%world_width;
			cluster_origin_province_y = rand()%world_height;
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

std::string Game::CreateName(int myNumberLetters)
{
	std::string name ("");
	for (int i = 0; i < myNumberLetters; i++)
	{
		if(i%2==0)
		{
			int index = rand()%17;
			name+=consonants[index];
			if(consonants[index] == "q")
			{
				name+="u";
			}
		}
		else
		{
			name+=vowels[rand()%6];
		}
	}
	return name;
};

//This wraps by x but not y
std::vector<Province*> Game::GetSquareOfProvinces(int province_x, int province_y, int radius,bool doGetCenter)
{
	std::vector<Province*> square;

	for (int x = province_x-radius; x <= province_x+radius; x++)
	{
		for (int y = province_y-radius; y <= province_y+radius; y++)
		{
			if(x == province_x && y == province_y )
			{
				if(doGetCenter)
					square.push_back(provinces[y][x]);
			}
			else
			{
				int wrapped_x = x;
				int wrapped_y = y;

				WrapCoordinates(&wrapped_x,&wrapped_y);

				square.push_back(provinces[wrapped_y][wrapped_x]);
			}
		}
	}

	return square;
};
std::vector<Province*> Game::GetDiamondOfProvinces(int province_x, int province_y, int radius, bool doGetCenter)
{
	std::vector<Province*> blob;

	//Center
	if(doGetCenter)
	{
		if(province_y >=0 && province_y < world_height && province_x >=0 && province_x < world_width)
		{ 
			blob.push_back(provinces[province_y][province_x]);
		}
	}

	//Making circles
	int location_x = 0;
	int location_y = 0;
	for (int r = radius; r > 0; r--)
	{
		location_x = province_x;
		location_y = province_y-r;

		while(location_x != province_x+r)
		{
			location_x++;
			location_y++;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;



			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);


		}

		while(location_x != province_x)
		{
			location_x--;
			location_y++;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;

			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);

		}

		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;

			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);


		}

		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;

			int wrapped_location_x = location_x;
			int wrapped_location_y = location_y;

			WrapCoordinates(&wrapped_location_x,&wrapped_location_y);

			blob.push_back(provinces[wrapped_location_y][wrapped_location_x]);
		}
	}

	return blob;
};
std::vector<Vector2*> Game::GetBlobOfCoordinates(int province_x, int province_y, int radius)
{
	std::vector<Vector2*> blob;

	//Center
	if(province_y >=0 && province_y < world_height && province_x >=0 && province_x < world_width)
	{ 
		blob.push_back(new Vector2(province_x,province_y));
	}

	//Axis
	for (int r = 1; r <= radius; r++)
	{
		if(province_y-r >=0 && province_y-r < world_height)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=world_width;}
			if(province_x >= world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y-r));
		}
		if(province_y >=0 && province_y < world_height)
		{ 
			int wrapped_x = province_x+r;
			if(province_x+r <0){wrapped_x+=world_width;}
			if(province_x+r >= world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y));
		}
		if(province_y+r >=0 && province_y+r < world_height)
		{
			int wrapped_x = province_x;
			if(province_x <0){wrapped_x+=world_width;}
			if(province_x >= world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y+r));
		}
		if(province_y >=0 && province_y < world_height)
		{ 
			int wrapped_x = province_x-r;
			if(province_x-r <0){wrapped_x+=world_width;}
			if(province_x-r >= world_width){wrapped_x-=world_width;}

			blob.push_back(new Vector2(wrapped_x,province_y));
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
			if(location_y >=0 && location_y < world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >= world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_x != province_x)
		{
			location_x--;
			location_y++;
			if(location_y >=0 && location_y < world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >= world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_x != province_x-r)
		{
			location_x--;
			location_y--;
			if(location_y >=0 && location_y < world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >= world_width){wrapped_location-=world_width;}

				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
		while(location_y != province_y-r)
		{
			location_x++;
			location_y--;
			if(location_y >=0 && location_y < world_height)
			{
				int wrapped_location = location_x;
				if(location_x <0){wrapped_location+=world_width;}
				if(location_x >= world_width){wrapped_location-=world_width;}
				blob.push_back(new Vector2(wrapped_location,location_y));
			}
		}
	}

	return blob;
}


void Game::DefineColors()
{
	color_base_value = 0.5;

	color_text[0] = 255;color_text[1] = 255;color_text[2] = 255;

	color_resource[0] = 200;color_resource[1] = 200;color_resource[2] = 200;
	color_house[0] = 150;color_house[1] = 150;color_house[2] = 150;

	color_grassland[0] = 75;color_grassland[1] =150;color_grassland[2] = 60;
	color_jungle[0] = 130;color_jungle[1] =140;color_jungle[2] = 70;
	color_desert[0] = 150;color_desert[1] =150;color_desert[2] = 10;
	color_water[0] = 0;color_water[1] =0;color_water[2] = 100;
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
				RunHumans();
			}
			accumulator -= dt;
		}


	}
	FreeMemory();
};

void Game::UpdateDeepestWater()
{
	province_deepest_depth=0;
	for(std::vector<std::vector<Province*>>::size_type y = 0; y < world_height; y++) 
	{
		for(std::vector<Province*>::size_type x = 0; x < world_width; x++) 
		{
			Province* province = provinces[y][x];

			if(province_deepest_depth<province->water_depth)
				province_deepest_depth = province->water_depth;
		}
	}
};
void Game::UpdateHighestMountain()
{
	province_highest_altitude = 0;

	for(std::vector<std::vector<Province*>>::size_type y = 0; y < world_height; y++) 
	{
		for(std::vector<Province*>::size_type x = 0; x < world_width; x++) 
		{
			Province* province = provinces[y][x];

			if(province_highest_altitude<province->getLandAndWaterHeight())
				province_highest_altitude =province->getLandAndWaterHeight();
		}
	}
};

void TectonicHandler::RunTectonics()
{
	//Advancing time
	current_year+=10000;

	//Changing what we are viewing, plate or land
	if(currentMapMode == PLATE_TECTONICS)
	{
		currentMapMode = TERRAIN;
		screen_game_width = screen_width;// - 250;
	}
	else
	{
		currentMapMode = PLATE_TECTONICS;
		screen_game_width = screen_width - 250;
	}

	//Clear our tect plate lists of old changes
	for (int y = 0; y < world_height; y++)
	{
		for (int x = 0; x < world_width; x++)
		{
			int heat = provinces[y][x]->altitude - provinces[y][x]->water_depth;
			if(heat >0)
				asthenosphere_heat_map[y][x] += heat;

			plates_on_province[y][x].clear();
			provinces_pending_altitude_changes[y][x] = 0;
		}
	}

	//Moving the plates, not we only make a new array of the supposed positons we dont ACTUALYL move them
	for (int t = 0; t < tectonic_plates.size(); t++)
	{
		//If it isn't moving get it a new direction
		if(tectonic_plates[t]->x_velocity == -9999)
		{
			tectonic_plates[t]->x_velocity = -1 + rand()%3;
			tectonic_plates[t]->y_velocity = -1 + rand()%3;
		}

		//Populating array of plates on each province
		for (int p = 0; p < tectonic_plates[t]->provinces_in_plate.size(); p++)
		{
			Province* province = provinces[tectonic_plates[t]->provinces_in_plate[p]->y][tectonic_plates[t]->provinces_in_plate[p]->x];

			int wrapped_x = tectonic_plates[t]->provinces_in_plate[p]->x+tectonic_plates[t]->x_velocity;
			int wrapped_y = tectonic_plates[t]->provinces_in_plate[p]->y+tectonic_plates[t]->y_velocity;

			WrapCoordinates(&wrapped_x,&wrapped_y);

			plates_on_province[wrapped_y][wrapped_x].push_back(tectonic_plates[t]->plate_number);
		}
	}

	//Conflict resolve - There was never a peaceful solution
	for (int y = 0; y < world_height; y++)
	{
		for (int x = 0; x < world_width; x++)
		{
			//The plates that are on this province
			std::vector<int> tectonic_plate_conflicts = plates_on_province[y][x];

			if(tectonic_plate_conflicts.size() >1)//If there is indeed a conflict
			{
				//The provinces that tried ot move onto this province
				std::vector<Province*> province_conflicts;

				//Pulling the old provinces by backtracking so we can compare
				for (int c = 0; c < tectonic_plate_conflicts.size(); c++)
				{
					int wrapped_x = x-tectonic_plates[tectonic_plate_conflicts[c]]->x_velocity;
					int wrapped_y = y-tectonic_plates[tectonic_plate_conflicts[c]]->y_velocity;
					WrapCoordinates(&wrapped_x,&wrapped_y);

					Province* old_plate_province_pos = provinces[wrapped_y][wrapped_x];
					province_conflicts.push_back(old_plate_province_pos);
				}

				int highest_density = INT_MIN;
				int highest_value_index= - 9990;

				for (int c = 0; c < province_conflicts.size(); c++)
				{
					//Determining which plate will not get subducted and destroyed
					int plate_density = CalculatePlateDensity(tectonic_plates[c]);
					if(plate_density >= highest_density)
					{
						highest_density = plate_density;
						highest_value_index = tectonic_plate_conflicts[c];
					}
				}

				//Highest one doesnt get deleted
				plates_on_province[y][x].clear();
				plates_on_province[y][x].push_back(highest_value_index);
			}
			if(plates_on_province[y][x].size() !=0)
			{
				//Now move the altitude from the chosen plate
				Vector2* old_position =new Vector2(
					x - tectonic_plates[plates_on_province[y][x][0]]->x_velocity,
					y - tectonic_plates[plates_on_province[y][x][0]]->y_velocity);
				WrapCoordinates(old_position);

				Province* previous_province_position = provinces[old_position->y][old_position->x];

				provinces_pending_altitude_changes[y][x]+=previous_province_position->altitude;

				provinces_pending_altitude_changes[old_position->y][old_position->x]-= previous_province_position->altitude;
				delete(old_position);
			}
		}
	}
	//Apply the height changes
	for (int y = 0; y < world_height; y++)
	{
		for (int x = 0; x < world_width; x++)
		{
			provinces[y][x]->altitude += provinces_pending_altitude_changes[y][x];
		}
	}

	//Fill in teh gaps!
	std::vector<Province*> provinces_without_plate_neighbors;//Any provinces that have no nearby plates are put into this list and handled later
	for (int y = 0; y < world_height; y++)
	{
		for (int x = 0; x < world_width; x++)
		{
			if(plates_on_province[y][x].size() == 0)
			{
				//Perhaps only add altitude if there is water...
				provinces[y][x]->altitude+= 5+rand()%10;//Magma is melting! add some altitude

				//The number of times a plate is a neighbor
				std::unordered_map<int,int> neighboring_plate_occurences;
				bool no_nearby_plates = true;

				//Cylcing through neighbors
				std::vector<Province*> neighbors = GetDiamondOfProvinces(x,y,1,false);
				for (int p = 0; p < neighbors.size(); p++)
				{
					Province* prov = neighbors[p];
					if(plates_on_province[prov->province_y][prov->province_x].size()!=0)
					{
						neighboring_plate_occurences[plates_on_province[prov->province_y][prov->province_x][0]]++;
						no_nearby_plates = false;
					}
				}
				if(no_nearby_plates == false)
				{
					int highest_occurence = 0;
					int chosen_plate = 0;
					//Figuring out which plate occurs the most
					for (auto it = neighboring_plate_occurences.begin(); it != neighboring_plate_occurences.end(); ++it) 
					{
						if(it->second>highest_occurence	)
						{
							highest_occurence = it->second;
							chosen_plate = it->first;
						}
					}
					tectonic_plates[chosen_plate]->provinces_in_plate.push_back(new Vector2(x,y));
					plates_on_province[y][x].push_back(chosen_plate);
				}
				else
				{
					provinces_without_plate_neighbors.push_back(provinces[y][x]);
				}
			}
		}
	}


	//Update the plate with its new provinces!
	//First clear them
	for (int t = 0; t < tectonic_plates.size(); t++)
	{
		tectonic_plates[t]->provinces_in_plate.clear();
	}

	//Now add the new provinces
	total_land = 0 ;
	for (int x = 0; x < world_width; x++)
	{
		for (int y = 0; y < world_height; y++)
		{
			if(plates_on_province[y][x].size()>0)
			{
				tectonic_plates[plates_on_province[y][x][0]]->provinces_in_plate.push_back(new Vector2(x,y));//Where it is now
			}
			if(provinces[y][x]->water_depth>0)
				province_water_unresolved.push_back(provinces[y][x]);
			total_land+= provinces[y][x]->altitude;
		}
	}

	//	Now that the altitude has changed we should update the water

	for (int i = 0; i < 10; i++)
	{
		for (int x = 0; x < world_width; x++)
		{
			for (int y = 0; y < world_height; y++)
			{
				if(provinces[y][x]->water_depth>0)
					province_water_unresolved.push_back(provinces[y][x]);
			}
		}
		ResolveAllWater();
	}



	for (int y= 0; y < world_height; y++)
	{
		for (int x= 0; x < world_width; x++)
		{
			if(provinces[y][x]->water_depth==0)
			{
				provinces[y][x]->biome = GRASSLAND;
			}
		}
	}

	//UpdateHighestMountain();

	Draw();

	//Resolve Collisions

	//Resolve creation

	//Resolve new plates speeds

	//Resolve Heigthmap

	//Resolve Water
};
int TectonicHandler::CalculatePlateDensity(TectonicPlate* myPlate)
{
	int total_landmass = 0;
	int total_water = 0;
	for (int p = 0; p < myPlate->provinces_in_plate.size(); p++)
	{
		total_landmass+=provinces[myPlate->provinces_in_plate[p]->y][myPlate->provinces_in_plate[p]->x]->altitude;
		total_water+=provinces[myPlate->provinces_in_plate[p]->y][myPlate->provinces_in_plate[p]->x]->water_depth;
	}

	return total_landmass/total_water;
};

void People::RunHumans()
{
	//Do all your hourly logic here
	ProcessPeople();

	if(current_hour !=24)
		current_hour++;
	else
		current_hour = 0;

	//DivvyUpFood();


	if(draw_every_hour || current_hour==12)
		Draw();


	if(current_hour>hours_in_day)
	{
		current_hour=0;
		current_day++;

		//Do all your daily logic here
		//UpdateProvinceFood();

		if(current_day>days_in_year)
		{
			//Do all your yearly logic here
			current_day = 0;
			current_year++;
		}

	}
}

void TectonicHandler::ResolveAllWater()
{
	ResolveWaterInProvince(province_water_unresolved[0]);
	while(province_water_unresolved.size()>0)
	{
		ResolveWaterInProvince(province_water_unresolved[province_water_unresolved.size()-1]);
		auto prov = province_water_unresolved.end()-1;
		province_water_unresolved.erase(province_water_unresolved.end()-1);
	}

	UpdateDeepestWater();
};
void TectonicHandler::ResolveWaterInProvince(Province* prov)
{
	//The province above, to the right, down, and left of our prov BUT NOT the prov itself
	std::vector<Province*> neighboring_provinces = GetSquareOfProvinces(prov->province_x,prov->province_y,2,false);

	bool done = false;
	int provinces_checked = 0;

	while(!done)//Keep passing out water till ur done
	{
		if(prov->water_depth > 1)//Do we have water to pass out?
		{
			int steepest_slope = 1;
			int chosen_slope = 0;

			//Finding out which neighbor has the steepest slope
			for (int n = 0; n < neighboring_provinces.size(); n++)
			{
				int difference = prov->getLandAndWaterHeight() - neighboring_provinces[n]->getLandAndWaterHeight();
				if(difference>steepest_slope)
				{
					chosen_slope = n;
					steepest_slope = difference;
				}
			}

			//Passing water to neighbor
			if( steepest_slope >1)
			{
				Province* neighbor = provinces[neighboring_provinces[chosen_slope]->province_y][neighboring_provinces[chosen_slope]->province_x];

				//Can we pass half our height difference
				if(prov->water_depth>=(steepest_slope/2))
				{
					//Yes we can
					prov->water_depth-= (steepest_slope/2) + (steepest_slope%2);

					neighbor->biome = WATER;
					neighbor->water_depth+=steepest_slope/2+ (steepest_slope%2);
				}
				else
				{
					//No we cant give enoguht to level them so we give all instead
					neighbor->water_depth+=prov->water_depth;
					neighbor->biome = WATER;
					prov->water_depth = 0;
				}

				if(times_drawn ==100000)
				{
					times_drawn =0;
				}
				if(times_drawn==0)
				{
					UpdateDeepestWater();
					Draw();
				}
				times_drawn++;

				province_water_unresolved.push_back(neighbor);
			}
			else
			{
				//Every surrounding province has a difference of 1 or 0
				done=true;
			}
		}
		else
		{
			//We have 1 or 0 water units
			done=true;
		}
	}
};
void TectonicHandler::FlushWater(int number_times)
{
	for (int i = 0; i < number_times; i++)
	{
		for (int x = 0; x < world_width; x++)
		{
			for (int y = 0; y < world_height; y++)
			{
				province_water_unresolved.push_back(provinces[y][x]);
			}
		}
		ResolveAllWater();
	}
};

void TectonicHandler::Erode()
{
};

void PeopleHandler::ProcessPeople()
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
void PeopleHandler::ProcessPersonAI(Person* person)
{
	/*Priority Order
	Food
	Sleep
	Building up food, shelter, crafts
	Interaction
	Dominion
	Philisophical
	*/

	MoveRandomDirection(person);
	//if(person->hunger>=hunger_seek_level)
	//{
	//	//Hungry
	//	SeekFood(person);
	//}
	////else if(current_hour < 6 || current_hour > 20)
	////{
	////	//Sleepy
	////	if(person->home!=NULL)
	////	{
	////		if(person->province_x != person->home->province_x || person->province_y != person->home->province_y)
	////		{
	////			//Go home
	////			MoveToCoordinates(person,person->home->province_x,person->home->province_y);
	////		}
	////	}
	////}
	///*else if( current_hour == 9 || current_hour ==19 ||current_hour ==20 || current_hour==21)
	//{
	//SeekInteraction(person);
	//}*/
	//else if(person->home == NULL)
	//{
	//	int province_x = person->province_x;
	//	int province_y = person->province_y;

	//	//Calculating the power buy in for the home
	//	Province* prov = provinces[province_y][province_x];
	//	int province_total_power = 0;

	//	if(prov->homes_on_province.size()!=0)
	//	{
	//		for (auto h = prov->homes_on_province.begin(); h != prov->homes_on_province.end(); ++h)
	//		{
	//			House* home = h->second;
	//			province_total_power += home->owner->power;
	//		}
	//		province_total_power = province_total_power/prov->homes_on_province.size();
	//	}

	//	//Can you afford it?
	//	if(person->power>=province_total_power)
	//	{
	//		person->power-=province_total_power;
	//		BuildHome(person);
	//	}
	//	else
	//	{
	//		MoveRandomDirection(person);
	//	}
	//}
	//else
	//{
	//	if(person->occupation == FARMER)
	//	{
	//		////If there isnt a house here
	//		//if(provinces[person->province_y][person->province_x]->homes_on_province.size()!=0)
	//		//{
	//		//	//Work the land
	//		//	provinces[person->home->province_y][person->home->province_x] ->food_in_province += person->strength;
	//		//}
	//		//else
	//		//{
	//		//	MoveRandomDirection(person);
	//		//}
	//	}
	//	else if (person->occupation == ARTISAN)
	//	{

	//	}
	//}
};
void PeopleHandler::SeekFood(Person* person)
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
void PeopleHandler::Sleep(Person* person)
{

};
void PeopleHandler::BuildResources(Person* person)
{

};
void PeopleHandler::BuildHome(Person* person)
{
	House* home = new House(house_id,person,person->province_x,person->province_y,
		provinces[person->province_y][person->province_x]->getCenter().x-((province_width)/2)+(rand()% (province_width)),
		provinces[person->province_y][person->province_x]->getCenter().y-((province_height)/2)+(rand()%(province_height)));

	person->home = home;
	Province* prov = provinces[person->province_y][person->province_x];
	prov->homes_on_province[house_id] = home;//home;
	house_id=house_id+1;
};
void PeopleHandler::SeekInteraction(Person* person)
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
void PeopleHandler::SeekDominion(Person* person)
{

};

void PeopleHandlerD::MoveRandomDirection(Person* person)
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
		if(person->province_x+1<world_width)
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
			if(provinces[person->province_y][person->province_x+1-world_width]->biome !=WATER)
			{
				person->province_x = person->province_x+1-world_width;

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
		if(person->province_y+1<world_height)
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
			if(provinces[person->province_y][person->province_x-1+world_width]->biome !=WATER)
			{
				person->province_x = person->province_x-1+world_width;

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
void PeopleHandler::MoveToCoordinates(Person* person, int x,int y)
{
	//If you are within bounds
	if(x != person->province_x && y!=person->province_y)
	{
		if(x>=0 && x<world_width && y>=0 && y<world_height)
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
					if(person->province_x<world_width)
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
					if(person->province_x<world_width)
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
					if(person->province_y<world_height)
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
					if(person->province_y<world_height)
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

void PeopleHandler::GetInLineForFood(int x,int y,Person* myPerson)
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
	for (int y = 0; y < world_height; y++)
	{
		for (int x = 0; x < world_width; x++)
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

void PeopleHandler::UpdatePersonPositionToProvince(Person* person)
{
	Vector2 province_center = provinces[person->province_y][person->province_x]->getCenter();
	person->position_x = province_center.x -(province_width/2) + rand()%province_width;
	person->position_y =province_center.y -(province_height/2) + rand()%province_height;
};

void Game::Draw()
{
	DrawProvinces();

	DrawPeople();

	DrawHouses();

	DrawData();

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
};

void Game::DrawProvinces()
{
	switch (currentMapMode)
	{
	case TERRAIN:
		for(std::vector<std::vector<Province*>>::size_type y = 0; y < world_height; y++) 
		{
			for(std::vector<Province*>::size_type x = 0; x < world_width; x++) 
			{
				Province* province = (provinces[y][x]);

				int color[3];
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;


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
					color[0] =  color_water[0];
					color[1] =  color_water[1];
					color[2] =  color_water[2];

					break;
				}

				if(province->biome != WATER)
				{
					//Land
					for (int c = 0; c < 3; c++)
					{
						if(province->altitude <=province_highest_altitude)
						{
							float percentage = ((float)color[c]/2) * ((float)province->altitude/(float)province_highest_altitude);
							color[c] = (color[c]*0.5) + percentage;
						}
						else
						{
							float w = 2;
						}
					}
				}
				else
				{
					//Water
					for (int c = 0; c < 3; c++)
					{
						float depth = (float)province->water_depth/(float)province_deepest_depth;
						if(depth >1)
							depth = 1;
						color[c] = ((float)color[c]) - (((float)color[c]/2) * depth);
					}
				}

				ALLEGRO_VERTEX vertices[] = 
				{
					{province->p0->x,province->p0->y, 0},
					{province->p1->x,province->p1->y, 0},
					{province->p2->x,province->p2->y,0},
					{province->p3->x,province->p3->y,0},
				};

				for (int i = 0; i < 4; i++)
				{
					vertices[i].color = al_map_rgb(color[0],color[1],color[2]);
				}

				al_draw_prim(vertices, NULL, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN );

				//Draw border
				if(province_borders_drawn)
				{
					al_draw_line(province->p0->x,province->p0->y,province->p1->x,province->p1->y,
						al_map_rgb(0,0,0),1);
					al_draw_line(province->p1->x,province->p1->y,province->p2->x,province->p2->y,
						al_map_rgb(0,0,0),1);
					al_draw_line(province->p2->x,province->p2->y,province->p3->x,province->p3->y,
						al_map_rgb(0,0,0),1);
					al_draw_line(province->p3->x,province->p3->y,province->p0->x,province->p0->y,
						al_map_rgb(0,0,0),1);
				}
			}
		}
		if(province_height_drawn)
		{
			for(std::vector<std::vector<Province*>>::size_type y = 0; y < world_height; y++) 
			{
				for(std::vector<Province*>::size_type x = 0; x < world_width; x++) 
				{
					Province* province = provinces[y][x];
					/*if(province->water_depth!=0){*/
					std::string string_date = std::to_string(province->getLandAndWaterHeight());
					const char * date = string_date.c_str();
					al_draw_text(arial8,al_map_rgb(color_text[0],color_text[1],color_text[1]), province->p0->x, province->p0->y, 0, date);
					//}
				}
			}
		}
		break;
	case PLATE_TECTONICS:

		int color[3];
		color[0] = 50;color[1] = 50;color[2]=50;

		for (int t = 0; t < tectonic_plates.size(); t++)
		{
			TectonicPlate* plate = tectonic_plates[t];
			for (int p = 0; p < plate->provinces_in_plate.size(); p++)
			{
				Province* province = provinces[plate->provinces_in_plate[p]->y][plate->provinces_in_plate[p]->x];

				ALLEGRO_VERTEX vertices[] = 
				{
					{province->p0->x,province->p0->y, 0},
					{province->p1->x,province->p1->y, 0},
					{province->p2->x,province->p2->y,0},
					{province->p3->x,province->p3->y,0},
				};

				for (int i = 0; i < 4; i++)
				{
					vertices[i].color = al_map_rgb(color[0],color[1],color[2]);
				}

				al_draw_prim(vertices, NULL, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN );
			}

			//Each new plate gets a different color
			color[0] += 66;
			color[1] += 42;
			color[2] += 33;
		}
		break;
	case PLANT:
		break;
	case ANIMAL:
		break;
	case HUMAN:
		break;
	default:
		break;
	}

};
void Game::DrawRivers()
{
	for(std::vector<std::vector<Vector2*>>::size_type r = 0; r < rivers_points.size(); r++) 
	{
		for(std::vector<Vector2*>::size_type p = 1; p < rivers_points[r].size(); p++) 
		{
			Vector2* beginning = rivers_points[r][p-1];
			Vector2* end = rivers_points[r][p];

			al_draw_line(
				beginning->x,beginning->y,end->x,end->y,
				al_map_rgb(color_water[0],color_water[1],color_water[2]),
				3);
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

	//	if(y-1>=0 && x+1<world_width)//Topright
	//	{
	//		topLeftArability = provinces[y-1][x+1]->arability;
	//		total_arability+=topLeftArability;
	//		provinces_used++;
	//	}
	//	if(x+1<world_width)//bottomright
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

	//	if(x+1<world_width)//Topright
	//	{
	//		topLeftArability = provinces[y][x+1]->arability;
	//		total_arability+=topLeftArability;
	//		provinces_used++;
	//	}
	//	if(x+1<world_width&&y+1<world_height)//bottomright
	//	{
	//		topRightArability = provinces[y+1][x+1]->arability;
	//		total_arability+= topRightArability;
	//		provinces_used++;
	//	}

	//	if(y+1<world_height)//bottomleft
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
	//	if(y+1<world_height)//bottomright
	//	{
	//		topRightArability = provinces[y+1][x]->arability;
	//		total_arability+= topRightArability;
	//		provinces_used++;
	//	}

	//	if(y+1<world_height && x-1>=0)//bottomleft
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

void Game::DrawHouses()
{
	for (int y = 0; y < world_height; y++)
	{
		for (int x = 0; x < world_width; x++)
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
	al_draw_line(x,y-1,x,y-5,al_map_rgb(r,g,b),3);//Actual
};
void Game::DrawData()
{
	std::string string_date = "Year: " + std::to_string(current_year) + " Day: " + std::to_string(current_day) + " Hour: " + std::to_string(current_hour);
	const char * date = string_date.c_str();
	al_draw_text(arial16,al_map_rgb(color_text[0],color_text[1],color_text[1]), (screen_game_width/2)-80, 0, 0, date);

	std::string string_water = "Land: " + std::to_string(total_land);
	const char * water = string_water.c_str();
	al_draw_text(arial16,al_map_rgb(color_text[0],color_text[1],color_text[1]), (screen_game_width/2)-80, 20, 0, water);
};
void Game::DrawPopulation()
{
	std::string string_population= "Population: " + std::to_string(people.size());
	const char * population = string_population.c_str();
	al_draw_text(arial16,al_map_rgb(color_text[0],color_text[1],color_text[1]), (screen_game_width/2)-80, 20, 0, population);
};

void Game::FreeMemory()
{

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

