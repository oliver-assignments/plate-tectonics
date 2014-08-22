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
	std::cout<<endl<<"Creating tectonic plates."<<endl;

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

		std::vector<bool> old_row_of_flipped_provinces;
		context->old_flipped_provinces.push_back(old_row_of_flipped_provinces);

		std::vector<bool> row_of_flipped_provinces;
		context->new_flipped_provinces.push_back(row_of_flipped_provinces);

		for (int x = 0; x < context->world_width; x++)
		{
			has_plate[y].push_back(false);
			pending_altitude_changes[y].push_back(0);
			context->asthenosphere_heat_map[y].push_back(0);
			context->new_plates_on_province[y].push_back(*new std::vector<int>);
			context->old_plates_on_province[y].push_back(*new std::vector<int>);
			context->new_flipped_provinces[y].push_back(false);
			context->old_flipped_provinces[y].push_back(false);
		}
	}

	//CREATING ALL THE PLATES
	while(provinces_without_plate>0)
	{
		if(reject_count<1000)
		{
			int radius = (context->world_width+context->world_height)/15;

			TectonicPlate* tectonic_plate = new TectonicPlate();
			tectonic_plate->plate_number = plate_count;
			plate_count++;

			//Finding a good blob origin that isnt already taken
			int cluster_origin_province_x=-1;
			int cluster_origin_province_y=-1;

			//Finding a starting location
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
				//radius =(context->world_width+context->world_height)/RandomNumberBetween(6,15);
				int piece_radius = radius/2;

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
			int duplicate_count = 0;
			vector<Vector2*> nonduplicated_provinces;
			for (int w = 0; w < tectonic_plate->provinces_in_plate.size(); w++)
			{
				bool duplicate = false;
				for (int a = 0; a < nonduplicated_provinces.size(); a++)
				{
					if(tectonic_plate->provinces_in_plate[w]->x==nonduplicated_provinces[a]->x && tectonic_plate->provinces_in_plate[w]->y==nonduplicated_provinces[a]->y)
					{
						duplicate = true;
						duplicate_count++;
						break;
					}
				}
				if(!duplicate)
				{
					nonduplicated_provinces.push_back(tectonic_plate->provinces_in_plate[w]);
				}
			}
			//Replacing tectonic plate province list with nonduplicate version
			tectonic_plate->provinces_in_plate.clear();
			for (int l = 0; l < nonduplicated_provinces.size(); l++)
			{
				tectonic_plate->provinces_in_plate.push_back(nonduplicated_provinces[l]);
			}

			//If its not a super tiny plate
			if(tectonic_plate->provinces_in_plate.size()>5)
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
					//std::cout<<provinces_without_plate<<" provinces left to fill."<<endl;

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
					//Not contiguous
					plate_count--;
					reject_count++;
				}
			}
			else
			{
				//To small
				plate_count--;
				reject_count++;
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
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			context->asthenosphere_heat_map[y][x] += context->provinces[y][x]->altitude;
			context->asthenosphere_heat_map[y][x] += context->provinces[y][x]->water_depth;
		}
	}

	std::cout<<"Tectonic plates created."<<endl;
};

void TectonicHandler::PlateContiguitySearch(Vector2 myCoordinate, std::vector<Vector2*>* plate_coordinates, std::vector<Vector2>* myConnected)
{
	vector<Vector2>& connected_reference = *myConnected;
	vector<Vector2*>& plate_reference = *plate_coordinates;

	Vector2 north (myCoordinate.x,myCoordinate.y-1);

	Vector2 east (myCoordinate.x+1,myCoordinate.y);
	TectonicHandler::context->WrapCoordinates(&east);

	Vector2 south (myCoordinate.x,  myCoordinate.y+1);

	Vector2 west (myCoordinate.x-1,myCoordinate.y);
	TectonicHandler::context->WrapCoordinates(&west);

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
	std::cout<<endl<<"Creating Water."<<endl;
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
	ResolveWaterInProvince(TectonicHandler::unresolved_water[0]);
	while(TectonicHandler::unresolved_water.size()>1)
	{
		ResolveWaterInProvince(TectonicHandler::unresolved_water[TectonicHandler::unresolved_water.size()-1]);
		auto prov = TectonicHandler::unresolved_water.end()-1;
		TectonicHandler::unresolved_water.erase(TectonicHandler::unresolved_water.end()-1);
	}
};
void TectonicHandler::ResolveWaterInProvince(Province* prov)
{
	//The province above, to the right, down, and left of our prov BUT NOT the prov itself
	std::vector<Province*> neighboring_provinces = context->GetDiamondOfProvinces(prov->province_x,prov->province_y,2,false);

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
	int hottest_asthenosphere = 0;
	Vector2 hottest_asthenosphere_location (0,0);
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			//context->asthenosphere_heat_map[y][x] = 0;
			context->asthenosphere_heat_map[y][x] += context->provinces[y][x]->altitude;
			context->asthenosphere_heat_map[y][x] -= context->provinces[y][x]->water_depth;

			if(context->asthenosphere_heat_map[y][x]>hottest_asthenosphere)
			{
				if( y !=0 && y != (context->world_height-1))
				{
					hottest_asthenosphere = context->asthenosphere_heat_map[y][x];
					hottest_asthenosphere_location.x = x;
					hottest_asthenosphere_location.y = y;
				}
			}
		}
	}

	//		MOVE EACH PLATE		//
	for (int t = 0; t < context->tectonic_plates.size(); t++)
	{
		/*if(context->tectonic_plates[t]->x_velocity == -9999)
		{
		context->tectonic_plates[t]->x_velocity = RandomNumberBetween(-1,2);
		context->tectonic_plates[t]->y_velocity = RandomNumberBetween(-1,2);

		while(context->tectonic_plates[t]->x_velocity==0 && context->tectonic_plates[t]->y_velocity==0)
		{
		context->tectonic_plates[t]->x_velocity = RandomNumberBetween(-1,2);
		context->tectonic_plates[t]->y_velocity = RandomNumberBetween(-1,2);
		}
		}*/

		//Setting direction of plate
		if((hottest_asthenosphere - CalculateAverageAsthenosphereTemperature(context->tectonic_plates[t])) > (hottest_asthenosphere/2))
		{
			Vector2 plate_center = CalculatePlateCenter(context->tectonic_plates[t]);
			Vector2 difference_in_centers (hottest_asthenosphere_location.x - plate_center.x, hottest_asthenosphere_location.y - plate_center.y);

			if(difference_in_centers.x >0)
				context->tectonic_plates[t]->x_velocity = -1;
			else if(difference_in_centers.x ==0)
				context->tectonic_plates[t]->x_velocity = 0;
			else
				context->tectonic_plates[t]->x_velocity = 1;

			if(difference_in_centers.y >0)
				context->tectonic_plates[t]->y_velocity = -1;
			else if(difference_in_centers.y ==0)
				context->tectonic_plates[t]->y_velocity = 0;
			else
				context->tectonic_plates[t]->y_velocity = 1;
		}
		else
		{
			context->tectonic_plates[t]->x_velocity = 0;
			context->tectonic_plates[t]->y_velocity = 0;
		}


		//Logic behind plate movement here

		//Heat convection?

		//Populating where the plates move to
		for (int p = 0; p < context->tectonic_plates[t]->provinces_in_plate.size(); p++)
		{
			int where_it_was_x = context->tectonic_plates[t]->provinces_in_plate[p]->x;
			int where_it_was_y = context->tectonic_plates[t]->provinces_in_plate[p]->y;

			int wrapped_x;
			int wrapped_y;
			if(context->old_flipped_provinces[context->tectonic_plates[t]->provinces_in_plate[p]->y][context->tectonic_plates[t]->provinces_in_plate[p]->x] == false)
			{
				wrapped_x = where_it_was_x + context->tectonic_plates[t]->x_velocity;
				wrapped_y = where_it_was_y + context->tectonic_plates[t]->y_velocity;
			}
			else
			{
				//This is a flipped version of the plate aroudn the north or south go in the other direction
				wrapped_x = where_it_was_x - context->tectonic_plates[t]->x_velocity;
				wrapped_y = where_it_was_y - context->tectonic_plates[t]->y_velocity;
			}

			context->WrapCoordinates(&wrapped_x,&wrapped_y);

			//Don't add it if we already have it
			bool already_have_this_plate = false;
			for (int q = 0; q < context->new_plates_on_province[wrapped_y][wrapped_x].size(); q++)
				if(context->new_plates_on_province[wrapped_y][wrapped_x][q] == t)
					already_have_this_plate = true;

			if(!already_have_this_plate)
			{
				context->new_plates_on_province[wrapped_y][wrapped_x].push_back(t);
			}
		}
	}

	//		CONFLICT RESOLUTION		//
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			if(context->new_plates_on_province[y][x].size()>0)
			{
				//The plates that are conflicting on this province
				std::vector<int> tectonic_plate_conflicts = context->new_plates_on_province[y][x];

				int non_subducting_plate = tectonic_plate_conflicts[0];

				//Do we still have conflict
				if(tectonic_plate_conflicts.size() > 1)
				{
					int highest_density = INT_MIN;

					for (int c = 0; c < tectonic_plate_conflicts.size(); c++)
					{
						//Determining which plate will not get subducted and destroyed
						int plate_density = CalculatePlateDensity(context->tectonic_plates[c]);
						if(plate_density >= highest_density)
						{
							highest_density = plate_density;
							non_subducting_plate = tectonic_plate_conflicts[c];
						}
					}
				}

				//Subtract from all moved plates but only add if its the winning plate
				for (int i = 0; i < tectonic_plate_conflicts.size(); i++)
				{
					Vector2 old_position(
						x - context->tectonic_plates[tectonic_plate_conflicts[i]]->x_velocity,
						y - context->tectonic_plates[tectonic_plate_conflicts[i]]->y_velocity);

					context->WrapCoordinates(&old_position);

					bool flipped = false;

					//If that aint our plate go the other way
					if(tectonic_plate_conflicts[i] != context->tectonic_plates[context->old_plates_on_province[old_position.y][old_position.x][0]]->plate_number)
					{
						flipped = true;
						old_position.x= x + context->tectonic_plates[context->new_plates_on_province[y][x][0]]->x_velocity;
						old_position.y= y + context->tectonic_plates[context->new_plates_on_province[y][x][0]]->y_velocity;

						context->WrapCoordinates(&old_position);
					}

					int altitude_move = context->provinces[old_position.y][old_position.x]->altitude;

					//Now move the altitude from the chosen plate
					if(tectonic_plate_conflicts[i] == non_subducting_plate)
					{
						pending_altitude_changes[y][x] += altitude_move;

						if(flipped)
						{
							if(context->new_flipped_provinces[y][x] == true)
								context->new_flipped_provinces[y][x] =false;
							else
								context->new_flipped_provinces[y][x] = true;
						}
					}
					else
					{
						pending_altitude_changes[y][x] += altitude_move/3;
					}
					pending_altitude_changes[old_position.y][old_position.x] -=  altitude_move;
				}
			}
		}
	}
	//Apply the height changes
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			if(pending_altitude_changes[y][x]!=0)
			{
				context->provinces[y][x]->altitude += pending_altitude_changes[y][x];
				pending_altitude_changes[y][x] =0;
			}
		}
	}

	//		Fill in the gaps!		//
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

				//Make it more likely for the old plate to take over the space
				neighboring_plate_occurences[context->old_plates_on_province[y][x][0]]++;

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
		for (int i = 0; i < context->tectonic_plates[t]->provinces_in_plate.size(); i++)
		{
			delete(context->tectonic_plates[t]->provinces_in_plate[i]);
		}
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

	//Update the new to the old 
	for (int y = 0; y < context->world_height; y++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			context->old_plates_on_province[y][x].clear();
			context->old_plates_on_province[y][x].push_back(context->new_plates_on_province[y][x][0]);
			context->new_plates_on_province[y][x].clear();

			context->old_flipped_provinces[y][x] = context->new_flipped_provinces[y][x];
			context->new_flipped_provinces[y][x] = false;

		}
	}

	//RESOLVING WATER
	for (int i = 0; i < 10; i++)
	{
		for (int x = 0; x < context->world_width; x++)
		{
			for (int y = 0; y < context->world_height; y++)
			{
				if(context->provinces[y][x]->water_depth>0)
					TectonicHandler::unresolved_water.push_back(context->provinces[y][x]);
			}
		}
		ResolveAllWater();
	}


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
	if(total_landmass == 0){return 0 ;}

	if(total_water!=0)
		return total_landmass/total_water;
	else
	{
		return total_landmass;
	}
};
Vector2 TectonicHandler::CalculatePlateCenter(TectonicPlate* myPlate)
{
	Vector2 total (0,0);

	for (int i = 0; i < myPlate->provinces_in_plate.size(); i++)
	{
		total.x += myPlate->provinces_in_plate[i]->x;
		total.y += myPlate->provinces_in_plate[i]->y;
	}
	total.x/=myPlate->provinces_in_plate.size();
	total.y/=myPlate->provinces_in_plate.size();

	return total;
};
int TectonicHandler::CalculateAverageAsthenosphereTemperature(TectonicPlate* myPlate)
{
	if(myPlate->provinces_in_plate.size()>0){
		int total_temperature = 0;

		for (int i = 0; i < myPlate->provinces_in_plate.size(); i++)
		{
			total_temperature += context->provinces[myPlate->provinces_in_plate[i]->y][myPlate->provinces_in_plate[i]->x]->altitude;
			total_temperature -= context->provinces[myPlate->provinces_in_plate[i]->y][myPlate->provinces_in_plate[i]->x]->water_depth;
		}

		return total_temperature/ myPlate->provinces_in_plate.size();
	}
	else
	{
		return 0;
	}
};


