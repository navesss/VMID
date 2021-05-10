


#ifndef __USEFUL_H_
#define __USEFUL_H_

//#include "fstream.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "math.h"
//#include "tk.h"

#pragma warning(disable: 4996)

//My defines
#define PI 3.14159265358979323846
#define limit(a,b,c) ((a>b) ? a : ((b>c) ? c : b))
//clamp b to a<b<c

//typedef enum {TRUE = 1 , FALSE =0} BOOL;
typedef int MH_BOOL;
#define MH_TRUE 1;
#define MH_FALSE 0;
//#define toBOOL(a) (a ? TRUE : FALSE)
typedef unsigned char BYTE;

//class Point3DList
struct Point3DList
{
	double x, y, z;
	struct Point3DList *next;
};
	
//MyGL primitives:
//================
void MyGLTranslate(Point3DList *Location);
void MyGLRotate(double teta, Point3DList *Axe);
void MyGLRestorLocation();
void MyGLRelocate(Point3DList *Location, Point3DList *Direction, Point3DList *Up);
void MyGLNormal(Point3DList *APoint);
void MyGLVertex(Point3DList *APoint);
void MyGLColor();
void MyGLBegin(int type);
void MyGLEnd();

//Point3DList class
//===============
Point3DList *new_Point3DList(Point3DList *APoint3DList);
Point3DList Point3DList_Point3DList(Point3DList *APoint3DList);
Point3DList Point3DList_Point3DList3(double Ax, double Ay, double Az);
void Point3DList_reset1(Point3DList *This, Point3DList *A);
void Point3DList_reset3(Point3DList *This, double ax, double ay, double az);
void Point3DList_reset(Point3DList *This);
void Point3DList_Invert(Point3DList *This);
MH_BOOL/*bool*/ Point3DList_IsNull(Point3DList *This);
double Point3DList_GetNorm(Point3DList *This);
double Point3DList_Normalize(Point3DList *This);
double Point3DList_Normalize1(Point3DList *This, double Value);
Point3DList Point3DList_VecProd (Point3DList *This, Point3DList *A);
double Point3DList_Angle (Point3DList *This, Point3DList A);
void Point3DList_XRotate (Point3DList *This, double teta);
void Point3DList_YRotate (Point3DList *This, double teta);
void Point3DList_ZRotate (Point3DList *This, double teta);
/*void Point3DList_Rotate(Point3DList *This, Point3DList K);*/
Point3DList Point3DList_div (Point3DList *This, double factor);
double Point3DList_ScalProd (Point3DList *This, Point3DList *A);
Point3DList Point3DList_mult (Point3DList *This, double factor);
Point3DList Point3DList_sub (Point3DList *This, Point3DList *A);
Point3DList Point3DList_add (Point3DList *This, Point3DList *A);
void Point3DList_addeq (Point3DList *This, Point3DList *A);
void Point3DList_multeq (Point3DList *This, double factor);
void Point3DList_subeq (Point3DList *This, Point3DList *A);
void Point3DList_diveq (Point3DList *This, double denom);
double Point3DList_sin(Point3DList A, Point3DList B);
double Point3DList_cos(Point3DList A, Point3DList B);
void Point3DList_read(Point3DList *This, FILE *infile);
void Point3DList_write(Point3DList *This, FILE *outfile);


//CLASS Triangle
//member vars:
typedef struct TriangleStruct
{
	int a, b, c;
	struct TriangleStruct *next;
} Triangle;

//CLASS Triangle
//member functions:
Triangle *new_Triangle3(int Aa, int Ab, int Ac);
Triangle *new_Triangle(Triangle *ATriangle);
Triangle Triangle_Triangle();
Triangle Triangle_Triangle3(int Aa, int Ab, int Ac);
Triangle Triangle_Triangle1(Triangle *ATriangle);
void Triangle_write (Triangle *This, FILE *outfile);

//CLASS TriangleList
//member variables:
typedef struct
{
	Triangle *Head;
	Triangle *Tail;
} TriangleList;

//CLASS TriangleList
//member functions:
TriangleList TriangleList_TriangleList();
void TriangleList_Add(TriangleList *This, Triangle *ATriangle);
void TriangleList_Concat(TriangleList *This, TriangleList *ATriangleList);

//CLASS Edge
//member variables:
typedef struct
{
	int corner1, corner2; //edge defined by two corners
} Edge;

//CLASS Edge
//member functions:
Edge Edge_Edge();
Edge Edge_Edge2(int Acorner1, int Acorner2);

#endif
