#ifndef Animal_H
#define Animal_H

#include "Province.h"

namespace ws_global
{
	class Animal
	{
	public:
		Animal(bool myAngry,
			int myTastiness, 
			int myYield, 
			std::vector<Biome> myBiomesAccepted, 
			int myGenerationsForMutation, 
			int myCompetativeness)
		{

		};
		std::string name;

		bool is_domesticated;
		int hours_spent_domesticating;
		double chance_to_kill;
		int hours_to_domesticate;
		int work_hours_replaced;
		bool is_rideable;

		int meat_tastiness;
		int meat_produced;
		int biproduct_luxury_level;

		bool is_angry;
		bool has_weird_diet;
		bool can_public_sex;
		bool will_panic;
		bool is_social;

		int generation;
		int generations_to_mutate;

		int competativeness;


	};
}

#endif