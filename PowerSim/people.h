#ifndef __WS_People_HANDLER__
#define __WS_People_HANDLER__

class Context;
class Game;

class PeopleHandler
{
public:
	static void InitializeHandler(Context* myContext)
	{
		context=myContext;
	};

	static void CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius);

	void RunHumans();
	void ProcessPeople();

	static void ProcessPersonAI(Person* person);
	static void SeekDominion(Person* person);
	static void SeekFood(Person* person);
	static void BuildHome(Person* person);
	static void Sleep(Person* person);
	static void BuildResources(Person* person);
	static void SeekInteraction(Person* person);
	static void MoveRandomDirection(Person* person);
	static void MoveToCoordinates(Person* person, int x,int y);

	void GetInLineForFood(int x,int y,Person* myPerson);
	/*void DivvyUpFood();
	void UpdateProvinceFood();*/
	
	void UpdatePersonPositionToProvince(Person* person);

	static Context* context;


	static int power_highest_current;
	static Person* power_highest_person;

	static int strength_highest_current;
	static Person* strength_highest_person;
 
	static int intelligence_highest_current;
	static Person* intelligence_highest_person;
 
	static int hunger_death_level;
	static int hunger_seek_level;
 
	static int foreign_max;
 
	static int generation_youngest;

	std::vector<Province*> provinces_with_hungry_people;
};

#endif