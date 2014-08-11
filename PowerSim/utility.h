#ifndef __WS_UTILITY__
#define __WS_UTILITY__


using namespace std;

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <stdexcept>
#include <sstream>
#include <thread>
#include <stdlib.h> 
#include <ctime>
#include <mutex>
#include <string>
#include <algorithm>  
#include <ctype.h>

int RandomNumberBetween(int myLowerIncusive, int myUpperExclusive);

std::string CreateGibberishName(int myNumberLetters);

class Vector2
{
public:
	double x,y;
	Vector2():x(0),y(0){};
	Vector2(double myX,double myY)
	{
		x = myX;
		y = myY;
	}

	bool Equals(Vector2 first, Vector2 second)
	{
		return (first.x==second.x && first.y==second.y);
	}
	static double ToMathRadian(Vector2 myVector)
	{
		return atan2(myVector.y,myVector.x);
	};
	static Vector2 ToVector2(double angle)
	{
		return Vector2 (cos(angle), sin(angle));
	};

	Vector2 &operator+=(Vector2 other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Vector2 operator+(Vector2 other)
	{
		Vector2 ret(*this);
		ret += other;
		return ret;
	}
	Vector2 &operator-=(Vector2 other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	Vector2 operator-(Vector2 other)
	{
		Vector2 ret(*this);
		ret -= other;
		return ret;
	}

	static double getDistanceBetween(Vector2 first, Vector2 second)
	{
		return sqrt( (second.x-first.x)*(second.x-first.x) + (second.y-first.y)*(second.y-first.y));
	}

	static Vector2 Normalize(Vector2 myVector)
	{
		if(!(myVector.x ==0 && myVector.y==0))
		{
			float length = sqrt( (myVector.x*myVector.x) + (myVector.y*myVector.y));

			return Vector2(myVector.x/length,myVector.y/length);
		}
		else
		{
			return Vector2(0,0);
		}
	}
	double GetLength()
	{
		return sqrt((x*x)+(y*y));
	}
	void Normalize()
	{
		if(!(x ==0 && y==0))
		{
			float length = sqrt((x*x) + (y*y));

			x = x/length;
			y = y/length;
		}
	}
};


#endif