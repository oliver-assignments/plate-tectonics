#ifndef __WS_TECTONIC_HANDLER__
#define __WS_TECTONIC_HANDLER__

class Context;

class TectonicHandler
{
public:
	static void CreateTectonicPlates();

	static void Erode();

	static void CreateWater();
	
	static void ResolveAllWater();
	static void ResolveWaterInProvince(Province* prov);
	static void FlushWater(int number_times);
	 
	static void AdvanceTectonics();
	static int CalculatePlateDensity(TectonicPlate* myPlate);
private:
	static Context* context;
};

#endif