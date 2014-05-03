#ifndef Province_H
#define Province_H

#include "Vector2.h"

class Province
{

public:
	Province(Vector2* myP0,Vector2* myP1,Vector2* myP2,Vector2* myP3)
	{
		p0 = myP0;
		p1 = myP1;
		p2 = myP2;
		p3 = myP3;
	};

	Vector2* p0;
	Vector2* p1;
	Vector2* p2;
	Vector2* p3;

	int altitude;
	int vegetation;


//	Province(int x0,int y0,
//			 int x1,int y1,
//			 int x2,int y2,
//			 int x3,int y3)
//	{
//		vertices[0] =x0;
//		vertices[1] =y0;
//		vertices[2] =x1;
//		vertices[3] =y1;
//		vertices[4] =x2;
//		vertices[5] =y2;
//		vertices[6] =x3;
//		vertices[7] =y3;
//
//		altitude = 0;
//		vegetation = 0;
//
//	};
//
//	int vertices[8];
//
//	int altitude;
//	int vegetation;
};

#endif