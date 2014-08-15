#include "tectonic.h"

Context* TectonicHandler::context=NULL;

int TectonicHandler::plate_count = 0;
std::vector<std::vector<bool>> TectonicHandler::has_plate;
std::vector<std::vector<int>> TectonicHandler::pending_altitude_changes;

std::vector<Vector2*> TectonicHandler::currentPlateCollisions;
std::vector<Vector2*> TectonicHandler::oldPlateCollisions;

std::vector<Province*> TectonicHandler::unresolved_water;

void TectonicHandler::CreateTectonicPlates()
{
	std::cout<<"Creating tectonic plates."<<endl<<endl;

	int provinces_without_plate = context->world_height * context->world_width;

	int plate_count = 0;
	int reject_count=0;

	//Initializing the grid for tectonic plates
	for (int y = 0; y < context->world_height; y++)
	{
		std::vector<bool> row_of_taken_provinces;
		has_plate.push_back(row_of_taken_provinces);

		std::vector<int> row_of_altitude_changes;
		pending_altitude_changes.push_back(row_of_altitude_changes);

		std::vector<int> row_of_astehnosphere_heat;
		context->asthenosphere_heat_map.push_back(row_of_astehnosphere_heat);

		std::vector<std::vector<int>> row_of_list_of_new_plates_on_province;
		context->new_plates_on_province.push_back(row_of_list_of_new_plates_on_province);

		std::vector<std::vector<int>> row_of_list_of_old_plates_on_province;
		context->old_plates_on_province.push_back(row_of_list_of_old_plates_on_province);

		for (int x = 0; x < context->world_width; x++)
		{
			has_plate[y].push_back(false);
			pending_altitude_changes[y].push_back(0);
			context->asthenosphere_heat_map[y].push_back(0);
			context->new_plates_on_province[y].push_back(*new std::vector<int>);
			context->old_plates_on_province[y].push_back(*new std::vector<int>);
		}
	}

	int radius = (context->world_width+context->world_height)/12;

	//Creating every plate until 90% is covered
	while(provinces_without_plate>((context->world_height*context->world_width)/10))
	{
		if(reject_count<1000)
		{
			TectonicPlate* tectonic_plate = new TectonicPlate();
			tectonic_plate->plate_number = plate_count;
			plate_count++;

			//Finding a good blob origin that isnt already taken
			int cluster_origin_province_x=-1;
			int cluster_origin_province_y=-1;

			while(cluster_origin_province_x==-1 && cluster_origin_province_y==-1)
			{
				int attempted_x = RandomNumberBelow(context->world_width);
				int attempted_y = RandomNumberBelow(context->world_height);

				//This hasnt been taken, go ahead and use it as your center
				if(has_plate[attempted_y][attempted_x] == false)
				{
					cluster_origin_province_x = attempted_x;
					cluster_origin_province_y = attempted_y;
				}
				tectonic_plate->provinces_in_plate.push_back(new Vector2(cluster_origin_province_x,cluster_origin_province_y = attempted_y));
			}

			//Creating # of smaller diamonds near our center
			for (int q = 0; q < 60; q++)
			{
				//Wrap the x
				int piece_origin_x = cluster_origin_province_x + RandomNumberBetween(-radius,radius);
				if(piece_origin_x<0)
					piece_origin_x+=context->world_width;
				if(piece_origin_x>=context->world_width)
					piece_origin_x-=context->world_width;

				//But not the y
				int piece_origin_y = cluster_origin_province_y + RandomNumberBetween(-radius,radius);
				if(piece_origin_y<0)
					continue;
				if(piece_origin_y>=context->world_height)
					continue;

				//A mildy random piece size
				int piece_radius = (radius/4) + RandomNumberBetween(-radius/15/2, radius/15);

				//The coordinates of the piece
				std::vector<Province*> piece = context->GetDiamondOfProvinces(piece_origin_x,piece_origin_y,piece_radius,true);

				//Making sure this province is not taken by another plate or itself
				for (int m = 0; m < piece.size(); m++)
				{
					if(has_plate[piece[m]->province_y][piece[m]->province_x] == false)
					{
						//Add it to our plate!
						tectonic_plate->provinces_in_plate.push_back(new Vector2(piece[m]->province_x,piece[m]->province_y));
					}
				}
			}

			//	DUPLICATE CLEANING	//
			vector<Vector2*> nonduplicated_provinces;
			for (int w = 0; w < tectonic_plate->provinces_in_plate.size(); w++)
			{
				bool duplicate = false;
				for (int a = 0; a < nonduplicated_provinces.size(); a++)
				{
					if(tectonic_plate->provinces_in_plate[w]->x==nonduplicated_provinces[a]->x && tectonic_plate->provinces_in_plate[w]->y==nonduplicated_provinces[a]->y)
					{
						duplicate = true;
						break;
					}
				}
				if(!duplicate)
				{
					nonduplicated_provinces.push_back(tectonic_plate->provinces_in_plate[w]);
				}
			}
			tectonic_plate->provinces_in_plate.clear();
			for (int l = 0; l < nonduplicated_provinces.size(); l++)
			{
				tectonic_plate->provinces_in_plate.push_back(nonduplicated_provinces[l]);
			}

			//If its not a super tiny plate
			if(tectonic_plate->provinces_in_plate.size()>10)
			{
				//Depth first search to see if its contiguous
				std::vector<Vector2> confirmed_contiguous_provinces;

				Vector2 starting (tectonic_plate->provinces_in_plate[0]->x,tectonic_plate->provinces_in_plate[0]->y);

				TectonicHandler::PlateContiguitySearch(
					starting,
					&tectonic_plate->provinces_in_plate,
					&confirmed_contiguous_provinces);

				//If this tectonic plate checks out
				if(confirmed_contiguous_provinces.size() == tectonic_plate->provinces_in_plate.size())
				{
					//std::cout<<"Plate	CREATED		; it was size "<<tectonic_plate->provinces_in_plate.size()<<"."<<endl;
					std::cout<<provinces_without_plate<<" provinces left to fill."<<endl;

					context->tectonic_plates.push_back(tectonic_plate);
					provinces_without_plate -= tectonic_plate->provinces_in_plate.size();

					for (int p = 0; p < tectonic_plate->provinces_in_plate.size(); p++)
					{
						context->old_plates_on_province[tectonic_plate->provinces_in_plate[p]->y][tectonic_plate->provinces_in_plate[p]->x].push_back(tectonic_plate->plate_number);
						has_plate[tectonic_plate->provinces_in_plate[p]->y][tectonic_plate->provinces_in_plate[p]->x] = true;
					}
				}
				else
				{
					plate_count--;
					reject_count++;
					/*std::cout<<"Plate rejected; lack of contiguity."<<endl;
					std::cout<<provinces_without_plate<<" provinces left to fill."<<endl;*/
				}
			}
			else
			{
				plate_count--;
				reject_count++;
				/*std::cout<<"Plate rejected; it was only size "<<tectonic_plate->provinces_in_plate.size()<<"."<<endl;
				std::cout<<provinces_without_plate<<" provinces left to fill."<<endl;*/
			}
		}
		else
		{
			std::cout<<"Reject max achieved."<<endl;
			break;
		}
	}

	//Meshing together the remaining spots
	std::vector<Province*> provinces_without_plate_neighbors;//Any provinces that have no nearby plates are put into this list and handled later
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			if(context->old_plates_on_province[y][x].size() == 0)
			{
				//The number of times a plate is a neighbor
				std::unordered_map<int,int> neighboring_plate_occurences;
				bool no_nearby_plates = true;

				//Cylcing through neighbors
				std::vector<Province*> neighbors = context->GetDiamondOfProvinces(x,y,1,false);
				for (int p = 0; p < neighbors.size(); p++)
				{
					Province* prov = neighbors[p];
					if(context->old_plates_on_province[prov->province_y][prov->province_x].size()!=0)
					{
						neighboring_plate_occurences[context->old_plates_on_province[prov->province_y][prov->province_x][0]]++;
						no_nearby_plates = false;
					}
				}
				if(no_nearby_plates == false)
				{
					int highest_occurence = 0;
					int chosen_plate = 0;
					//Figuring out which plate occurs the most
					for (auto it = neighboring_plate_occurences.begin(); it != neighboring_plate_occurences.end(); ++it) 
					{
						if(it->second>highest_occurence	)
						{
							highest_occurence = it->second;
							chosen_plate = it->first;
						}
					}
					context->tectonic_plates[chosen_plate]->provinces_in_plate.push_back(new Vector2(x,y));
					context->old_plates_on_province[y][x].push_back(chosen_plate);
					provinces_without_plate--;
				}
				else
				{
					provinces_without_plate_neighbors.push_back(context->provinces[y][x]);
				}
			}
		}
	}


	/*//If a plate is too small we mesh it with other plates
	for (int t = 0; t < tectonic_plates.size(); t++)
	{
	if(tectonic_plates[t]->provinces_in_plate.size() < 10 && tectonic_plates[t]->provinces_in_plate.size() != 0)
	{
	for (int p = 0; p < tectonic_plates[t]->provinces_in_plate.size(); p++)
	{
	//All the neighboring provinces we could give this peice to
	std::vector<Province*> neighboring = GetSquareOfProvinces(tectonic_plates[t]->provinces_in_plate[p]->x,
	tectonic_plates[t]->provinces_in_plate[p]->y,
	1, false,true,false);

	//We try this neighbor
	int attempted_neighbor_index= NULL;
	while(attempted_neighbor_index==NULL)
	{
	int attempted_index= rand()%neighboring.size();
	std::vector<int> plates_on_this_province = new_plates_on_province[neighboring[attempted_neighbor_index]->province_y]
	[neighboring[attempted_neighbor_index]->province_x];

	if(plates_on_this_province.size() >1)
	{
	attempted_neighbor_index = attempted_index;
	}
	else if(plates_on_this_province.size() ==1)
	{
	if(plates_on_this_province[0] != t)
	{
	attempted_neighbor_index = attempted_index;
	}
	}
	}

	//The plates on the neighboring province
	std::vector<int> plates_on_this_province = new_plates_on_province[neighboring[attempted_neighbor_index]->province_y]
	[neighboring[attempted_neighbor_index]->province_x];

	//Add this province coordinate to our neighbors list
	int recipient_plate = -9999;
	while(recipient_plate == -9999)
	{
	int attempted_index = rand()%plates_on_this_province.size();
	if(attempted_index!=t)
	{
	recipient_plate = attempted_index;
	}
	}

	tectonic_plates[recipient_plate]->provinces_in_plate.push_back(
	new Vector2(tectonic_plates[t]->provinces_in_plate[p]->x,
	tectonic_plates[t]->provinces_in_plate[p]->y));
	Draw();
	}
	//Effectively delte this plate
	tectonic_plates[t]->provinces_in_plate.clear();
	}
	}*/

	std::cout<<"Tectonic plates created."<<endl;
};

void TectonicHandler::PlateContiguitySearch(Vector2 myCoordinate, std::vector<Vector2*>* plate_coordinates, std::vector<Vector2>* myConnected)
{
	vector<Vector2>& connected_reference = *myConnected;
	vector<Vector2*>& plate_reference = *plate_coordinates;

	Vector2 north (myCoordinate.x,myCoordinate.y-1);

	Vector2 east (myCoordinate.x+1,myCoordinate.y);

	Vector2 south (myCoordinate.x,  myCoordinate.y+1);

	Vector2 west (myCoordinate.x-1,myCoordinate.y);

	//Go through every province in the plate and see if its a neihgbor of this province
	for (int i = plate_reference.size()-1; i >= 0; i--)
	{
		if(plate_reference[i]->x == north.x && plate_reference[i]->y == north.y)
		{
			//Check if we already ahve it
			bool already_have_it = false;
			for (int c = 0; c < myConnected->size(); c++)
			{
				if(north.x == connected_reference[c].x && north.y == connected_reference[c].y)
				{
					already_have_it=true;
				}
			}
			if(!already_have_it)
			{
				myConnected->push_back((Vector2)(north));
				PlateContiguitySearch(north,&plate_reference,myConnected);
			}
		}
		if(plate_reference[i]->x == east.x && plate_reference[i]->y == east.y)
		{
			//Check if we already ahve it
			bool already_have_it = false;
			for (int c = 0; c < myConnected->size(); c++)
			{
				if(east.x == connected_reference[c].x && east.y == connected_reference[c].y)
				{
					already_have_it=true;
				}
			}
			if(!already_have_it)
			{
				myConnected->push_back((Vector2)(east));
				PlateContiguitySearch(east,&plate_reference,myConnected);
			}
		}
		if(plate_reference[i]->x == south.x && plate_reference[i]->y == south.y)
		{
			//Check if we already ahve it
			bool already_have_it = false;
			for (int c = 0; c < myConnected->size(); c++)
			{
				if(south.x == connected_reference[c].x && south.y == connected_reference[c].y)
				{
					already_have_it=true;
				}
			}
			if(!already_have_it)
			{
				myConnected->push_back((Vector2)(south));
				PlateContiguitySearch(south,&plate_reference,myConnected);
			}
		}
		if(plate_reference[i]->x == west.x && plate_reference[i]->y == west.y)
		{
			//Check if we already ahve it
			bool already_have_it = false;
			for (int c = 0; c < myConnected->size(); c++)
			{
				if(west.x == connected_reference[c].x && west.y == connected_reference[c].y)
				{
					already_have_it=true;
				}
			}
			if(!already_have_it)
			{
				myConnected->push_back((Vector2)(west));
				PlateContiguitySearch(west,&plate_reference,myConnected);
			}
		}
	}

};

void TectonicHandler::CreateWater()
{
	std::cout<<"Creating Water."<<endl;
	int amount_water_needed = context->world_width*context->world_height*100;
	for (int w = 0; w < 100; w++)
	{
		Province* prov = NULL;
		while(prov == NULL)
		{
			int x = RandomNumberBetween(0,context->world_width);
			int y = RandomNumberBetween(0,context->world_height);
			if(context->provinces[y][x]->biome != GRASSLAND)
			{
				prov = context->provinces[y][x];
			}
		}
		prov->water_depth +=amount_water_needed/80;
		prov->biome = WATER;
		TectonicHandler::unresolved_water.push_back(prov);
	}

	std::cout<<"Water created."<<endl;
};

void TectonicHandler::ResolveAllWater()
{
	std::cout<<"Resolving water."<<endl;
	ResolveWaterInProvince(TectonicHandler::unresolved_water[0]);
	while(TectonicHandler::unresolved_water.size()>1)
	{
		ResolveWaterInProvince(TectonicHandler::unresolved_water[TectonicHandler::unresolved_water.size()-1]);
		auto prov = TectonicHandler::unresolved_water.end()-1;
		TectonicHandler::unresolved_water.erase(TectonicHandler::unresolved_water.end()-1);
	}
	std::cout<<"Water resolved"<<endl;
};
void TectonicHandler::ResolveWaterInProvince(Province* prov)
{
	//The province above, to the right, down, and left of our prov BUT NOT the prov itself
	std::vector<Province*> neighboring_provinces = context->GetSquareOfProvinces(prov->province_x,prov->province_y,2,false);

	bool done = false;
	int provinces_checked = 0;

	while(!done)//Keep passing out water till ur done
	{
		if(prov->water_depth > 1)//Do we have water to pass out?
		{
			int steepest_slope = 1;
			int chosen_slope = 0;

			//Finding out which neighbor has the steepest slope
			for (int n = 0; n < neighboring_provinces.size(); n++)
			{
				int difference = prov->getLandAndWaterHeight() - neighboring_provinces[n]->getLandAndWaterHeight();
				if(difference>steepest_slope)
				{
					chosen_slope = n;
					steepest_slope = difference;
				}
			}

			//Passing water to neighbor
			if( steepest_slope >1)
			{
				Province* neighbor = context->provinces[neighboring_provinces[chosen_slope]->province_y][neighboring_provinces[chosen_slope]->province_x];

				//Can we pass half our height difference
				if(prov->water_depth>=(steepest_slope/2))
				{
					//Yes we can
					prov->water_depth-= (steepest_slope/2) + (steepest_slope%2);

					neighbor->biome = WATER;
					neighbor->water_depth+=steepest_slope/2+ (steepest_slope%2);
				}
				else
				{
					//No we cant give enoguht to level them so we give all instead
					neighbor->water_depth+=prov->water_depth;
					neighbor->biome = WATER;
					prov->water_depth = 0;
				}
				TectonicHandler::unresolved_water.push_back(neighbor);
			}
			else
			{
				//Every surrounding province has a difference of 1 or 0
				done=true;
			}
		}
		else
		{
			//We have 1 or 0 water units
			done=true;
		}
	}
};
void TectonicHandler::FlushWater(int number_times)
{
	for (int i = 0; i < number_times; i++)
	{
		for (int x = 0; x <context->world_width; x++)
		{
			for (int y = 0; y <context->world_height; y++)
			{
				unresolved_water.push_back(context->provinces[y][x]);
			}
		}
		ResolveAllWater();
	}
};

void TectonicHandler::Erode()
{
};

void TectonicHandler::AdvanceTectonics()
{
	//		CLEAR OLD CHANGES		//
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			context->new_plates_on_province[y][x].clear();
			pending_altitude_changes[y][x] = 0;
		}
	}

	//		MOVE EACH PLATE		//
	for (int t = 0; t < context->tectonic_plates.size(); t++)
	{
		//If it isn't moving get it a new direction
		if(context->tectonic_plates[t]->x_velocity == -9999)
		{
			context->tectonic_plates[t]->x_velocity = -1 + rand()%3;
			context->tectonic_plates[t]->y_velocity = -1 + rand()%3;
		}

		//Populating where the plates move to
		for (int p = 0; p < context->tectonic_plates[t]->provinces_in_plate.size(); p++)
		{
			Province* province = context->provinces[context->tectonic_plates[t]->provinces_in_plate[p]->y][context->tectonic_plates[t]->provinces_in_plate[p]->x];

			int wrapped_x = context->tectonic_plates[t]->provinces_in_plate[p]->x+context->tectonic_plates[t]->x_velocity;
			int wrapped_y = context->tectonic_plates[t]->provinces_in_plate[p]->y+context->tectonic_plates[t]->y_velocity;

			context->WrapCoordinates(&wrapped_x,&wrapped_y);

			context->new_plates_on_province[wrapped_y][wrapped_x].push_back(context->tectonic_plates[t]->plate_number);
		}
	}

	//								//
	//		CONFLICT RESOLUTION		//
	//								//
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			//The plates that are conflicting on this province
			std::vector<int> tectonic_plate_conflicts = context->new_plates_on_province[y][x];

			if(tectonic_plate_conflicts.size() >1)//If there is indeed a conflict
			{
				//Is it just a duplicate?
				std::vector<int> duplicates;

				//Removing duplicates -- why would we have duplicates!?
				for (int i = 0; i < tectonic_plate_conflicts.size(); i++)
				{
					bool already_have = false;
					for (int q = 0; q < duplicates.size(); q++)
					{
						if(duplicates[q] ==tectonic_plate_conflicts[i])
						{
							already_have = true;
						}
					}
					if(!already_have)
					{
						duplicates.push_back(tectonic_plate_conflicts[i]);
					}
				}
				tectonic_plate_conflicts = duplicates;

				//Do we still have conflict
				if(tectonic_plate_conflicts.size() >1)
				{

					//The provinces that tried ot move onto this province
					std::vector<Province*> province_conflicts;

					//Pulling the competing provinces
					for (int c = 0; c < tectonic_plate_conflicts.size(); c++)
					{
						int wrapped_x = x-context->tectonic_plates[tectonic_plate_conflicts[c]]->x_velocity;
						int wrapped_y = y-context->tectonic_plates[tectonic_plate_conflicts[c]]->y_velocity;

						context->WrapCoordinates(&wrapped_x,&wrapped_y);

						//If that old province did not contain our plate then we need to look in the other direction
						if(!context->old_plates_on_province[wrapped_y][wrapped_x][0] == tectonic_plate_conflicts[c])
						{
							wrapped_x = x+context->tectonic_plates[tectonic_plate_conflicts[c]]->x_velocity;
							wrapped_y = y+context->tectonic_plates[tectonic_plate_conflicts[c]]->y_velocity;
							context->WrapCoordinates(&wrapped_x,&wrapped_y);
						}

						Province* old_plate_province_pos = context->provinces[wrapped_y][wrapped_x];
						province_conflicts.push_back(old_plate_province_pos);
					}

					//												//
					//		WHICH PLATE DOES NOT GET SUBDUCTED		//
					//												//

					int highest_density = INT_MIN;
					int highest_value_index= - 9990;

					for (int c = 0; c < province_conflicts.size(); c++)
					{
						//Determining which plate will not get subducted and destroyed
						int plate_density = CalculatePlateDensity(context->tectonic_plates[c]);
						if(plate_density >= highest_density)
						{
							highest_density = plate_density;
							highest_value_index = tectonic_plate_conflicts[c];
						}
					}

					//Highest one doesnt get deleted
					context->new_plates_on_province[y][x].clear();
					context->new_plates_on_province[y][x].push_back(highest_value_index);
				}
				if(context->new_plates_on_province[y][x].size() !=0)
				{
					//Now move the altitude from the chosen plate
					Vector2* old_position =new Vector2(
						x - context->tectonic_plates[context->new_plates_on_province[y][x][0]]->x_velocity,
						y - context->tectonic_plates[context->new_plates_on_province[y][x][0]]->y_velocity);
					context->WrapCoordinates(old_position);

					Province* previous_province_position = context->provinces[old_position->y][old_position->x];

					pending_altitude_changes[y][x]+=previous_province_position->altitude;

					pending_altitude_changes[old_position->y][old_position->x]-= previous_province_position->altitude;
					delete(old_position);
				}
			}
		}
	}

	//Apply the height changes
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			context->provinces[y][x]->altitude += pending_altitude_changes[y][x];
		}
	}

	//Fill in teh gaps!
	std::vector<Province*> provinces_without_plate_neighbors;//Any provinces that have no nearby plates are put into this list and handled later
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			if(context->new_plates_on_province[y][x].size() == 0)
			{
				//Perhaps only add altitude if there is water...
				if(context->provinces[y][x]->altitude==0)
				{
					context->provinces[y][x]->altitude+= RandomNumberBetween(5,16);//Magma is melting! add some altitude
				}

				//The number of times a plate is a neighbor
				std::unordered_map<int,int> neighboring_plate_occurences;
				bool no_nearby_plates = true;

				//Cylcing through neighbors
				std::vector<Province*> neighbors = context->GetDiamondOfProvinces(x,y,1,false);
				for (int p = 0; p < neighbors.size(); p++)
				{
					Province* prov = neighbors[p];
					if(context->new_plates_on_province[prov->province_y][prov->province_x].size()!=0)
					{
						neighboring_plate_occurences[context->new_plates_on_province[prov->province_y][prov->province_x][0]]++;
						no_nearby_plates = false;
					}
				}
				if(no_nearby_plates == false)
				{
					int highest_occurence = 0;
					int chosen_plate = 0;
					//Figuring out which plate occurs the most
					for (auto it = neighboring_plate_occurences.begin(); it != neighboring_plate_occurences.end(); ++it) 
					{
						if(it->second>highest_occurence	)
						{
							highest_occurence = it->second;
							chosen_plate = it->first;
						}
					}
					context->tectonic_plates[chosen_plate]->provinces_in_plate.push_back(new Vector2(x,y));
					context->new_plates_on_province[y][x].push_back(chosen_plate);
				}
				else
				{
					provinces_without_plate_neighbors.push_back(context->provinces[y][x]);
				}
			}
		}
	}


	//Clear out what the tectonic plate was
	for (int t = 0; t < context->tectonic_plates.size(); t++)
	{
		context->tectonic_plates[t]->provinces_in_plate.clear();
	}

	//Now recreate the plates
	for (int x = 0; x < context->world_width; x++)
	{
		for (int y = 0; y < context->world_height; y++)
		{
			if(context->new_plates_on_province[y][x].size()>0)
			{
				context->tectonic_plates[context->new_plates_on_province[y][x][0]]->provinces_in_plate.push_back(new Vector2(x,y));//Where it is now
			}
			if(context->provinces[y][x]->water_depth>0)
				TectonicHandler::unresolved_water.push_back(context->provinces[y][x]);

		}
	}

	//RESOLVING WATER
	for (int x = 0; x < context->world_width; x++)
	{
		for (int y = 0; y < context->world_height; y++)
		{
			if(context->provinces[y][x]->water_depth>0)
				TectonicHandler::unresolved_water.push_back(context->provinces[y][x]);
		}
	}
	ResolveAllWater();

	//Any uncovered land becomes grassland
	for (int y= 0; y < context->world_height; y++)
	{
		for (int x= 0; x < context->world_width; x++)
		{
			if(context->provinces[y][x]->water_depth==0)
			{
				context->provinces[y][x]->biome = GRASSLAND;
			}
		}
	}

	//Update the new to the old 
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			context->old_plates_on_province[y][x].clear();
			context->old_plates_on_province[y][x].push_back(context->new_plates_on_province[y][x][0]);
		}
	}

	//Resolve new plates speeds
};
int TectonicHandler::CalculatePlateDensity(TectonicPlate* myPlate)
{
	int total_landmass = 0;
	int total_water = 0;
	for (int p = 0; p < myPlate->provinces_in_plate.size(); p++)
	{
		//This method compares the all land to the all water provinces and considers water provinces fully cool
		if(context->provinces[myPlate->provinces_in_plate[p]->y][myPlate->provinces_in_plate[p]->x]->water_depth==0)
		{
			total_landmass++;
		}
		else
		{
			total_water++;
		}

		//This method compares teh total amount of land vs the total amount of water for different ranges
		//total_landmass+=context->provinces[myPlate->provinces_in_plate[p]->y][myPlate->provinces_in_plate[p]->x]->altitude;
		//total_water+=context->provinces[myPlate->provinces_in_plate[p]->y][myPlate->provinces_in_plate[p]->x]->water_depth;
	}

	return total_landmass/total_water;
};


