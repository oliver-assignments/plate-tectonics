#ifndef __WS_PLANT_HANDLER__
#define __WS_PLANT_HANDLER__

#include "Plant.h"
class Context;
class Province;


class PlantHandler
{
public:
	static void InitializeHandler(Context* myContext)
	{
		context=myContext;
	};
	static void CreatePlants();

	static void AdvancePlants();

	static Context* context;
};

#endif