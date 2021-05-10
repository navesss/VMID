

#ifndef __VOXEL_H_
#define __VOXEL_H_

//#include "stdafx.h"

//my includes:
#include "Useful.h"
#include "DataArray.h"
#include "Configuration.h"
#include "Rotation.h"

//CLASS RotationAndBaseCase
typedef struct
{
	int Rot;
	int BaseCase;
} RotationAndBaseCase;

//CLASS Voxel
//member variables:
typedef struct
{
	Configuration MyConfig;
	Configuration TestConfig;

	int i, j, k;
	DataArray *MyDataArray;

	Configuration BaseConfig[23];
	Rotation BaseRotat[24];
	RotationAndBaseCase MapToBase[256];

	Configuration MyBaseConfig;
	Rotation MyBaseRotation;
} Voxel;

//CLASS Voxel
//member functions:
void Voxel_SetParams(Voxel *This, DataArray *ADataArray);
void Voxel_reset8(Voxel *This, MH_BOOL corner0, MH_BOOL corner1, MH_BOOL corner2, MH_BOOL corner3, 
	MH_BOOL corner4, MH_BOOL corner5, MH_BOOL corner6, MH_BOOL corner7,
	int Ai, int Aj, int Ak);
void Voxel_reset3(Voxel *This, int Ai, int Aj, int Ak);
TriangleList Voxel_OutputTriangles(Voxel *This);//DataArray *MyDataArray);
int Voxel_GetEdgePoint(Voxel *This, int corner1, int corner2);
Triangle *Voxel_NewTriangle(Voxel *This, int c1a, int c1b, int c2a, int c2b, int c3a, int c3b);

#endif //__VOXEL_H_
