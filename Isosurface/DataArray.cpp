
#include "DataArray.h"
#include "math.h"
#include <fstream>
//#include "Sort.h"

//useful classes for the data array:

DataElt::DataElt()
{
	reset();
}

void DataElt::reset()
{
	density = 0;
	XEdgePoint = -1;
	YEdgePoint = -1;
	ZEdgePoint = -1;
}

DataArray::DataArray()
{
	XDimention = -1;
	YDimention = -1;
	ZDimention = -1;
	data = 0;
	NbPoints = 0;
	MyT = 128;
}

DataArray::DataArray(int dimx, int dimy, int dimz)
{
	XDimention = dimx;
	YDimention = dimy;
	ZDimention = dimz;

	data = new DataElt[XDimention*YDimention*ZDimention];

	NbPoints = 0;
	MyT = 128;
}

DataArray::~DataArray()
{
	if (data) delete[] data;
}

void DataArray::SetParams(float Threashold, VRMLobject *AVRMLobject)
{
	MyT = Threashold;
	MyVRMLobject = AVRMLobject;
}

float DataArray::GetValue(int i, int j, int k)
{
	//temporary:
	if (0<=i && i<XDimention && 0<=j && j<YDimention && 0<=k && k<ZDimention)
	//first check if data is init. and the indexes are good.
		return data[i+XDimention*j+YDimention*XDimention*k].density;
	else
		return 0;
}

Point3DList DataArray::GetCoord(int i, int j, int k)
{
	double x = ((double)i)/((double)XDimention-1);
	double y = ((double)j)/((double)YDimention-1);
	double z = ((double)k)/((double)ZDimention-1);
	return Point3DList_Point3DList3(x,y,z);
}

void DataArray::SetValue(int i, int j, int k, BYTE value)
{
	//first check if data is init. and the indexes are good.
	data[i+XDimention*j+YDimention*XDimention*k].density = value;
}

Point3DList DataArray::ReverseInterpolate(int i1, int j1, int k1, int i2, int j2, int k2)
{
	float V1 = GetValue(i1,j1,k1);
	float V2 = GetValue(i2,j2,k2);
	double factor1 = fabs((MyT-V2)/(V2-V1));
	double factor2 = fabs((MyT-V1)/(V2-V1));
	Point3DList P1;
	Point3DList P2;
	P1 = GetCoord(i1, j1, k1);
	P2 = GetCoord(i2, j2, k2);
	Point3DList tmp;
	tmp.x = (P1.x*factor1 + P2.x*factor2);
	tmp.y = (P1.y*factor1 + P2.y*factor2);
	tmp.z = (P1.z*factor1 + P2.z*factor2);
	//tmp = (P1*0.5 + P2*0.5);
	if (V1==V2 || factor1>1 || factor2>1)//temporary
	{
		int bobo = 3;
		bobo += 1;
		return P1;
	}
	return tmp;
}

MH_BOOL DataArray::IsInitialized()
{
	return data!=0;
}

int DataArray::GetXD()
{
	return XDimention;
}

int DataArray::GetYD()
{
	return YDimention;
}

int DataArray::GetZD()
{
	return ZDimention;
}

MH_BOOL DataArray::ValueAboveT(int i, int j, int k)
{
	float tmp = GetValue(i, j, k);
	if (tmp>MyT)
	{
		int bobo=5;
		return MH_TRUE;
	}
	else
		return MH_FALSE;
}

int DataArray::GetXEdgePoint(int i, int j, int k)
{
	Point3DList tmp;
	DataElt *ADataElt = &data[i+XDimention*j+YDimention*XDimention*k];
	if (ADataElt->XEdgePoint == -1) //this point has not been used yet
	{
		//add this point and give it a new index:
		ADataElt->XEdgePoint = NbPoints; //set the edge point index to new value
		NbPoints++;	//one more point in list
		tmp = ReverseInterpolate(i,j,k,i+1,j,k);
		VRMLobject_AddPoint(MyVRMLobject,&tmp); //add the right point
	}
	//else
	return ADataElt->XEdgePoint;
}

int DataArray::GetYEdgePoint(int i, int j, int k)
{
	Point3DList tmp;
	DataElt *ADataElt = &data[i+XDimention*j+YDimention*XDimention*k];
	if (ADataElt->YEdgePoint == -1) //this point has not been used yet
	{
		//add this point and give it a new index:
		ADataElt->YEdgePoint = NbPoints; //set the edge point index to new value
		NbPoints++;	//one more point in list
		tmp = ReverseInterpolate(i,j,k,i,j+1,k);
		VRMLobject_AddPoint(MyVRMLobject,&tmp); //add the right point
	}
	//else
	return ADataElt->YEdgePoint;
}

int DataArray::GetZEdgePoint(int i, int j, int k)
{
	Point3DList tmp;
	DataElt *ADataElt = &data[i+XDimention*j+YDimention*XDimention*k];
	if (ADataElt->ZEdgePoint == -1) //this point has not been used yet
	{
		//add this point and give it a new index:
		ADataElt->ZEdgePoint = NbPoints; //set the edge point index to new value
		NbPoints++;	//one more point in list
		tmp = ReverseInterpolate(i,j,k,i,j,k+1);
		VRMLobject_AddPoint(MyVRMLobject,&tmp); //add the right point
	}
	else
	{
		//CString toto(' ',7);
		NbPoints+=0;
	}
	return ADataElt->ZEdgePoint;
}

int DataArray::GetEdgePoint(int corner2, int corner1, int i, int j, int k)
{
	//corner2 must be greatre (by convention)
	if (corner1>corner2)
	{
		int tmp = corner2;
		corner2 = corner1;
		corner1 = tmp;
	}
	switch (10*corner2 + corner1)
	{
		case 10: //corner2==1 corner1==0
			return GetXEdgePoint(i,j,k);
			break;
		case 20: //corner2== corner1==
			return GetYEdgePoint(i,j,k);
			break;
		case 40: //corner2== corner1==
			return GetZEdgePoint(i,j,k);
			break;
		case 31: //corner2==3 corner1==1
			return GetYEdgePoint(i+1,j,k);
			break;
		case 51: //corner2==5 corner1==1
			return GetZEdgePoint(i+1,j,k);
			break;
		case 54: //corner2==5 corner1==4
			return GetXEdgePoint(i,j,k+1);
			break;
		case 64: //corner2==6 corner1==4
			return GetYEdgePoint(i,j,k+1);
			break;
		case 32: //corner2==3 corner1==2
			return GetXEdgePoint(i,j+1,k);
			break;
		case 62: //corner2==6 corner1==2
			return GetZEdgePoint(i,j+1,k);
			break;
		case 73: //corner2==7 corner1==3
			return GetZEdgePoint(i+1,j+1,k);
			break;
		case 75: //corner2==7 corner1==5
			return GetYEdgePoint(i+1,j,k+1);
			break;
		case 76: //corner2==6 corner1==7
			return GetXEdgePoint(i,j+1,k+1);
			break;
	}
	return -1;
}

/*void DataArray::AddTriangles(Triangle *TriangleList)
{
	Triangle *tmp = TriangleList;
	while (tmp)
	{
		VRMLobject_AddTriangle(MyVRMLobject,*tmp);
	}
}*/

int DataArray::ReadMyFile(ifstream *infile, bool Format254)
{
	//counters:
	int i, j, k;
	float tmp;

	//read size of array:
	*infile >> XDimention;
	*infile >> YDimention;
	*infile >> ZDimention;

	//initialize the data array:
	if (IsInitialized()) delete[] data;
	data = new DataElt[XDimention*YDimention*ZDimention];
	if (data == NULL)
	{
		//MessageBox(NULL, "not enough memory to read in file.",NULL,  MB_OK );
		return 0;
	}

	//read array:
	for (i=0 ; i<XDimention ; i++)
	{
		for (j=0 ; j<YDimention ; j++)
		{
			for (k=0 ; k<ZDimention ; k++)
			{
				*infile >> tmp;
				SetValue(i,j,k,(unsigned char)tmp);
			}
		}
	}
	return 1; //operation has succeded
}
/*
void DataArray::WriteMyFile(ofstream outfile)
{
	//counters:
	int i, j, k;//, tmp;
	BYTE OutByte;

	outfile << XDimention;
	outfile << " ";
	outfile << YDimention;
	outfile << " ";
	outfile << ZDimention;
	outfile << "\n";

	//write array:
	for (i=0 ; i<XDimention ; i++)
	{
		for (j=0 ; j<XDimention ; j++)
		{
			for (k=0 ; k<XDimention ; k++)
			{
				OutByte = GetValue(i,j,k);
				//OutByte = (BYTE)ch0[i][j][k];
				if (OutByte == '\n' || OutByte == '\tab') OutByte = ' ';
				outfile << OutByte;
				//outfile << " ";
			}
			outfile << "\n";
		}
		outfile << "\n";
	}

}

void DataArray::ReadFile(ifstream infile)
{
	
}

void DataArray::WriteFile(ofstream infile)
{
	
}
*/