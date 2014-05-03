#ifndef Resource_H
#define Resource_H

class Resource
{
public:
	Resource(int myProvinceX,int myProvinceY,int myX,int myY,int myNecessityValue,int myDomesticabilty)
	{
		province_x = myProvinceX;
		province_y = myProvinceY;
		position_x = myX;
		position_y = myY;
		necessity_value = myNecessityValue;
		domesticability = myDomesticabilty;
	};
	int position_x;
	int position_y;

	int province_x;
	int province_y;

	int necessity_value;//10 is necessary for life, 0 is kings lux
	int domesticability;//10 is max difficulty
};

#endif