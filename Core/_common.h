// _common.h

#pragma once

#include "point.h"


Point3D operator + (Point3D &p0, Point3D &p1);
void operator += (Point3D &p0, Point3D &p1);
Point3D operator * (double w, Point3D &p1);
Point3D operator * (Point3D &p1, double w);

double operator * (Point3D &p0, Point3D &p1);

////////////////////////////////////////////////////////////////////////
inline Point3D operator + (Point3D &p0, Point3D &p1)
{
	return Point3D(p0.x+p1.x, p0.y+p1.y, p0.z+p1.z);
}

inline void operator += (Point3D &p0, Point3D &p1)
{
	p0 = p0+p1;
}

inline Point3D operator * (double w, Point3D &p)
{
	return Point3D(p.x*w, p.y*w, p.z*w);
}

inline Point3D operator * (Point3D &p, double w)
{
	return w*p;
}

inline	double operator * (Point3D &p0, Point3D &p1)
{ 
	return p0.x * p1.x + p0.y * p1.y + p0.z * p1.z; 
}