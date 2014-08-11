#ifndef __WS_ANIMAL_HEADER__
#define __WS_ANIMAL_HEADER__

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

private:
	static Context* context;
};

#endif