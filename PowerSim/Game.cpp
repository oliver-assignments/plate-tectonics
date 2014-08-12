
#include "Game.h"
class TectonicHandler;
class PlantHandler;
class PeopleHandler;
class AnimalHandler;

#include <iostream>

void Game::ProcessSettings()
{
	PeopleHandler::hunger_death_level = 100;
	PeopleHandler::hunger_seek_level = PeopleHandler::hunger_death_level/9;

	province_jiggle_width = (int)(context->province_width*0.8);
	province_jiggle_height = (int)(context->province_height*0.8);

	province_jiggle = false;
	province_borders_drawn = false;
	province_height_drawn = false;

	//Loading color settings
	color_base_value = 0.5;

	color_text[0] = atoi(Settings::GetSetting("color_text_r").c_str());
	color_text[1] = 255;
	color_text[2] = 255;

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
void Game::InitializeVariables()
{	
	census_mode = POWER;

	//These are used to make the map relative and change often
	province_highest_altitude = 1000;
	province_deepest_depth = 10;

	//Setuping up our values





	PeopleHandler::foreign_max=100;

	PeopleHandler::generation_youngest=1;

	player_id = 0;
	house_id = 0;
	province_id=0;



};

void Game::CreateWorld()
{
	context = new Context();

	CreateProvinces();
	CreateContinents();

	TectonicHandler::InitializeHandler(context);

	TectonicHandler::CreateTectonicPlates();
	TectonicHandler::CreateWater();
	TectonicHandler::ResolveAllWater();

	PlantHandler::InitializeHandler(context);
	//PlantHandler::CreatePlants();

	AnimalHandler::InitializeHandler(context);
	//AnimalHandler::CreateAnimals();

	PeopleHandler::InitializeHandler(context);
	//Note we may not want to do this before we move the plates along
	//PeopleHandler::CreatePeople();
};

void Game::CreateProvinces()
{
	context->world_width = (int)(AllegroEngine::screen_width/context->province_width);
	context->world_height = (int)(AllegroEngine::screen_height/context->province_height);

	int equator_position = (int)(context->world_height/2);

	for (int i = 0; i < ((context->world_width+1)*(context->world_height+1)); i++)
	{
		province_vertices.push_back(NULL);
	}

	for (int y = 0; y < context->world_height;y++)
	{
		std::vector<Province*> row;
		context->provinces.push_back(row);
		for (int x = 0; x < context->world_width; x++)
		{
			Vector2* top_left;
			Vector2* top_right;
			Vector2* bottom_left;
			Vector2* bottom_right;

			//TOP LEFT
			int index = (y*(context->world_width+1))+x;
			if(province_vertices[index]!=NULL)
			{
				top_left = province_vertices[index];
			}
			else
			{
				top_left = new Vector2((x*context->province_width), (y*context->province_height));
				province_vertices[index] = top_left;
			}

			//TOP RIGHT
			index = (y*(context->world_width+1))+(x+1);
			if(province_vertices[index]!=NULL)
			{
				top_right = province_vertices[index];
			}
			else
			{
				top_right = new Vector2((x*context->province_width)+context->province_width, y*context->province_height);
				province_vertices[index] = top_right;
			}

			//BOTTOM LEFT
			index = ((y+1)*(context->world_width+1))+x;
			if(province_vertices[index]!=NULL)
			{
				bottom_left = province_vertices[index];
			}
			else
			{
				bottom_left = new Vector2((x*context->province_width), (y*context->province_height) + context->province_height);
				province_vertices[index] = bottom_left;
			}

			//BOTTOM RIGHT
			index = ((y+1)*(context->world_width+1))+(x+1);
			if(province_vertices[index]!=NULL)
			{
				bottom_right = province_vertices[index];
			}
			else
			{
				bottom_right = new Vector2((x*context->province_width)+context->province_width, (y*context->province_height) + context->province_height);
				province_vertices[index] = bottom_right;
			}
			Province* province = new Province(province_id,x,y,top_left,top_right,bottom_right,bottom_left);

			province->altitude+= rand()%15;
			province->distance_from_equator = abs(equator_position-y);
			province_id++;

			context->provinces[y].push_back(province);
		}
	}

	//Jiggle
	if(province_jiggle){
		for(std::vector<Vector2*>::size_type v = 0; v != province_vertices.size(); v++) 
		{
			int x = v% (context->world_width+1);
			int y = v/ (context->world_width+1);
			if(x>0 && y>0 && x<context->world_width && y<context->world_height)
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
			int attempted_x = (context->world_width/2)+(rand()%(context->world_width/3))-(context->world_width/6);
			int attempted_y = (context->world_height/2)+(rand()%(context->world_height/2))-(context->world_height/4);

			if(context->provinces[attempted_y][attempted_x]->biome!=GRASSLAND)
			{
				cluster_origin_province_x=attempted_x;
				cluster_origin_province_y=attempted_y;
			}
		}
		//How far we venture from the continent to place a blob
		int radius = ((context->world_width+context->world_height)/2)/8;

		for (int i = 0; i < 15; i++)
		{
			std::vector<Province*> grassland_blob = context->GetDiamondOfProvinces(
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

//void Game::CreateRivers()
//{
//	int number_of_rivers = 0;
//	for (int r = 0; r <number_of_rivers; r++)
//	{
//		int origin_province_x = rand()%world_width;
//		int origin_province_y = rand()%world_height;
//
//		if(provinces[origin_province_y][origin_province_x]->biome!=WATER)
//		{
//			std::vector<Vector2*> river;
//
//			//First segment
//			Province* current_prov = context->provinces[origin_province_y][origin_province_x];
//
//			river.push_back(current_prov->p0);
//
//			//Making other segments
//			int river_length = 3+rand()%15;
//			for (int l = 0; l < river_length; l++)
//			{
//				Province* old_prov = current_prov;
//
//				while(old_prov == current_prov)
//				{
//					std::vector<Province*> surrounding = GetDiamondOfProvinces(current_prov->province_x,current_prov->province_y,1,true);
//
//					int next_prov = rand()%4;
//					switch (next_prov)
//					{
//					case 0:
//						river.push_back(surrounding[1]->p0);
//						current_prov = surrounding[1];
//						break;
//					case 1:
//						river.push_back(surrounding[2]->p0);
//						current_prov = surrounding[2];
//						break;
//					case 2:
//						river.push_back(surrounding[3]->p0);
//						current_prov = surrounding[3];
//						break;
//					case 3:
//						river.push_back(surrounding[4]->p0);
//						current_prov = surrounding[4];
//						break;
//					}
//
//					for (int p = 0; p < river.size()-1; p++)
//					{
//						if(river[river.size()-1] == river[p])
//						{
//							//We already have this point, undo the last point
//							river.pop_back();
//							current_prov = old_prov;
//						}
//					}
//					if(current_prov->biome == WATER)
//					{
//						river.pop_back();
//						l = river_length;
//					}
//				}
//			}
//			rivers_points.push_back(river);
//		}
//		else
//		{
//			r--;
//		}
//	}
//};

void Game::Update()
{	


};

void Game::UpdateDeepestWater()
{
	province_deepest_depth=0;
	for(std::vector<std::vector<Province*>>::size_type y = 0; y <context->world_height; y++) 
	{
		for(std::vector<Province*>::size_type x = 0; x <context->world_width; x++) 
		{
			Province* province = context->provinces[y][x];

			if(province_deepest_depth<province->water_depth)
				province_deepest_depth = province->water_depth;
		}
	}
};
void Game::UpdateHighestMountain()
{
	province_highest_altitude = 0;

	for(std::vector<std::vector<Province*>>::size_type y = 0; y < context->world_height; y++) 
	{
		for(std::vector<Province*>::size_type x = 0; x <context->world_width; x++) 
		{
			Province* province = context->provinces[y][x];

			if(province_highest_altitude<province->getLandAndWaterHeight())
				province_highest_altitude =province->getLandAndWaterHeight();
		}
	}
};

void Game::Draw()
{
	DrawProvinces();

	DrawPeople();

	DrawHouses();

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
};

void Game::DrawProvinces()
{
	switch (map_mode)
	{
	case TERRAIN:
		for(std::vector<std::vector<Province*>>::size_type y = 0; y <context->world_height; y++) 
		{
			for(std::vector<Province*>::size_type x = 0; x <context->world_width; x++) 
			{
				Province* province = (context->provinces[y][x]);

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
			for(std::vector<std::vector<Province*>>::size_type y = 0; y <context->world_height; y++) 
			{
				for(std::vector<Province*>::size_type x = 0; x <context->world_width; x++) 
				{
					Province* province = context->provinces[y][x];
					//AllegroEngine::DrawTextC(AllegroEngine::arial8,province->p0->x, province->p0->y,std::to_string(province->getLandAndWaterHeight()));
				}
			}
		}
		break;
	case PLATE_TECTONICS:

		int color[3];
		color[0] = 50;color[1] = 50;color[2]=50;

		for (int t = 0; t < context->tectonic_plates.size(); t++)
		{
			TectonicPlate* plate =  context->tectonic_plates[t];
			for (int p = 0; p < plate->provinces_in_plate.size(); p++)
			{
				Province* province = context->provinces[plate->provinces_in_plate[p]->y][plate->provinces_in_plate[p]->x];

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

//void Game::DrawRivers()
//{
//	for(std::vector<std::vector<Vector2*>>::size_type r = 0; r < rivers_points.size(); r++) 
//	{
//		for(std::vector<Vector2*>::size_type p = 1; p < rivers_points[r].size(); p++) 
//		{
//			Vector2* beginning = rivers_points[r][p-1];
//			Vector2* end = rivers_points[r][p];
//
//			al_draw_line(
//				beginning->x,beginning->y,end->x,end->y,
//				al_map_rgb(color_water[0],color_water[1],color_water[2]),
//				3);
//		}
//	}
//};
//void Game::CalculateVertexColor(int x, int y, ALLEGRO_VERTEX* myVertices)
//{
//	//int color[3];
//
//	//int average_arability = 0;
//
//	////Top left vertex
//	//{
//	//	int total_arability = 0;
//	//	int provinces_used = 0;
//
//	//	int bottomRightArability;
//	//	int topLeftArability;
//	//	int topRightArability;
//	//	int bottomLeftArability;
//
//	//	if(y-1>=0 && x-1>=0)//Topleft
//	//	{
//	//		bottomRightArability = context->provinces[y-1][x-1]->arability;
//	//		total_arability+= bottomRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(y-1>=0)//Topright
//	//	{
//	//		topLeftArability = context->provinces[y-1][x]->arability;
//	//		total_arability+=topLeftArability;
//	//		provinces_used++;
//	//	}
//	//	if(true)//bottomright
//	//	{
//	//		topRightArability = context->provinces[y][x]->arability;
//	//		total_arability+= topRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(x-1>=0)//bottomleft
//	//	{
//	//		bottomLeftArability = context->provinces[y][x-1]->arability;
//	//		total_arability+= bottomLeftArability;
//	//		provinces_used++;
//	//	}
//	//	average_arability = total_arability/provinces_used;
//
//	//	//Top left vertex
//	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
//	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
//	//	color[2] = ((double)average_arability/arability_max) * color_province[2];
//
//	//	myVertices[0].color = al_map_rgb(color[0],color[1],color[2]);
//	//}
//	////Top right vertex
//	//{
//	//	int total_arability = 0;
//	//	int provinces_used = 0;
//
//	//	int bottomRightArability;
//	//	int topLeftArability;
//	//	int topRightArability;
//	//	int bottomLeftArability;
//
//	//	if(y-1>=0)//Topleft
//	//	{
//	//		bottomRightArability = context->provinces[y-1][x]->arability;
//	//		total_arability+= bottomRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(y-1>=0 && x+1<world_width)//Topright
//	//	{
//	//		topLeftArability = context->provinces[y-1][x+1]->arability;
//	//		total_arability+=topLeftArability;
//	//		provinces_used++;
//	//	}
//	//	if(x+1<world_width)//bottomright
//	//	{
//	//		topRightArability = context->provinces[y][x+1]->arability;
//	//		total_arability+= topRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(true)//bottomleft
//	//	{
//	//		bottomLeftArability = context->provinces[y][x]->arability;
//	//		total_arability+= bottomLeftArability;
//	//		provinces_used++;
//	//	}
//	//	average_arability = total_arability/provinces_used;
//
//	//	//Top right vertex
//	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
//	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
//	//	color[2] = ((double)average_arability/arability_max) * color_province[2];
//
//	//	myVertices[1].color = al_map_rgb(color[0],color[1],color[2]);
//	//}
//	////Bottomright vertex
//	//{
//	//	int total_arability = 0;
//	//	int provinces_used = 0;
//
//	//	int bottomRightArability;
//	//	int topLeftArability;
//	//	int topRightArability;
//	//	int bottomLeftArability;
//
//	//	if(true)//Topleft
//	//	{
//	//		bottomRightArability = context->provinces[y][x]->arability;
//	//		total_arability+= bottomRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(x+1<world_width)//Topright
//	//	{
//	//		topLeftArability = context->provinces[y][x+1]->arability;
//	//		total_arability+=topLeftArability;
//	//		provinces_used++;
//	//	}
//	//	if(x+1<world_width&&y+1<world_height)//bottomright
//	//	{
//	//		topRightArability = context->provinces[y+1][x+1]->arability;
//	//		total_arability+= topRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(y+1<world_height)//bottomleft
//	//	{
//	//		bottomLeftArability = context->provinces[y+1][x]->arability;
//	//		total_arability+= bottomLeftArability;
//	//		provinces_used++;
//	//	}
//	//	average_arability = total_arability/provinces_used;
//
//	//	//Bottomright vertex
//	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
//	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
//	//	color[2] = ((double)average_arability/arability_max) * color_province[2];
//
//	//	myVertices[2].color = al_map_rgb(color[0],color[1],color[2]);
//	//}
//	////Bottomleft vertex
//	//{
//	//	int total_arability = 0;
//	//	int provinces_used = 0;
//
//	//	int bottomRightArability;
//	//	int topLeftArability;
//	//	int topRightArability;
//	//	int bottomLeftArability;
//
//	//	if(x-1>=0)//Topleft
//	//	{
//	//		bottomRightArability = context->provinces[y][x-1]->arability;
//	//		total_arability+= bottomRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(true)//Topright
//	//	{
//	//		topLeftArability = context->provinces[y][x]->arability;
//	//		total_arability+=topLeftArability;
//	//		provinces_used++;
//	//	}
//	//	if(y+1<world_height)//bottomright
//	//	{
//	//		topRightArability = context->provinces[y+1][x]->arability;
//	//		total_arability+= topRightArability;
//	//		provinces_used++;
//	//	}
//
//	//	if(y+1<world_height && x-1>=0)//bottomleft
//	//	{
//	//		bottomLeftArability = context->provinces[y+1][x-1]->arability;
//	//		total_arability+= bottomLeftArability;
//	//		provinces_used++;
//	//	}
//	//	average_arability = total_arability/provinces_used;
//
//	//	//Bottomleft vertex
//	//	color[0] = ((double)average_arability/arability_max) * color_province[0];
//	//	color[1] = ((double)average_arability/arability_max) * color_province[1];
//	//	color[2] = ((double)average_arability/arability_max) * color_province[2];
//
//	//	myVertices[3].color = al_map_rgb(color[0],color[1],color[2]);
//	//}
//
//};

void Game::DrawPeople()
{
	//Maintianing highest of power strength and intel
	if(PeopleHandler::power_highest_person != NULL)
		if(PeopleHandler::power_highest_person->dead)
			PeopleHandler::power_highest_person = NULL;
	if(PeopleHandler::strength_highest_person != NULL)
		if(PeopleHandler::strength_highest_person->dead)
			PeopleHandler::strength_highest_person = NULL;
	if(PeopleHandler::intelligence_highest_person != NULL)
		if(PeopleHandler::intelligence_highest_person->dead)
			PeopleHandler::intelligence_highest_person=NULL;


	//Processing highest
	for(std::vector<Person*>::size_type i = 0; i != context->people.size(); i++) 
	{
		if(!context->people[i]->dead)
		{
			if(PeopleHandler::power_highest_person == NULL)
				PeopleHandler::power_highest_person = context->people[i];
			if(PeopleHandler::strength_highest_person== NULL)
				PeopleHandler::strength_highest_person  = context->people[i];
			if(PeopleHandler::intelligence_highest_person== NULL)
				PeopleHandler::intelligence_highest_person = context->people[i];

			//Power

			if(PeopleHandler::power_highest_current<context->people[i]->power)
			{
				PeopleHandler::power_highest_current = context->people[i]->power;
				PeopleHandler::power_highest_person = context->people[i];
			}
			//Strength

			if(PeopleHandler::strength_highest_current<context->people[i]->strength)
			{
				PeopleHandler::strength_highest_current = context->people[i]->strength;
				PeopleHandler::strength_highest_person = context->people[i];
			}
			//Intel
			if(PeopleHandler::intelligence_highest_current<context->people[i]->intelligence)
			{
				PeopleHandler::intelligence_highest_current = context->people[i]->intelligence;
				PeopleHandler::intelligence_highest_person = context->people[i];
			}
		}
	}

	//Drawing alle
	for(std::vector<Person*>::size_type i = 0; i != context->people.size(); i++) 
	{
		Person* person = context->people[i];
		if(person->dead == false){
			ALLEGRO_COLOR person_color;
			double color [3]; 
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;

			if(census_mode == POWER)
			{

				color[0] = color_power[0]*((double)person->power/PeopleHandler::power_highest_current);

			}
			else if(census_mode == HUNGER)
			{
				color [0] =color_hunger[0]*(double)person->hunger/PeopleHandler::hunger_death_level;
				color [1] =color_hunger[1]*(double)person->hunger/PeopleHandler::hunger_death_level;
				color [2] =color_hunger[2]*(double)person->hunger/PeopleHandler::hunger_death_level;
			}
			else if(census_mode == STRENGTH)
			{

				color [0] =color_strength[0]*(double)person->strength/PeopleHandler::strength_highest_current;
				color [1] =color_strength[1]*(double)person->strength/PeopleHandler::strength_highest_current;
				color [2] =color_strength[2]*(double)person->strength/PeopleHandler::strength_highest_current;

			}
			else if(census_mode == INTELLIGENCE)
			{
				color [0] = color_intelligence[0]*(double)person->intelligence/ PeopleHandler::intelligence_highest_current;
				color [1] = color_intelligence[1] *(double)person->intelligence/PeopleHandler::intelligence_highest_current;
				color [2] = color_intelligence[2] *(double)person->intelligence/PeopleHandler::intelligence_highest_current;
			}
			else if(census_mode == FOREIGN)
			{
				if(person->foreign_x>=0 && person->foreign_y>=0)
				{
					//first quadrant

					color [0] =((double)person->foreign_x/PeopleHandler::foreign_max * color_foreign_east[0])+((double)person->foreign_y/PeopleHandler::foreign_max * color_foreign_north[0]);
					color [1] =((double)person->foreign_x/PeopleHandler::foreign_max * color_foreign_east[1])+((double)person->foreign_y/PeopleHandler::foreign_max * color_foreign_north[1]);
					color [2] =((double)person->foreign_x/PeopleHandler::foreign_max * color_foreign_east[2])+((double)person->foreign_y/PeopleHandler::foreign_max * color_foreign_north[2]);
				}
				else if(person->foreign_x<=0 && person->foreign_y>=0)
				{
					//second quad
					color [0] =((double)person->foreign_x/-PeopleHandler::foreign_max * color_foreign_west[0])+((double)person->foreign_y/PeopleHandler::foreign_max * color_foreign_north[0]);
					color [1] =((double)person->foreign_x/-PeopleHandler::foreign_max * color_foreign_west[1])+((double)person->foreign_y/PeopleHandler::foreign_max * color_foreign_north[1]);
					color [2] =((double)person->foreign_x/-PeopleHandler::foreign_max * color_foreign_west[2])+((double)person->foreign_y/PeopleHandler::foreign_max * color_foreign_north[2]);
				}
				else if(person->foreign_x<=0 && person->foreign_y<=0)
				{
					//third quad
					color [0] =((double)person->foreign_x/-PeopleHandler::foreign_max * color_foreign_west[0])+((double)person->foreign_y/-PeopleHandler::foreign_max * color_foreign_south[0]);
					color [1] =((double)person->foreign_x/-PeopleHandler::foreign_max * color_foreign_west[1])+((double)person->foreign_y/-PeopleHandler::foreign_max * color_foreign_south[1]);
					color [2] =((double)person->foreign_x/-PeopleHandler::foreign_max * color_foreign_west[2])+((double)person->foreign_y/-PeopleHandler::foreign_max * color_foreign_south[2]);
				}
				else
				{
					//fourth quadrant
					color [0] =((double)person->foreign_x/PeopleHandler::foreign_max * color_foreign_east[0])+((double)person->foreign_y/-PeopleHandler::foreign_max * color_foreign_south[0]);
					color [1] =((double)person->foreign_x/PeopleHandler::foreign_max * color_foreign_east[1])+((double)person->foreign_y/-PeopleHandler::foreign_max * color_foreign_south[1]);
					color [2] =((double)person->foreign_x/PeopleHandler::foreign_max * color_foreign_east[2])+((double)person->foreign_y/-PeopleHandler::foreign_max * color_foreign_south[2]);
				}
			}
			else if(census_mode == OCCUPATION)
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
				color [0] =((double)person->generation/ PeopleHandler::generation_youngest * color_generation[0]);
				color [1] =((double)person->generation/ PeopleHandler::generation_youngest * color_generation[1]);
				color [2] =((double)person->generation/ PeopleHandler::generation_youngest * color_generation[2]);
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
	//for (int y = 0; y <context->world_height; y++)
	//{
	//	for (int x = 0; x <context->world_width; x++)
	//	{
	//		if(context->provinces[y][x]->homes_on_province.size()>0)
	//		{
	//			int position_x=context->provinces[y][x]->getCenter().x;
	//			int position_y=context->provinces[y][x]->getCenter().y;

	//			int pop = context->provinces[y][x]->homes_on_province.size();

	//			if(pop>500000000000)
	//			{
	//				DrawMegalopolis(position_x,position_y);
	//			}
	//			else if(pop>400000)
	//			{
	//				DrawConurbation(position_x,position_y);
	//			}
	//			else if(pop>300000)
	//			{
	//				DrawMetropolis(position_x,y);
	//			}
	//			else if(pop>30000)
	//			{
	//				DrawCity(position_x,position_y);
	//			}
	//			else if(pop>2000)
	//			{
	//				DrawTown(position_x,position_y);
	//			}
	//			else if(pop>100)
	//			{
	//				DrawVillage(position_x,position_y);
	//			}
	//			else if(pop>10)
	//			{
	//				DrawHamlet(position_x,position_y);
	//			}
	//			else if(pop>0)
	//			{
	//				DrawHouse(position_x,position_y);
	//			}
	//			else//Nothing
	//			{
	//				return;
	//			}
	//		}
	//	}
	//}

	////Draw every house
	///*for(std::vector<House*>::size_type i = 0; i < houses.size(); i++) 
	//{
	//int position_x=houses[i]->position_x;
	//int position_y=houses[i]->position_y;

	//DrawHouse(position_x,position_y);
	//}*/
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
