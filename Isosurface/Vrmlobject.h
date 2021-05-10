

#ifndef __VRML_OBJECT_H_
#define __VRML_OBJECT_H_

#include "stdio.h"

#include "Useful.h"
//CLASS VRMLobject
//member var.:



typedef struct
{
	int NbPoints, NbTri, N;
	FILE *MyOutFile;
	Point3DList *PointListHead;
	Point3DList *PointListTail;
	Triangle *TriangleListHead;
	Triangle *TriangleListTail;
} VRMLobject;

//CLASS VRMLobject
//member functions:
void VRMLobject_relese(VRMLobject *This);
void VRMLobject_WriteTo(VRMLobject *This, FILE *outfile);
void VRMLobject_AddPoint(VRMLobject *This, Point3DList *APoint3DList);
void VRMLobject_AddTriangle(VRMLobject *This, Triangle *ATriangle);
void VRMLobject_AddTriangleList(VRMLobject *This, TriangleList *ATriangleList);
int VRMLobject_CountNbTri(VRMLobject *This);
void VRMLobject_SaveToFileAsVRML1(VRMLobject *This);
void VRMLobject_SaveToFileAsOff(VRMLobject *This);



//////////////////////////////////////////////////////////////////////////
// revised by Yushen Liu
void VRMLobject_SaveToFileAsVRML1_2(VRMLobject *This, char *OutputName);

#endif //__VRML_OBJECT_H_
