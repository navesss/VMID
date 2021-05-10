

#include "Voxel.h"
#include "math.h"

//class Voxel:
void Voxel_SetParams(Voxel *This, DataArray *ADataArray)
{
	int i,j,k;
	MH_BOOL found;
	Configuration CurrentConfig, RotatedBCase;

	This->MyDataArray = ADataArray;

	//Initialize the cases
	//initialize to base configurations
	//Configuration BaseConfig[23];
	Configuration_reset1(&(This->BaseConfig[0]),0);		//00000000	
	Configuration_reset1(&(This->BaseConfig[1]),1);		//00000001
	Configuration_reset1(&(This->BaseConfig[2]),3);		//00000011
	Configuration_reset1(&(This->BaseConfig[3]),33);	//00100001
	Configuration_reset1(&(This->BaseConfig[4]),35);	//00100011	
	Configuration_reset1(&(This->BaseConfig[5]),51);	//00110011
	Configuration_reset1(&(This->BaseConfig[6]),36);	//00100100
	Configuration_reset1(&(This->BaseConfig[7]),37);	//00100101
	Configuration_reset1(&(This->BaseConfig[8]),39);	//00100111	
	Configuration_reset1(&(This->BaseConfig[9]),22);	//00010110
	Configuration_reset1(&(This->BaseConfig[10]),23);	//00010111				
	Configuration_reset1(&(This->BaseConfig[11]),53);	//00110101	
	Configuration_reset1(&(This->BaseConfig[12]),54);	//00110110
	Configuration_reset1(&(This->BaseConfig[13]),55);	//00110111
	Configuration_reset1(&(This->BaseConfig[14]),60);	//00111100
	Configuration_reset1(&(This->BaseConfig[15]),61);	//00111101
	Configuration_reset1(&(This->BaseConfig[16]),63);	//00111111
	Configuration_reset1(&(This->BaseConfig[17]),150);	//10010110
	Configuration_reset1(&(This->BaseConfig[18]),151);	//10010111
	Configuration_reset1(&(This->BaseConfig[19]),183);	//10110111
	Configuration_reset1(&(This->BaseConfig[20]),189);	//10111101
	Configuration_reset1(&(This->BaseConfig[21]),191);	//10111111
	Configuration_reset1(&(This->BaseConfig[22]),255);	//11111111

	//initialize to base rotations
	//Rotation BaseRotat[24];
	//error in case #3
	Rotation_reset(&(This->BaseRotat[0]),0,1,2,3,4,5,6,7);Rotation_reset(&(This->BaseRotat[1]),4,5,0,1,6,7,2,3);
	Rotation_reset(&(This->BaseRotat[2]),6,7,4,5,2,3,0,1);Rotation_reset(&(This->BaseRotat[3]),2,3,6,7,0,1,4,5);
	Rotation_reset(&(This->BaseRotat[4]),1,5,3,7,0,4,2,6);Rotation_reset(&(This->BaseRotat[5]),5,4,7,6,1,0,3,2);
	Rotation_reset(&(This->BaseRotat[6]),4,0,6,2,5,1,7,3);Rotation_reset(&(This->BaseRotat[7]),2,0,3,1,6,4,7,5);
	Rotation_reset(&(This->BaseRotat[8]),3,2,1,0,7,6,5,4);Rotation_reset(&(This->BaseRotat[9]),1,3,0,2,5,7,4,6);
	Rotation_reset(&(This->BaseRotat[10]),5,7,1,3,4,6,0,2);Rotation_reset(&(This->BaseRotat[11]),7,6,3,2,5,4,1,0);
	Rotation_reset(&(This->BaseRotat[12]),6,4,2,0,7,5,3,1);Rotation_reset(&(This->BaseRotat[13]),7,3,5,1,6,2,4,0);
	Rotation_reset(&(This->BaseRotat[14]),2,6,0,4,3,7,1,5);Rotation_reset(&(This->BaseRotat[15]),3,1,7,5,2,0,6,4);
	Rotation_reset(&(This->BaseRotat[16]),1,0,5,4,3,2,7,6);Rotation_reset(&(This->BaseRotat[17]),0,2,4,6,1,3,5,7);
	Rotation_reset(&(This->BaseRotat[18]),0,4,1,5,2,6,3,7);Rotation_reset(&(This->BaseRotat[19]),4,6,5,7,0,2,1,3);
	Rotation_reset(&(This->BaseRotat[20]),6,2,7,3,4,0,5,1);Rotation_reset(&(This->BaseRotat[21]),3,7,2,6,1,5,0,4);
	Rotation_reset(&(This->BaseRotat[22]),7,5,6,4,3,1,2,0);Rotation_reset(&(This->BaseRotat[23]),5,1,4,0,7,3,6,2);

	//initialise the MapToBase array
	for (i=0 ; i<256 ; i++)
	//for each possible case
	{
		This->MapToBase[i].BaseCase = -1; //so that the case is -1 if not found
		Configuration_reset1(&CurrentConfig,i);	//set CurrentConfig to configuration #i
		found = 0;
		j=0;
		while (j<23 && !found)
		//for each possible base configuration
		{
			k=0;
			while (k<24 && !found)
			//for each possible rotation
			{
				RotatedBCase = This->BaseConfig[j];
				Configuration_Apply(&RotatedBCase,&(This->BaseRotat[k]));
				if (Configuration_IsSame(&RotatedBCase,&CurrentConfig))
				{
					This->MapToBase[i].Rot = k;
					This->MapToBase[i].BaseCase = j;
					found = 1;
				}
				k++;
			}
			j++;
		}
	}
}

void Voxel_reset8(Voxel *This, MH_BOOL corner0, MH_BOOL corner1, MH_BOOL corner2, MH_BOOL corner3,
	MH_BOOL corner4, MH_BOOL corner5, MH_BOOL corner6, MH_BOOL corner7, int Ai, int Aj, int Ak)
{
	Configuration_reset8(&(This->MyConfig),corner0, corner1, corner2, corner3,
					corner4, corner5, corner6, corner7);
	This->i = Ai; This->j = Aj; This->k = Ak;
}

void Voxel_reset3(Voxel *This, int Ai, int Aj, int Ak)
{
	MH_BOOL corner0, corner1, corner2, corner3;
	MH_BOOL corner4, corner5, corner6, corner7;

	This->i = Ai; This->j = Aj; This->k = Ak;

	corner0 = This->MyDataArray->ValueAboveT(This->i, This->j, This->k);
	corner1 = This->MyDataArray->ValueAboveT(This->i+1, This->j, This->k);
	corner2 = This->MyDataArray->ValueAboveT(This->i, This->j+1, This->k);
	corner3 = This->MyDataArray->ValueAboveT(This->i+1, This->j+1, This->k);
	corner4 = This->MyDataArray->ValueAboveT(This->i, This->j, This->k+1);
	corner5 = This->MyDataArray->ValueAboveT(This->i+1, This->j, This->k+1);
	corner6 = This->MyDataArray->ValueAboveT(This->i, This->j+1, This->k+1);
	corner7 = This->MyDataArray->ValueAboveT(This->i+1, This->j+1, This->k+1);

	Configuration_reset8(&(This->MyConfig),corner0,
					corner1,
					corner2,
					corner3,
					corner4,
					corner5,
					corner6,
					corner7
	);
	//printf("voxel %i%i%i%i%i%i%i%i\n",corner0, corner1, corner2, corner3,
	//	corner4, corner5, corner6, corner7);
}

TriangleList Voxel_OutputTriangles(Voxel *This)
//returns a list of Triangles
{
	TriangleList ToBeReturned = TriangleList_TriangleList();

	This->MyBaseConfig = This->BaseConfig[This->MapToBase[Configuration_ToInt(&(This->MyConfig))].BaseCase];
	//the base configuration that corresponds to this one
	This->MyBaseRotation = This->BaseRotat[This->MapToBase[Configuration_ToInt(&(This->MyConfig))].Rot];
	//the rotation that brings us from the base configuration
	//to the current one.

	This->TestConfig = This->MyBaseConfig;
	Configuration_Apply(&(This->TestConfig),&(This->MyBaseRotation));
	
	//printf("nb case : %i\n",This->MapToBase[Configuration_ToInt(&(This->MyConfig))].BaseCase);

	switch (This->MapToBase[Configuration_ToInt(&(This->MyConfig))].BaseCase)
	{
	case 0:
		//no triangles to add
		break;
	case 1:
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,2,	0,4,	0,1));
		break;
	case 2:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,2,	0,4,	1,5));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,2,	1,5,	1,3));
		}
		break;
	case 3:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,2,	0,4,	0,1));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,7,5,	5,1,	4,5));
		}
		break;
	case 4:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,4,	4,5,	5,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,4,	5,7,	0,2));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,2,	5,7,	1,3));
		}
		break;
	case 5:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	1,3,	0,2));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,1,3,	4,6,	5,7));
		}
		break;
	case 6:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	0,2,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,5,7,	1,5,	4,5));
		}
		break;
	case 7:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	0,4,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,3,	0,4,	0,1));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,5,7,	1,5,	4,5));
		}
		break;

	case 8:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	1,3,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,5,	1,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	0,4,	4,5));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,1,3,	4,5,	5,7));
		}
		break;
	case 9:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	2,0,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,1,3,	0,1,	1,5));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	4,5,	0,4));
		}
		break;
	case 10:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	4,5,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,3,	4,5,	1,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,1,3,	4,5,	1,5));
		}
		break;
	case 11:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,3,	4,6,	1,5));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,3,	1,5,	0,1));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,5,7,	1,5,	4,6));
		}
		break;
	case 12:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	0,2,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	1,3,	5,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	1,3,	0,1));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	0,1,	0,4));
		}
		break;
	case 13:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	1,3,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	5,7,	3,1));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	5,7));
		}
		break;
	case 14:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	0,2,	3,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,3,7,	0,2,	1,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,5,7,	0,4,	4,6));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,5,7,	1,5,	0,4));
		}
		break;
	case 15:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	3,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,3,7,	4,6,	1,0));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,3,7,	0,1,	1,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	5,7,	0,1));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,1,	1,5,	5,7));
		}
		break;
	case 16:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	5,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	5,7,	3,7));
		}
		break;
	case 17:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,1,3,	0,1,	1,5));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,3,7,	5,7,	6,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,0,2,	2,3,	2,6));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	4,5,	0,4));
		}
		break;
	case 18:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,3,7,	5,7,	6,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,5,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,4,6,	4,5,	2,6));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,3,	4,5,	1,5));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,3,	1,5,	1,3));
		}
		break;
	case 19:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	2,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,3,	4,6,	1,3));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,6,7,	1,3,	4,6));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,3,7,	1,3,	6,7));
		}
		break;
	case 20:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	6,7));
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,1,3,	1,5,	1,0));
		}
		break;
	case 21:
		{
		TriangleList_Add(&ToBeReturned,Voxel_NewTriangle(This,2,6,	4,6,	6,7));
		}
		break;
	case 22:
		//non to add
		break;
	default:
		//should warn that the case was not found
		break;
	}

	return ToBeReturned;
}

int Voxel_GetEdgePoint(Voxel *This, int corner1, int corner2)
{
	return This->MyDataArray->GetEdgePoint(corner1, corner2, This->i, This->j, This->k);
}

Triangle *Voxel_NewTriangle(Voxel *This, int c1a, int c1b, int c2a, int c2b, int c3a, int c3b)
{
	int mc1a=This->MyBaseRotation.r[c1a], mc1b=This->MyBaseRotation.r[c1b];
	int mc2a=This->MyBaseRotation.r[c2a], mc2b=This->MyBaseRotation.r[c2b];
	int mc3a=This->MyBaseRotation.r[c3a], mc3b=This->MyBaseRotation.r[c3b];
	int a = Voxel_GetEdgePoint(This,mc1a, mc1b);
	int b = Voxel_GetEdgePoint(This,mc2a, mc2b);
	int c = Voxel_GetEdgePoint(This,mc3a, mc3b);
	//printf("new triangle %i %i %i\n",a,b,c);
	return new_Triangle3( a, b, c);
}
