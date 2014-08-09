#ifndef __WS_People_HANDLER__
#define __WS_People_HANDLER__

class Context;

class PeopleHandler
{
public:
	static void InitializeHandler(Context* myWorldContext);

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
	void DivvyUpFood();
	void UpdateProvinceFood();
	
	void UpdatePersonPositionToProvince(Person* person);


private:
	Context* context;
};

#endif