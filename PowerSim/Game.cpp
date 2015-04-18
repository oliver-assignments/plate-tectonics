#include "Game.h"
class TectonicHandler;

#include <iostream>

void Game::ProcessSettings()
{

	//province_jiggle_width = (int)(context->province_width*0.8);
	//province_jiggle_height = (int)(context->province_height*0.8);

	province_jiggle = atoi(Settings::GetSetting("province_jiggle").c_str());
	province_borders_drawn = atoi(Settings::GetSetting("province_borders_drawn").c_str());
	province_height_drawn = atoi(Settings::GetSetting("province_height_drawn").c_str());

	//Loading color settings
	color_base_value = 0.5;

	color_text[0] = atoi(Settings::GetSetting("color_text_r").c_str());
	color_text[1] = atoi(Settings::GetSetting("color_text_g").c_str());
	color_text[2] = atoi(Settings::GetSetting("color_text_b").c_str());

	color_grassland[0] =atoi(Settings::GetSetting("color_grassland_r").c_str());
	color_grassland[1] =atoi(Settings::GetSetting("color_grassland_g").c_str());
	color_grassland[2] =atoi(Settings::GetSetting("color_grassland_b").c_str());

	color_jungle[0] =atoi(Settings::GetSetting("color_jungle_r").c_str());
	color_jungle[1] =atoi(Settings::GetSetting("color_jungle_g").c_str());
	color_jungle[2] =atoi(Settings::GetSetting("color_jungle_b").c_str());

	color_desert[0] =atoi(Settings::GetSetting("color_desert_r").c_str());
	color_desert[1] =atoi(Settings::GetSetting("color_desert_g").c_str());
	color_desert[2] =atoi(Settings::GetSetting("color_desert_b").c_str());

	color_water[0] =atoi(Settings::GetSetting("color_water_r").c_str());
	color_water[1] =atoi(Settings::GetSetting("color_water_g").c_str());
	color_water[2] =atoi(Settings::GetSetting("color_water_b").c_str());

	color_tundra[0] =atoi(Settings::GetSetting("color_tundra_r").c_str());
	color_tundra[1] =atoi(Settings::GetSetting("color_tundra_g").c_str());
	color_tundra[2] =atoi(Settings::GetSetting("color_tundra_b").c_str());

	color_alpine[0] =atoi(Settings::GetSetting("color_alpine_r").c_str());
	color_alpine[1] =atoi(Settings::GetSetting("color_alpine_g").c_str());
	color_alpine[2] =atoi(Settings::GetSetting("color_alpine_b").c_str());

	color_forest[0] =atoi(Settings::GetSetting("color_forest_r").c_str());
	color_forest[1] =atoi(Settings::GetSetting("color_forest_g").c_str());
	color_forest[2] =atoi(Settings::GetSetting("color_forest_b").c_str());
};
void Game::InitializeVariables()
{	
	//These are used to make the map relative and change often
	province_highest_altitude = 1000;
	province_deepest_depth = 10;
	province_id=0;
};

void Game::CreateWorld()
{
	context = new Context();
	context->world_name = std::to_string(RandomNumberBelow(1001));

	std::cout<<endl<<"The world is named "<<context->world_name<<"."<<endl;

	_mkdir(("./Output/"+context->world_name).c_str());
	_mkdir((("./Output/"+context->world_name)+"/Plates").c_str());
	_mkdir((("./Output/"+context->world_name)+"/Terrain").c_str());
	_mkdir((("./Output/"+context->world_name)+"/Asthenosphere").c_str());
		CreateProvinces();
	CreateContinents();

	UpdateHighestMountain();

	TectonicHandler::InitializeHandler(context);
	TectonicHandler::CreateTectonicPlates();

	TectonicHandler::CreateWater();
	TectonicHandler::ResolveAllWater();
	//PlantHandler::InitializeHandler(context);
	////PlantHandler::CreatePlants();

	//AnimalHandler::InitializeHandler(context);
	////AnimalHandler::CreateAnimals();

	//PeopleHandler::InitializeHandler(context);
	//Note we may not want to do this before we move the plates along
	//PeopleHandler::CreatePeople();

	std::cout<<endl<<"Done creating the world."<<endl;
};

void Game::CreateProvinces()
{
	context->screen_width = atoi((Settings::GetSetting("screen_width")).c_str());  
	context->screen_height = atoi((Settings::GetSetting("screen_height")).c_str());

	context->province_width = atoi((Settings::GetSetting("province_width")).c_str());
	context->province_height = atoi((Settings::GetSetting("province_height")).c_str());

	context->world_width = (int)(context->screen_width/context->province_width);
	context->world_height = (int)(context->screen_height/context->province_height);

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
	std::cout<<endl<<"Creating continents."<<endl;
	for (int f = 0; f <7; f++)
	{
		//Center_of_continent
		int cluster_origin_province_x=0;
		int cluster_origin_province_y=0;
		while(cluster_origin_province_x ==0 ||cluster_origin_province_y ==0)
		{
			int attempted_x = (context->world_width/2)+RandomNumberBetween(-context->world_width/4,context->world_width/4);
			int attempted_y = (context->world_height/2)+RandomNumberBetween(-context->world_height/3,context->world_height/3);

			if(context->provinces[attempted_y][attempted_x]->biome!=GRASSLAND)
			{
				cluster_origin_province_x=attempted_x;
				cluster_origin_province_y=attempted_y;
			}
		}
		//How far we venture from the continent to place a blob
		int radius = ((context->world_width+context->world_height)/2)/6;

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
	std::cout<<"Continents created."<<endl;
};

void Game::Update()
{	
	for (int p = 0; p < 30; p++)
	{
		UpdateHighestMountain();
		UpdateDeepestWater();
		UpdateHottestAsthenosphere();

		Draw(ASTHENOSPHERE);
		AllegroEngine::FlushScreenshot(context->world_name,context->current_year,context->current_day,"Asthenosphere");
		al_flip_display();

		if(atoi(Settings::GetSetting("create_screen").c_str()))
			al_rest(1);

		Draw(PLATE_TECTONICS);
		AllegroEngine::FlushScreenshot(context->world_name,context->current_year,context->current_day,"Plates");
		al_flip_display();

		if(atoi(Settings::GetSetting("create_screen").c_str()))
			al_rest(1);

		Draw(TERRAIN);
		AllegroEngine::FlushScreenshot(context->world_name,context->current_year,context->current_day,"Terrain");
		al_flip_display();

		TectonicHandler::AdvanceTectonics();

		context->current_year++;
	}


	//while(true==false)
	//{
	//	int save_interval = atoi(Settings::GetSetting("save_interval_years").c_str());
	//	int save_counter=0;

	//	int tectonic_interval = atoi(Settings::GetSetting("tectonic_interval_years").c_str());
	//	int tectonic_counter = 0;

	//	//The real game loop!
	//	for (int d= 1; d <= 365; d++)
	//	{
	//		for (int h = 1; h <= 24; h++)
	//		{
	//			//		HOURLY LOGIC		//
	//			//Update people

	//			context->current_hour++;
	//		}
	//		//		DAILY LOGIC			//
	//		//Update plants and animals

	//		context->current_hour=0;
	//		context->current_day++;
	//	}
	//	//		YEARLY LOGIC		//

	//	//Is it time to save? We do this before tectonics because plates are taxing
	//	save_counter++;
	//	if(save_counter>=save_interval)
	//	{
	//		context->SaveWorld();
	//	}

	//	//Is it time for plates to move?
	//	tectonic_counter++;
	//	if(tectonic_counter>=tectonic_interval)
	//	{
	//		//Now that the landscape has changed flush the new terrain!
	//		Draw(TERRAIN);
	//		AllegroEngine::FlushScreenshot(context->world_name,context->current_year,context->current_day,"Terrain");
	//		al_flip_display();

	//		Draw(PLATE_TECTONICS);
	//		AllegroEngine::FlushScreenshot(context->world_name,context->current_year,context->current_day,"Plates");
	//		al_flip_display();

	//		TectonicHandler::AdvanceTectonics();
	//		UpdateHighestMountain();
	//		TectonicHandler::ResolveAllWater();
	//		UpdateDeepestWater();

	//		tectonic_counter=0;
	//	}

	//	context->current_year++;
	//}

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
void Game::UpdateHottestAsthenosphere()
{
	province_hottest_asthenosphere = 0;
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			if(province_hottest_asthenosphere<context->asthenosphere_heat_map[y][x])
			{
				province_hottest_asthenosphere = context->asthenosphere_heat_map[y][x];
			}
		}
	}

};

void Game::Draw(MapMode myMapMode)
{
	DrawProvinces(myMapMode);
};

void Game::DrawProvinces(MapMode myMapMode)
{
	switch (myMapMode)
	{
	case FLIPPED:
		for(std::vector<std::vector<Province*>>::size_type y = 0; y <context->world_height; y++) 
		{
			for(std::vector<Province*>::size_type x = 0; x <context->world_width; x++) 
			{
				Province* province = (context->provinces[y][x]);

				int color[3];
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;

				if(context->new_flipped_provinces[y][x])
				{
					color[0] = 255;
				}
				else
				{
					color[1] = 255;
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
			}
		}
		break;
		break;
	case ASTHENOSPHERE:
		for(std::vector<std::vector<Province*>>::size_type y = 0; y <context->world_height; y++) 
		{
			for(std::vector<Province*>::size_type x = 0; x <context->world_width; x++) 
			{
				Province* province = (context->provinces[y][x]);

				int color[3];
				color[0] = 255;
				color[1] = 0;
				color[2] = 0;

				float percentage = ((float)color[0]/2) * ((float)context->asthenosphere_heat_map[y][x]/(float)province_hottest_asthenosphere);
				color[0] = (color[0]*0.5) + percentage;

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
		}
		break;
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
						al_map_rgb(150,150,150),1);
					al_draw_line(province->p1->x,province->p1->y,province->p2->x,province->p2->y,
						al_map_rgb(150,150,150),1);
					al_draw_line(province->p2->x,province->p2->y,province->p3->x,province->p3->y,
						al_map_rgb(150,150,150),1);
					al_draw_line(province->p3->x,province->p3->y,province->p0->x,province->p0->y,
						al_map_rgb(150,150,150),1);
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
					AllegroEngine::WriteText(std::to_string(province->getLandAndWaterHeight()),province->p0->x, province->p0->y,AllegroEngine::arial8,"center");
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
