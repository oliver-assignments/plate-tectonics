#ifndef __WS_ANIMAL_HEADER__
#define __WS_ANIMAL_HEADER__

#include "Animal.h"
class Context;

class AnimalHandler
{
public:
	static void InitializeHandler(Context* myContext)
	{
		context=myContext;
	};

	static void CreateAnimals();

	static void AdvanceAnimals();

	static Context* context;
};

#endif