

#ifndef __DATA_ARRAY_H_
#define __DATA_ARRAY_H_

//#include "stdafx.h"
#include <fstream>



//my includes:
#include "VRMLobject.h"
#include "Useful.h"


using namespace std;
//classes useful for the data array

class DataElt
{
public:
	DataElt();
	void reset();

public:
	float density; //the density at that vertex
	int XEdgePoint, YEdgePoint, ZEdgePoint;
	//the index of the point that is on the edge leaving that vertex
	//and going in the x/y/z direction (init to -1 if not indexed yet)
};

class DataArray
{
public:
	DataArray();
	DataArray(int dimx, int dimy, int dimz);
	~DataArray();
	void SetParams(float Threashold, VRMLobject *AVRMLobject);
	float GetValue(int i, int j, int k);
	Point3DList GetCoord(int i, int j, int k);
	void SetValue(int i, int j, int k, BYTE value);
	Point3DList ReverseInterpolate(int i1, int j1, int k1, int i2, int j2, int k2);
	//reverse interpolates over the density, to find the point between both points
	//(given by the indexes) where the density is equals to T (the Threashold).

	MH_BOOL IsInitialized();
	int GetXD();
	int GetYD();
	int GetZD();
	MH_BOOL ValueAboveT(int i, int j, int k);

	int GetXEdgePoint(int i, int j, int k);
	int GetYEdgePoint(int i, int j, int k);
	int GetZEdgePoint(int i, int j, int k);
	//returns an int = Get[XYZ]EdgePoint() if that pont has already been used
	//otherwhise, sets [XYZ]EdgePoint to ++NbPoints and returs that
	//AND adds this new point to the VRMLobject.
	int GetEdgePoint(int corner1, int corner2, int i, int j, int k);

	void AddTriangles(Triangle *TriangleList);

	int ReadMyFile(ifstream *infile, bool Format254);
	void WriteMyFile(ofstream outfile);
	void ReadFile(ifstream infile);
	void WriteFile(ofstream outfile);
	
private:
	int XDimention, YDimention, ZDimention;
	DataElt *data;
	int NbPoints;
	float MyT;
	bool MyUseEnhanced;
	VRMLobject *MyVRMLobject;
};

#endif //__DATA_ARRAY_H_