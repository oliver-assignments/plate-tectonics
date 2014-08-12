#include "people.h"

//Default values

Context* PeopleHandler::context;

int PeopleHandler::power_highest_current=0;
int PeopleHandler::strength_highest_current=0;
int PeopleHandler::intelligence_highest_current=0;

Person* PeopleHandler::power_highest_person = NULL;
Person* PeopleHandler::strength_highest_person = NULL;
Person* PeopleHandler::intelligence_highest_person = NULL;

int PeopleHandler::generation_youngest=1;

int PeopleHandler::hunger_seek_level = 0;
int PeopleHandler::hunger_death_level = 0;

int PeopleHandler::foreign_max = 100;

std::vector<Province*> PeopleHandler::provinces_with_hungry_people;

//Functions

void PeopleHandler::CreatePeople(int myNumberClusters,int myPeoplePerCluster, int myForeignRadius)
{
	//for(int c = 0 ; c<myNumberClusters;c++)
	//{
	//	//Province Spawn
	//	int cluster_origin_province_x =0;
	//	int cluster_origin_province_y =0;
	//	do
	//	{
	//		cluster_origin_province_x = rand()%context->world_width;
	//		cluster_origin_province_y = rand()%context->world_height;
	//	} while (context->provinces[cluster_origin_province_y][cluster_origin_province_x]->biome == WATER ||
	//		provinces[cluster_origin_province_y][cluster_origin_province_x]->biome == TUNDRA);


	//	int foreign_origin_x = cos(c*(2*3.14)/myNumberClusters) * myForeignRadius;
	//	int foreign_origin_y = sin(c*(2*3.14)/myNumberClusters) * myForeignRadius;

	//	for(int p = 0 ; p < myPeoplePerCluster;p++)
	//	{
	//		int province_x = cluster_origin_province_x;
	//		int province_y = cluster_origin_province_y;

	//		//5int province_area = context->provinces[province_y][province_x]->getArea();
	//		Vector2 province_center = context->provinces[province_y][province_x]->getCenter();

	//		Person* person = new Person(player_id,
	//			(rand()%hunger_seek_level),
	//			3+(1+rand()%50),
	//			1,
	//			-10+foreign_origin_x+(rand()%20), 
	//			-10+foreign_origin_y+(rand()%20),
	//			province_x,
	//			province_y,
	//			province_center.x -(province_width/2) + rand()%province_width,
	//			province_center.y -(province_height/2) + rand()%province_height);

	//		Province* prov = context->provinces[province_y][province_x];
	//		prov->people_on_province[player_id] = person;

	//		player_id = player_id+1;

	//		person->power+=rand()%40;

	//		people.push_back(person);
	//	}
	//}
};

void PeopleHandler::RunHumans()
{
	////Do all your hourly logic here
	//ProcessPeople();

	//if(current_hour !=24)
	//	current_hour++;
	//else
	//	current_hour = 0;

	////DivvyUpFood();


	//if(draw_every_hour || current_hour==12)
	//	Draw();


	//if(current_hour>hours_in_day)
	//{
	//	current_hour=0;
	//	current_day++;

	//	//Do all your daily logic here
	//	//UpdateProvinceFood();

	//	if(current_day>days_in_year)
	//	{
	//		//Do all your yearly logic here
	//		current_day = 0;
	//		current_year++;
	//	}

	//}
};

void PeopleHandler::ProcessPeople()
{
	//for(std::vector<Person*>::size_type i = 0; i < people.size(); i++) 
	//{
	//	if(i>=0 && i <people.size())
	//	{
	//		/*if(people[i]->hunger>=hunger_death_level)
	//		{
	//		House* home = people[i]->home;
	//		if(home!=NULL)
	//		{
	//		home->abandoned = true;
	//		provinces[home->province_y][home->province_x]->homes_on_province.erase(home->id);
	//		}

	//		people[i]->dead = true;
	//		provinces[people[i]->province_y][people[i]->province_x]->people_on_province.erase(people[i]->id);
	//		delete(people[i]);
	//		people.erase(people.begin()+i);
	//		i--;
	//		continue;
	//		}*/

	//		if(current_hour==11 || current_hour==16 || current_hour==20)
	//		{
	//			people[i]->hunger= people[i]->hunger + hunger_seek_level;
	//		}

	//		ProcessPersonAI(people[i]);
	//	}
	//}
};
void PeopleHandler::ProcessPersonAI(Person* person)
{
	/*Priority Order
	Food
	Sleep
	Building up food, shelter, crafts
	Interaction
	Dominion
	Philisophical
	*/

	//MoveRandomDirection(person);
	//if(person->hunger>=hunger_seek_level)
	//{
	//	//Hungry
	//	SeekFood(person);
	//}
	////else if(current_hour < 6 || current_hour > 20)
	////{
	////	//Sleepy
	////	if(person->home!=NULL)
	////	{
	////		if(person->province_x != person->home->province_x || person->province_y != person->home->province_y)
	////		{
	////			//Go home
	////			MoveToCoordinates(person,person->home->province_x,person->home->province_y);
	////		}
	////	}
	////}
	///*else if( current_hour == 9 || current_hour ==19 ||current_hour ==20 || current_hour==21)
	//{
	//SeekInteraction(person);
	//}*/
	//else if(person->home == NULL)
	//{
	//	int province_x = person->province_x;
	//	int province_y = person->province_y;

	//	//Calculating the power buy in for the home
	//	Province* prov = context->provinces[province_y][province_x];
	//	int province_total_power = 0;

	//	if(prov->homes_on_province.size()!=0)
	//	{
	//		for (auto h = prov->homes_on_province.begin(); h != prov->homes_on_province.end(); ++h)
	//		{
	//			House* home = h->second;
	//			province_total_power += home->owner->power;
	//		}
	//		province_total_power = province_total_power/prov->homes_on_province.size();
	//	}

	//	//Can you afford it?
	//	if(person->power>=province_total_power)
	//	{
	//		person->power-=province_total_power;
	//		BuildHome(person);
	//	}
	//	else
	//	{
	//		MoveRandomDirection(person);
	//	}
	//}
	//else
	//{
	//	if(person->occupation == FARMER)
	//	{
	//		////If there isnt a house here
	//		//if(provinces[person->province_y][person->province_x]->homes_on_province.size()!=0)
	//		//{
	//		//	//Work the land
	//		//	provinces[person->home->province_y][person->home->province_x] ->food_in_province += person->strength;
	//		//}
	//		//else
	//		//{
	//		//	MoveRandomDirection(person);
	//		//}
	//	}
	//	else if (person->occupation == ARTISAN)
	//	{

	//	}
	//}
};
void PeopleHandler::SeekFood(Person* person)
{
	////Currently eating -whatever amount- takes an hour

	//if(person->food_carried>0)//
	//{
	//	//Eat all your food
	//	int amount_eaten = person->food_carried - person->hunger;
	//	if(amount_eaten<=0)
	//	{
	//		person->food_carried = 0;
	//		person->hunger-=person->food_carried;
	//	}
	//	else if(amount_eaten>0)
	//	{
	//		person->hunger-=amount_eaten;
	//		person->food_carried-=amount_eaten;
	//	}
	//	//If you are more than food add the food back to what you carried
	//	if(person->hunger<0)
	//	{
	//		person->food_carried-=person->hunger;
	//		person->hunger = 0;
	//	}
	//}
	//else//You don't have any food
	//{
	//	GetInLineForFood(person->province_x,person->province_y,person);
	//}
	////else if(person->home->food_stored!=0)//If you have food in your home
	////{
	////	if(person->province_x == person->home->province_x &&person->province_x == person->home->province_x )
	////	{
	////		//You are on your houses tile
	////	} 
	////	else
	////	{
	////		//You are away from home
	////	}
	////}



};
void PeopleHandler::Sleep(Person* person)
{

};
void PeopleHandler::BuildResources(Person* person)
{

};
void PeopleHandler::BuildHome(Person* person)
{
	//House* home = new House(house_id,person,person->province_x,person->province_y,
	//	provinces[person->province_y][person->province_x]->getCenter().x-((province_width)/2)+(rand()% (province_width)),
	//	provinces[person->province_y][person->province_x]->getCenter().y-((province_height)/2)+(rand()%(province_height)));

	//person->home = home;
	//Province* prov = context->provinces[person->province_y][person->province_x];
	//prov->homes_on_province[house_id] = home;//home;
	//house_id=house_id+1;
};
void PeopleHandler::SeekInteraction(Person* person)
{
	//std::vector<Province*> adjacent_provinces = GetAllAdjacentProvinces(person->province_x,person->province_y);

	//Province* most_populated_neighbor = context->provinces[person->province_y][person->province_x];

	//for (int p = 0; p < adjacent_provinces.size(); p++)
	//{
	//	int prov_size =  most_populated_neighbor->people_on_province.size();
	//	int adj_size =adjacent_provinces[p]->people_on_province.size();

	//	if(adj_size >prov_size)
	//	{
	//		most_populated_neighbor = adjacent_provinces[p];
	//	}
	//}
	//MoveToCoordinates(person,most_populated_neighbor->province_x,most_populated_neighbor->province_y);

	//if(person->province_x == most_populated_neighbor->province_x &&person->province_y == most_populated_neighbor->province_y)
	//{
	//	if(most_populated_neighbor->people_on_province.size() >0)
	//	{
	//		int randomPersonIndex = rand()%most_populated_neighbor->people_on_province.size();
	//		Person* conversation_partner = most_populated_neighbor->people_on_province[randomPersonIndex];
	//		if(conversation_partner !=NULL)
	//		{
	//			int power_transfer = 1;

	//			//Whoever is stronger gets more powerful
	//			if(person->strength>conversation_partner->strength)
	//			{
	//				person->power = person->power+power_transfer;
	//				conversation_partner->power = conversation_partner->power-power_transfer;
	//			}
	//			else
	//			{
	//				person->power = person->power-power_transfer;
	//				conversation_partner->power = conversation_partner->power+power_transfer;
	//			}

	//			//Whoever is smarter gets more powerful
	//			if(person->intelligence>conversation_partner->intelligence)
	//			{
	//				person->power = person->power+power_transfer;
	//				conversation_partner->power = conversation_partner->power-power_transfer;
	//			}
	//			else
	//			{
	//				person->power = person->power-power_transfer;
	//				conversation_partner->power = conversation_partner->power+power_transfer;
	//			}

	//			//They cant be less than human
	//			if(person->power <0)
	//				person->power =0;
	//			if(conversation_partner->power <0)
	//				conversation_partner->power = 0;

	//			//Foreigness pulling//
	//		}
	//		else{most_populated_neighbor->people_on_province.erase(randomPersonIndex);}
	//	}
	//}
};
void PeopleHandler::SeekDominion(Person* person)
{

};

void PeopleHandler::MoveRandomDirection(Person* person)
{
	//int direction = rand()%20;

	//context->provinces[person->province_y][person->province_x]->people_on_province.erase(person->id);

	//switch (direction)
	//{
	//case 0 :
	//	//Up
	//	if(person->province_y-1>=0)
	//	{
	//		if(provinces[person->province_y-1][person->province_x]->biome !=WATER)
	//		{
	//			person->province_y = person->province_y-1;

	//			UpdatePersonPositionToProvince(person);
	//		}
	//		else
	//		{
	//			MoveRandomDirection(person);
	//		}
	//	}
	//	break;
	//case 1 :
	//	if(person->province_x+1<world_width)
	//	{
	//		if(provinces[person->province_y][person->province_x+1]->biome !=WATER)
	//		{
	//			person->province_x = person->province_x+1;

	//			UpdatePersonPositionToProvince(person);
	//		}
	//		else
	//		{
	//			MoveRandomDirection(person);
	//		}
	//	}
	//	else
	//	{
	//		if(provinces[person->province_y][person->province_x+1-world_width]->biome !=WATER)
	//		{
	//			person->province_x = person->province_x+1-world_width;

	//			UpdatePersonPositionToProvince(person);
	//		}
	//		else
	//		{
	//			MoveRandomDirection(person);
	//		}
	//	}
	//	break;
	//case 2 :
	//	//Down
	//	if(person->province_y+1<world_height)
	//	{
	//		if(provinces[person->province_y+1][person->province_x]->biome !=WATER)
	//		{
	//			person->province_y = person->province_y+1;

	//			UpdatePersonPositionToProvince(person);
	//		}
	//		else
	//		{
	//			MoveRandomDirection(person);
	//		}
	//	}
	//	break;
	//case 3 :
	//	//Left
	//	if(person->province_x-1>=0)
	//	{
	//		if(provinces[person->province_y][person->province_x-1]->biome !=WATER)
	//		{
	//			person->province_x = person->province_x-1;

	//			UpdatePersonPositionToProvince(person);
	//		}
	//		else
	//		{
	//			MoveRandomDirection(person);
	//		}
	//	}
	//	else
	//	{
	//		if(provinces[person->province_y][person->province_x-1+world_width]->biome !=WATER)
	//		{
	//			person->province_x = person->province_x-1+world_width;

	//			UpdatePersonPositionToProvince(person);
	//		}
	//		else
	//		{
	//			MoveRandomDirection(person);
	//		}
	//	}
	//	break;
	//default:
	//	break;
	//}
	//provinces[person->province_y][person->province_x]->people_on_province[person->id] = person;
};
void PeopleHandler::MoveToCoordinates(Person* person, int x,int y)
{
	////If you are within bounds
	//if(x != person->province_x && y!=person->province_y)
	//{
	//	if(x>=0 && x<world_width && y>=0 && y<world_height)
	//	{
	//		provinces[person->province_y][person->province_x]->people_on_province.erase(person->id);
	//		int location_difference_x = x - person->province_x;
	//		int location_difference_y = y - person->province_y;

	//		if(std::abs(location_difference_x)>std::abs(location_difference_y))
	//		{
	//			//X difference is greater than y difference
	//			//move in x diretion
	//			if(location_difference_x>0)
	//			{
	//				//Move right
	//				person->province_x = person->province_x+1;
	//				if(person->province_x<world_width)
	//				{
	//					UpdatePersonPositionToProvince(person);

	//				}
	//				else
	//				{
	//					person->province_x = person->province_x-1; 
	//				}
	//			}
	//			else
	//			{
	//				//Move left
	//				person->province_x = person->province_x-1;
	//				if(person->province_x>=0)
	//				{
	//					UpdatePersonPositionToProvince(person);
	//				}
	//				else
	//				{
	//					person->province_x = person->province_x+1; 
	//				}
	//			}
	//		}
	//		else if(std::abs(location_difference_x)==std::abs(location_difference_y))
	//		{
	//			if(location_difference_x>0)
	//			{
	//				//Move right
	//				person->province_x = person->province_x+1;
	//				if(person->province_x<world_width)
	//				{
	//					UpdatePersonPositionToProvince(person);

	//				}
	//				else
	//				{
	//					person->province_x = person->province_x-1; 
	//				}
	//			}
	//			else
	//			{
	//				//Move left
	//				person->province_x = person->province_x-1;
	//				if(person->province_x>=0)
	//				{
	//					UpdatePersonPositionToProvince(person);
	//				}
	//				else
	//				{
	//					person->province_x = person->province_x+1; 
	//				}
	//			}
	//			if(location_difference_y>0)
	//			{
	//				//Down
	//				person->province_y = person->province_y+1;
	//				if(person->province_y<world_height)
	//				{
	//					UpdatePersonPositionToProvince(person);

	//				}
	//				else
	//				{
	//					person->province_y = person->province_y-1; 
	//				}

	//			}
	//			else
	//			{
	//				//Up
	//				person->province_y = person->province_y-1;
	//				if(person->province_y>=0)
	//				{
	//					UpdatePersonPositionToProvince(person);
	//				}
	//				else
	//				{
	//					person->province_y = person->province_y+1; 
	//				}
	//			}
	//		}
	//		else
	//		{
	//			// ydifference is greater than x difference
	//			//move in y diretion

	//			if(location_difference_y>0)
	//			{
	//				//Down
	//				person->province_y = person->province_y+1;
	//				if(person->province_y<world_height)
	//				{
	//					UpdatePersonPositionToProvince(person);

	//				}
	//				else
	//				{
	//					person->province_y = person->province_y-1; 
	//				}

	//			}
	//			else
	//			{
	//				//Up
	//				person->province_y = person->province_y-1;
	//				if(person->province_y>=0)
	//				{
	//					UpdatePersonPositionToProvince(person);
	//				}
	//				else
	//				{
	//					person->province_y = person->province_y+1; 
	//				}
	//			}
	//		}
	//	}
	//	provinces[person->province_y][person->province_x]->people_on_province[person->id] = person;
	//}
};


//void PeopleHandler::GetInLineForFood(int x,int y,Person* myPerson)
//{
//	Province* prov = context->provinces[y][x];
//	prov->people_in_line.push_back(myPerson);
//
//	for (int i = 0; i < provinces_with_hungry_people.size(); i++)
//	{
//		if(prov->id == provinces_with_hungry_people[i]->id)
//		{
//			return;
//		}
//	}
//	provinces_with_hungry_people.push_back(prov);
//
//};
//void Game::UpdateProvinceFood()
//{
//	for (int y = 0; y <context->world_height; y++)
//	{
//		for (int x = 0; x <context->world_width; x++)
//		{
//			Province* province = context->provinces[y][x];
//
//			province->food_in_province = 0;
//			switch (province->biome)
//			{
//			case GRASSLAND:
//				province->food_in_province=hunger_seek_level*20;
//				break;
//			case DESERT:
//				province->food_in_province=hunger_seek_level*5;
//				break;
//			case JUNGLE:
//				province->food_in_province=hunger_seek_level*7;
//				break; 
//			case FOREST: 
//				province->food_in_province=hunger_seek_level*10;
//				break; 
//			case TUNDRA:
//				province->food_in_province=hunger_seek_level*5;
//				break;
//			case ALPINE:
//				province->food_in_province=hunger_seek_level*10;
//				break;
//			default:
//				break;
//			}
//		}
//
//	}
//};
//void Game::DivvyUpFood()
//{
//	if(provinces_with_hungry_people.size()!=0)
//	{
//		for (int i = 0; i < provinces_with_hungry_people.size(); i++)
//		{
//			if(i<0 || i >=provinces_with_hungry_people.size()){
//				return;}
//			Province* prov = provinces_with_hungry_people[i];
//
//			std::vector<Person*> line = prov->people_in_line;
//
//			//std::sort(line.begin(),line.end());
//			for (int p =  line.size()-1; p >-1; p--)
//			{
//				if(prov->food_in_province>0)
//				{
//					if(p<0 || p >=line.size()){
//						return;}
//					Person* next_in_line = line[p];
//
//					int amountTaken = ((next_in_line->strength*next_in_line->power)/hunger_seek_level);
//
//					if(amountTaken<prov->food_in_province)
//					{
//						prov->food_in_province-=amountTaken;
//
//					}
//					else
//					{
//						amountTaken = prov->food_in_province;
//						prov->food_in_province=0;
//
//					}
//					line[p]->food_carried+=amountTaken;
//				}
//				else
//				{
//					//Find place with more food
//					std::vector<Province*> adjacent_provinces = GetAllAdjacentProvinces(line[p]->province_x,line[p]->province_y);
//
//					Province* least_populated_neighbor = context->provinces[line[p]->province_y][line[p]->province_x];
//
//					for (int p = 0; p < adjacent_provinces.size(); p++)
//					{
//						int prov_size =  least_populated_neighbor->people_on_province.size();
//						int adj_size =adjacent_provinces[p]->people_on_province.size();
//
//						if(adj_size <prov_size)
//						{
//							least_populated_neighbor = adjacent_provinces[p];
//						}
//					}
//					//MoveToCoordinates(line[p],least_populated_neighbor->province_x,least_populated_neighbor->province_y);
//
//					MoveRandomDirection(line[p]);
//				}
//			}
//			prov->people_in_line.clear();
//		}
//		provinces_with_hungry_people.clear();
//	}
//};

//void PeopleHandler::UpdatePersonPositionToProvince(Person* person)
//{
//	Vector2 province_center = context->provinces[person->province_y][person->province_x]->getCenter();
//	person->position_x = province_center.x -(context->province_width/2) + rand()%context->province_width;
//	person->position_y =province_center.y -(context->province_height/2) + rand()%context->province_height;
//};