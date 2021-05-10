
//#include "math.h"
//#include "gl\gl.h"
//#include "gl\glu.h"
#include <stdio.h>

#include "Useful.h"

//MyGL primitives:
//================
void MyGLTranslate(Point3DList *Location)
{
	//glTranslated(Location->x, Location->y, Location->z);
}
void MyGLRotate(double teta, Point3DList *Axe)
{
	//glRotated(teta*(180/PI), Axe->x, Axe->y, Axe->z);
}
void MyGLRestorLocation()
{
	//glPopMatrix();
	///glPushMatrix();
}

void MyGLRelocate(Point3DList *Location, Point3DList *Direction, Point3DList *Up)
{
	double m[16];
	Point3DList W = Point3DList_VecProd(Up,Direction);
	//colum 1:
	m[0] = Direction->x; m[1] = Direction->y; m[2] = Direction->z; m[3] = 0;
	//colum 2:
	m[4] = W.x; m[5] = W.y; m[6] = W.z; m[7] = 0;
	//colum 3:
	m[8] = Up->x; m[9] = Up->y; m[10] = Up->z; m[11] = 0;
	//colum 4:
	m[12] = Location->x; m[13] = Location->y; m[14] = Location->z; m[15] = 1;

	//glMultMatrixd(m);
}

void MyGLNormal(Point3DList *APoint)
{
	//glNormal3d(APoint->x, APoint->y, APoint->z);
}

void MyGLVertex(Point3DList *APoint)
{
	//glVertex3d(APoint->x, APoint->y, APoint->z);
}

void MyGLColor()
{
	//glColor3d(0, 0, 150);
}

void MyGLBegin(int type)
{
	//glBegin(type);
}

void MyGLEnd()
{
	//glEnd();
}

//Point3DList class
Point3DList *new_Point3DList(Point3DList *APoint3DList)
{
	Point3DList *tmp = (Point3DList *)malloc(sizeof(Point3DList));
	*tmp = *APoint3DList;
	return tmp;
}
Point3DList Point3DList_Point3DList(Point3DList *APoint3DList)
{
	Point3DList tmp;
	tmp = *APoint3DList;
	return tmp;
}
Point3DList Point3DList_Point3DList3(double Ax, double Ay, double Az)
{
	Point3DList tmp;
	tmp.x = Ax;
	tmp.y = Ay;
	tmp.z = Az;
	return tmp;
}
void Point3DList_reset1(Point3DList *This, Point3DList *A)
{
	This->x = A->x;
	This->y = A->y;
	This->z = A->z;
}

void Point3DList_reset3(Point3DList *This, double ax, double ay, double az)
{
	This->x = ax;
	This->y = ay;
	This->z = az;
}

void Point3DList_reset(Point3DList *This){This->x=0;This->y=0;This->z=0;}

void Point3DList_Invert(Point3DList *This){This->x=-This->x; This->y=-This->y; This->z=-This->z;}

int/*bool*/ Point3DList_IsNull(Point3DList *This) {return This->x==0 && This->y==0 && This->z==0;}

double Point3DList_GetNorm(Point3DList *This)
{
	return sqrt((This->x*This->x)+(This->y*This->y)+(This->z*This->z));
}

double Point3DList_Normalize(Point3DList *This)
{
	double norm = Point3DList_GetNorm(This);
	if (norm>0)
	{
		This->x = This->x/norm;
		This->y = This->y/norm;
		This->z = This->z/norm;
	}
	return norm;
}

double Point3DList_Normalize1(Point3DList *This, double Value)
{
	double norm = Point3DList_GetNorm(This);
	if (norm>0)
	{
		This->x = (This->x*Value)/norm;
		This->y = (This->y*Value)/norm;
		This->z = (This->z*Value)/norm;
	}
	return norm;
}

Point3DList Point3DList_VecProd (Point3DList *This, Point3DList *A)
{
	Point3DList tmp;
	Point3DList_reset3(&tmp,
		(This->y*A->z-This->z*A->y),
		(This->z*A->x-This->x*A->z),
		(This->x*A->y-This->y*A->x)
	);
	return tmp;
}

double Point3DList_Angle (Point3DList *This, Point3DList A)
{
	Point3DList B;
	Point3DList_reset1(&B,This);
	Point3DList_Normalize(&B);
	Point3DList_Normalize(&A);
	return acos(limit(-1,Point3DList_ScalProd(&A,&B),1));
}

void Point3DList_XRotate (Point3DList *This, double teta)
{
	double CosO = cos(teta);
	double SinO = sin(teta);
	Point3DList_reset3(This, This->x, CosO*This->y - SinO*This->z, SinO*This->y + CosO*This->z);
}

void Point3DList_YRotate (Point3DList *This, double teta)
{
	double CosO = cos(teta);
	double SinO = sin(teta);
	Point3DList_reset3(This, CosO*This->x + SinO*This->z, This->y, SinO*This->x - CosO*This->z);
}

void Point3DList_ZRotate (Point3DList *This, double teta)
{
	double CosO = cos(teta);
	double SinO = sin(teta);
	Point3DList_reset3(This, CosO*This->x - SinO*This->y, SinO*This->x + CosO*This->y, This->z);
}

/*void Point3DList_Rotate(Point3DList *This, Point3DList K)
{
	Point3DList I,J;
	double X,Y,Z;
	double CosO;
	double SinO;
	double teta = Normalize(&K);
	if (teta != 0)
	{
		CosO = cos(teta);
		SinO = sin(teta);

		J = Point3DList_VecProd(&K,This);
		Normalize(&J);
		I = Point3DList_VecProd(&J,&K);
		X = Point3DList_ScalProd(&I,This);
		Y = Point3DList_ScalProd(&J,This);
		Z = Point3DList_ScalProd(&K,This);
		//line to be modified:
		*this = I*(CosO*X - SinO*Y) + J*(SinO*X + CosO*Y) + K*Z;
	}
}*/

Point3DList Point3DList_div (Point3DList *This, double factor)
{
	Point3DList tmp;
	Point3DList_reset3(&tmp, This->x/factor, This->y/factor, This->z/factor);
	return tmp;
}

double Point3DList_ScalProd (Point3DList *This, Point3DList *A)
{
	return (This->x*A->x + This->y*A->y + This->z*A->z);
}

Point3DList Point3DList_mult (Point3DList *This, double factor)
{
	Point3DList tmp;
	Point3DList_reset3(&tmp, This->x*factor, This->y*factor, This->z*factor);
	return tmp;
}

Point3DList Point3DList_sub (Point3DList *This, Point3DList *A)
{
	Point3DList tmp;
	Point3DList_reset3(&tmp, This->x-A->x, This->y-A->y, This->z-A->z);
	return tmp;
}

Point3DList Point3DList_add (Point3DList *This, Point3DList *A)
{
	Point3DList tmp;
	Point3DList_reset3(&tmp, This->x+A->x, This->y+A->y, This->z+A->z);
	return tmp;
}

void Point3DList_addeq (Point3DList *This, Point3DList *A)
{
	This->x += A->x;
	This->y += A->y;
	This->z += A->z;
}

void Point3DList_multeq (Point3DList *This, double factor)
{
	This->x *= factor;
	This->y *= factor;
	This->z *= factor;
}

void Point3DList_subeq (Point3DList *This, Point3DList *A)
{
	This->x -= A->x;
	This->y -= A->y;
	This->z -= A->z;
}

void Point3DList_diveq (Point3DList *This, double denom)
{
	This->x /= denom;
	This->y /= denom;
	This->z /= denom;
}

/*MyPoint Point3DList_LessZAxe(Point3DList *This)
{
	MyPoint tmp(x,y);
	return tmp;
}*/

double Point3DList_sin(Point3DList A, Point3DList B)
{
	Point3DList_Normalize(&A);
	Point3DList_Normalize(&B);
	return (Point3DList_VecProd(&A,&B)).z;
}

double Point3DList_cos(Point3DList A, Point3DList B)
{
	Point3DList_Normalize(&A);
	Point3DList_Normalize(&B);
	return Point3DList_ScalProd(&A,&B);
}

void Point3DList_read(Point3DList *This, FILE *infile)
{
	float x,y,z;
	fscanf(infile, "%f %f %f ", &x, &y, &z);
	This->x = (double)x;
	This->y = (double)y;
	This->z = (double)z;
}


void Point3DList_write(Point3DList *This, FILE *outfile)
{
	float x,y,z;
	x = (float)This->x;
	y = (float)This->y;
	z = (float)This->z;
	fprintf(outfile, "%f %f %f\n", x, y, z);
}

Triangle *new_Triangle(Triangle *ATriangle)
{
	Triangle *tmp = (Triangle *)malloc(sizeof(Triangle));
	*tmp = *ATriangle;
	return tmp;
}

Triangle *new_Triangle3(int Aa, int Ab, int Ac)
{
	Triangle *tmp = (Triangle *)malloc(sizeof(Triangle));
	tmp->a = Aa;
	tmp->b = Ab;
	tmp->c = Ac;
	tmp->next = 0;
	return tmp;
}

Triangle Triangle_Triangle()
{
	Triangle tmp;
	tmp.next = 0;
	return tmp;
}

Triangle Triangle_Triangle3(int Aa, int Ab, int Ac)
{
	Triangle tmp;
	tmp.a = Aa;
	tmp.b = Ab;
	tmp.c = Ac;
	tmp.next = 0;
	return tmp;
}

Triangle Triangle_Triangle1(Triangle *ATriangle)
{
	Triangle tmp;
	tmp.a = ATriangle->a;
	tmp.b = ATriangle->b;
	tmp.c = ATriangle->c;
	tmp.next = 0;
	return tmp;
}

void Triangle_write (Triangle *This, FILE *outfile)
{
	fprintf(outfile,"3 %i %i %i\n",This->b,This->a,This->c);
}

Edge Edge_Edge()
{
	Edge tmp;
	tmp.corner1 = -1;
	tmp.corner2 = -1;
	return tmp;
}

Edge Edge_Edge2(int Acorner1, int Acorner2)
{
	Edge tmp;
	tmp.corner1 = Acorner1;
	tmp.corner2 = Acorner2;
	return tmp;
}

TriangleList TriangleList_TriangleList()
{
	TriangleList tmp;
	tmp.Head = 0;
	tmp.Tail = 0;
	return tmp;
}

void TriangleList_Add(TriangleList *This, Triangle *ATriangle)
{
	if (This->Head)
		This->Tail->next = ATriangle;
	else
		This->Head = ATriangle;
	This->Tail = ATriangle;
	This->Tail->next = 0;
}



void TriangleList_Concat(TriangleList *This, TriangleList *ATriangleList)
{
	if (This->Head)//the list is not empty
	{
		This->Tail->next = ATriangleList->Head;
		if (ATriangleList->Tail)
			This->Tail = ATriangleList->Tail;
	}
	else
	{//the list is empty
		This->Head = ATriangleList->Head;
		This->Tail = ATriangleList->Tail;
	}
	ATriangleList->Head = 0;
	ATriangleList->Tail = 0;
	if (This->Tail) This->Tail->next = 0;
}
