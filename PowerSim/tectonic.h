#ifndef __WS_TECTONIC_HANDLER__
#define __WS_TECTONIC_HANDLER__

class Context;

class TectonicHandler
{
public:

	static void InitializeHandler(Context* myContext)
	{
		context=myContext;
	};

	static void CreateTectonicPlates();

	static void Erode();

	static void CreateWater();
	
	static void ResolveAllWater();
	static void ResolveWaterInProvince(Province* prov);
	static void FlushWater(int number_times);
	 
	static void AdvanceTectonics();
	static int CalculatePlateDensity(TectonicPlate* myPlate);

	static Context* context;

	static int plate_count;
	static std::vector<std::vector<bool>> has_plate;
	static std::vector<std::vector<int>> pending_altitude_changes;

	static std::vector<Vector2*> currentPlateCollisions;
	static std::vector<Vector2*> oldPlateCollisions;

	static::std::vector<Province*> unresolved_water;
	
};

#endif