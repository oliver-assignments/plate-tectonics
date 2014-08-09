#ifndef __WS_PLANT_HANDLER__
#define __WS_PLANT_HANDLER__

class Context;

class PlantHandler
{
public:
	static void CreatePlants();

	static void AdvancePlants(Context* myWorldContext);
private:
	static Context* context;
};

#endif