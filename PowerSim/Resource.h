#ifndef Resource_H
#define Resource_H

class Resource
{
public:
	Resource(int myX,int myY,int myNecessityValue)
	{
		position_x = myX;
		position_y = myY;
		necessity_value = myNecessityValue;
	};
	int position_x;
	int position_y;
	int necessity_value;//10 is necessary for life, 0 is kings lux
};

#endif