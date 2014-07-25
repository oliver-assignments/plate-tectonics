#ifndef Plant_H
#define Plant_H

#include "Province.h"
#include <vector>
#include <thread>
#include <stdlib.h> 
#include <ctime>
#include <mutex>
#include <string>
#include <algorithm>  
#include <stdio.h>
#include <tchar.h>
#include <iostream>

class Plant
{
public:

	Plant(
		std::string myName,
		Province* myProvince,
		int myTastiness, 
		int myCatalystHours, int myYield)
	{
		name = myName;
		province_parent = myProvince;

		tastiness = myTastiness;
		
		catalyst_hours = myCatalystHours;
		yield = myYield;


		/*generation = 0;
		generations_for_mutation = myGenerationsForMutation;

		competativeness = myCompetativeness;*/
	};

	Province* province_parent;

	std::string name;
	int id;

	int tastiness;
	int catalyst_hours; int worked_hours;
	int yield;
	int seeds_per_yield;

	std::string GetID()
	{
		return name							  + ":" + 
			   std::to_string(tastiness)      + ":" + 
               std::to_string(catalyst_hours) + ":" + 
			   std::to_string(yield)		  + ":" + 
			   std::to_string(seeds_per_yield);
	}

	int climate_minimum;
	int climate_maximum;

	int generation;
	int generations_for_mutation;

	int competativeness;

	void Work()
	{
		worked_hours++;
		
		if( worked_hours>=catalyst_hours)
		{
			worked_hours -= (catalyst_hours-worked_hours);
		}

		province_parent->food_in_province += yield;

		//Germinate onto other provinces!!!
	};

	//Dont make food too tasty or theyll starve!
	double getEffectivenessRatio()
	{
		return ((double)tastiness * ((double)catalyst_hours-worked_hours)/yield);
	};

	//If you are starving you will eat anything
	double getEffectivenessRatioWithoutTastiness()
	{
		return ((double)catalyst_hours-worked_hours)/yield;
	};
};
#endif