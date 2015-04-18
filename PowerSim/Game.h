#ifndef __WS_GAME__
#define __WS_GAME__

#include "settings.h"
#include "allegro_engine.h"


#include "tectonic.h"

//Global Code
#include "context.h"
#include "Province.h"
#include "TectonicPlate.h"

enum MapMode
{
	TERRAIN, 
	FLIPPED,
	ASTHENOSPHERE,
	PLATE_TECTONICS, 
	PLANT,
	ANIMAL,
	HUMAN, 
};

class Game
{
public:
	//			//
	//Initialize//
	//			//

	//Game();
	void Initialize();
	void InitializeVariables();
	void ProcessSettings();
	void CreateWorld();

	void CreateProvinces();
	void CreateContinents();

	//		//
	//Update//
	//		//
	void Update();
	
	void UpdateDeepestWater();
	void UpdateHighestMountain();
	void UpdateHottestAsthenosphere();

	//Draw
	void Draw(MapMode myMapMode);

	void DrawProvinces(MapMode myMapMode);

	void DrawCluster(int x, int y, unsigned char r,unsigned char g,unsigned char b);
	void DrawBlade(int x, int y, unsigned char r,unsigned char g,unsigned char b);

	Context* context;

private:

	std::vector<Vector2*> province_vertices;
	bool province_jiggle;

	int province_jiggle_width;
	int province_jiggle_height;

	bool province_borders_drawn;
	bool province_height_drawn;

	int province_highest_altitude;
	int province_deepest_depth;
	int province_hottest_asthenosphere;

	int province_id;

	//Definiting Colors
	MapMode map_mode;

	double color_base_value ;

	int color_text[3];

	int color_grassland[3];
	int color_forest[3];
	int color_alpine[3];
	int color_desert[3];
	int color_water[3];
	int color_jungle[3];
	int color_tundra[3];
};

#endif