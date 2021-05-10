

#include "Useful.h"
#include "DataArray.h"
#include "Voxel.h"
#include "ExtractIsoSurf.h"

#include <fstream>



using namespace std;

/*void ExtractIsoSurf2(PifFile *Input3DMap, char *OutputName, int Treashhold,
					/*tmp BOOL Format254)
{
	TriangleList MyTriangleList = TriangleList_TriangleList();
	TriangleList ToBeAdded = TriangleList_TriangleList();
	int i, j, k;
	int XVoxelNb, YVoxelNb, ZVoxelNb;

	FILE *outfile = fopen(OutputName, "w");

	Voxel MyVoxel;
	DataArray MyDataArray;
	VRMLobject MyVRMLobject;

	VRMLobject_WriteTo(&MyVRMLobject,outfile);
	DataArray_SetParams(&MyDataArray,Treashhold, &MyVRMLobject);
	Voxel_SetParams(&MyVoxel,&MyDataArray);

	DataArray_ReadPifFile(&MyDataArray,Input3DMap,Format254);

	// if a file is not already open, open one.
	if (DataArray_IsInitialized(&MyDataArray))
	{
		printf("starting the main loop...\n");
		//associate an ifstream with it
		//go through all the vertexes that are in the data set
		//for each:
		//see notes for conventions on corners.
		XVoxelNb = DataArray_GetXD(&MyDataArray)-1;
		YVoxelNb = DataArray_GetYD(&MyDataArray)-1;
		ZVoxelNb = DataArray_GetZD(&MyDataArray)-1;

		for (i=0 ; i<XVoxelNb ; i++)
		{
			for (j=0 ; j<YVoxelNb ; j++)
			{
				for (k=0 ; k<ZVoxelNb ; k++)
				{
	//have the voxel class compute the triangles that it outputs
	//as triplets of uniquely indexed points of the voxel.
					//get the new triangles:
					Voxel_reset3(&MyVoxel,i,j,k);
					ToBeAdded = Voxel_OutputTriangles(&MyVoxel);
					//add the triangles to the list
					//printf("adding triangle list...\n");
					//TriangleList_write(&ToBeAdded);
					TriangleList_Concat(&MyTriangleList,&ToBeAdded);
				}
			}
		}
		VRMLobject_AddTriangleList(&MyVRMLobject,&MyTriangleList);
		//write the VRMLObjoct to a file.
		VRMLobject_SaveToFileAsOff(&MyVRMLobject);
	}
	//fclose(infile);
	fclose(outfile);
	DataArray_relese(&MyDataArray);
	VRMLobject_relese(&MyVRMLobject);
}*/





void ExtractIsoSurf3(char *InputName, char *OutputName, float Treashhold,
					/*tmp*/MH_BOOL Format254)
{
	TriangleList MyTriangleList = TriangleList_TriangleList();
	TriangleList ToBeAdded = TriangleList_TriangleList();
	int i, j, k;
	int XVoxelNb, YVoxelNb, ZVoxelNb;

	//FILE *infile = fopen(InputName, "r");
	//ifstream infile(InputName, ios::in | ios::nocreate);
	ifstream infile(InputName, ios::in);
	FILE *outfile = fopen(OutputName, "w");

	Voxel MyVoxel;
	DataArray MyDataArray;
	VRMLobject MyVRMLobject;

	VRMLobject_WriteTo(&MyVRMLobject,outfile);
	MyDataArray.SetParams(Treashhold, &MyVRMLobject);
	Voxel_SetParams(&MyVoxel,&MyDataArray);

	MyDataArray.ReadMyFile(&infile,Format254!=0);

	// if a file is not already open, open one.
	if (MyDataArray.IsInitialized())
	{
		printf("starting the main loop...\n");
		//associate an ifstream with it
		//go through all the vertexes that are in the data set
		//for each:
		//see notes for conventions on corners.
		XVoxelNb = MyDataArray.GetXD()-1;
		YVoxelNb = MyDataArray.GetYD()-1;
		ZVoxelNb = MyDataArray.GetZD()-1;

		for (i=0 ; i<XVoxelNb ; i++)
		{
			for (j=0 ; j<YVoxelNb ; j++)
			{
				for (k=0 ; k<ZVoxelNb ; k++)
				{
	//have the voxel class compute the triangles that it outputs
	//as triplets of uniquely indexed points of the voxel.
					//get the new triangles:
					Voxel_reset3(&MyVoxel,i,j,k);
					ToBeAdded = Voxel_OutputTriangles(&MyVoxel);
					//add the triangles to the list
					//printf("adding triangle list...\n");
					//TriangleList_write(&ToBeAdded);
					TriangleList_Concat(&MyTriangleList,&ToBeAdded);
				}
			}
		}
		VRMLobject_AddTriangleList(&MyVRMLobject,&MyTriangleList);
		//write the VRMLObjoct to a file.
		//VRMLobject_SaveToFileAsOff(&MyVRMLobject);
		VRMLobject_SaveToFileAsVRML1(&MyVRMLobject);
	}
	infile.close();
	fclose(outfile);
	//DataArray_relese(&MyDataArray);
	VRMLobject_relese(&MyVRMLobject);
}

VRMLobject* ExtractIsoSurf3(DataArray *MyDataArray, float Treashhold,
					 /*tmp*/MH_BOOL Format254)
{
	TriangleList MyTriangleList = TriangleList_TriangleList();
	TriangleList ToBeAdded = TriangleList_TriangleList();
	int i, j, k;
	int XVoxelNb, YVoxelNb, ZVoxelNb;

	Voxel MyVoxel;
	//DataArray MyDataArray;
	//VRMLobject MyVRMLobject;
	VRMLobject *MyVRMLobject = new VRMLobject();

	MyDataArray->SetParams(Treashhold, MyVRMLobject);
	Voxel_SetParams(&MyVoxel, MyDataArray);

	// if a file is not already open, open one.
	if (MyDataArray->IsInitialized())
	{
		printf("starting the main loop...\n");
		//associate an ifstream with it
		//go through all the vertexes that are in the data set
		//for each:
		//see notes for conventions on corners.
		XVoxelNb = MyDataArray->GetXD()-1;
		YVoxelNb = MyDataArray->GetYD()-1;
		ZVoxelNb = MyDataArray->GetZD()-1;

		for (i=0 ; i<XVoxelNb ; i++)
		{
			for (j=0 ; j<YVoxelNb ; j++)
			{
				for (k=0 ; k<ZVoxelNb ; k++)
				{
					//have the voxel class compute the triangles that it outputs
					//as triplets of uniquely indexed points of the voxel.
					//get the new triangles:
					Voxel_reset3(&MyVoxel,i,j,k);
					ToBeAdded = Voxel_OutputTriangles(&MyVoxel);
					//add the triangles to the list
					//printf("adding triangle list...\n");
					//TriangleList_write(&ToBeAdded);
					TriangleList_Concat(&MyTriangleList,&ToBeAdded);
				}
			}
		}
		VRMLobject_AddTriangleList(MyVRMLobject,&MyTriangleList);
	}

	return MyVRMLobject;
}
