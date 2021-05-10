
#ifndef	POINT_H
#define POINT_H

#include <math.h>
//#include "Global.h"
#define TOLER		1e-10

class Vector2D;
class Vector3D;
class Point2D;
class Point3D;

/*-------------------------------------------------------*/

class Vector2D
{
public:
	double	x, y;

public:
	//构造函数
	Vector2D ()
		{ x = y = 0; }
	Vector2D (double newx, double newy)
		{ x = newx; y = newy; }

	//赋值操作符
	void operator += (const Vector2D& v)
		{ x += v.x; y += v.y; }
	void operator -= (const Vector2D& v)
		{ x -= v.x; y -= v.y; }
	void operator *= (double num)
		{ x *= num; y *= num; }
	void operator /= (double num)
		{ //ASSERT (fabs (num) < TOLER);
		  x /= num; y /= num; 
		}

	//矢量加
	Vector2D operator + (const Vector2D& v) const
		{ return Vector2D (x + v.x, y + v.y); }
	//矢量减
	Vector2D operator - (const Vector2D& v) const
		{ return Vector2D (x - v.x, y - v.y); }
	//矢量乘(点积)
	double  operator * (const Vector2D& v) const
		{ return x * v.x + y * v.y; }
	//数乘
	Vector2D operator * (double num) const
		{ return Vector2D (x * num, y * num); }
	//数除
	Vector2D operator / (double num) const
		{ //ASSERT (fabs (num) < TOLER);
		  return Vector2D (x / num, y / num); 
		}

	//单目减
	Vector2D operator - () const
		{ return Vector2D (-x, -y); }

	//判等(不等)
	int operator == (const Vector2D& v) const;
	int operator != (const Vector2D& v) const;

	//类型转换
	operator Point2D ();

	//取元素
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				default: return x;
			}
		}
	
	//设为单位矢量
	void SetUnit ();       

};

/*-------------------------------------------------------*/

class Vector3D
{
public:
	double	x, y, z;

public:
	//构造函数
	Vector3D ()
		{ x = y = z = 0; }
	Vector3D (double newx, double newy, double newz)
		{ x = newx; y = newy; z = newz; }

	//赋值操作符
	void operator += (const Vector3D& v)
		{ x += v.x; y += v.y; z += v.z; }
	void operator -= (const Vector3D& v)
		{ x -= v.x; y -= v.y; z -= v.z; }
	void operator *= (double num)
		{ x *= num; y *= num; z *= num; }
	void operator /= (double num)
		{ //ASSERT (fabs (num) < TOLER);
		  x /= num; y /= num; z /= num; 
		}
	void operator ^= (const Vector3D& v)
		{ double a =   y * v.z - z * v.y;
		  double b = - x * v.z + z * v.x;
		  double c =   x * v.y - y * v.x;

		  x = a;
		  y = b;
		  z = c;
		}

	//矢量加
	Vector3D operator + (const Vector3D& v) const
		{ return Vector3D (x + v.x, y + v.y, z + v.z); }
	//矢量减
	Vector3D operator - (const Vector3D& v) const
		{ return Vector3D (x - v.x, y - v.y, z - v.z); }
	//矢量乘(点积)
	double  operator * (const Vector3D& v) const
		{ return x * v.x + y * v.y + z * v.z; }
	//矢量乘(叉积)
	Vector3D operator ^ (const Vector3D& v) const
		{ return Vector3D ( y * v.z - z * v.y,
						  - x * v.z + z * v.x,
							x * v.y - y * v.x); 
		}
	//数乘
	Vector3D operator * (double num) const
		{ return Vector3D (x * num, y * num, z * num); }
	//数除
	Vector3D operator / (double num) const
		{ //ASSERT (fabs (num) < TOLER);
		  return Vector3D (x / num, y / num, z / num); 
		}
	//单目减
	Vector3D operator - () const
		{ return Vector3D (-x, -y, -z); }

	//判等(不等)
	int operator == (const Vector3D& v) const;
	int operator != (const Vector3D& v) const;

	//类型转换
	operator Point3D ();

	//取元素
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: return x;
			}
		}

	//设为单位矢量
	void SetUnit ();
};

/*-------------------------------------------------------*/

class Point2D
{
public:
	double	x, y;

public:
	//构造函数
	Point2D ()
		{ x = y = 0;}
	Point2D (double newx, double newy)
		{ x = newx; y = newy;}

	//赋值操作符
	void operator += (const Vector2D& v)
		{ x += v.x; y += v.y; }
	void operator -= (const Vector2D& v)
		{ x -= v.x; y -= v.y; }

	//点加向量
	Point2D operator + (const Vector2D& v) const;
	//点减向量
	Point2D operator - (const Vector2D& v) const;
	//两点相减
	Vector2D operator - (const Point2D& v) const;
	
	//单目减
	Point2D operator - () const;

	//判等(不等)
	int operator == (const Point2D& v) const;
	int operator != (const Point2D& v) const;
   
    //取元素
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				default: return x;
			}
		}
};

/*-------------------------------------------------------*/

class Point3D
{
public:
	double	x, y, z;

public:
	//构造函数
	Point3D ()
		{ x = y = z = 0;}
	Point3D (double newx, double newy, double newz)
		{ x = newx; y = newy; z = newz; }

	//赋值操作符
	void operator += (const Vector3D& v)
		{ x += v.x; y += v.y; z += v.z; }
	void operator -= (const Vector3D& v)
		{ x -= v.x; y -= v.y; z -= v.z; }

	//点加向量
	Point3D operator + (const Vector3D& v) const;
	//点减向量
	Point3D operator - (const Vector3D& v) const;
	//两点相减
	Vector3D operator - (const Point3D& v) const;

	//单目减
	Point3D operator - () const;

	//判等(不等)
	int operator == (const Point3D& v) const;
	int operator != (const Point3D& v) const;
	
	//取元素
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: return x;
			}
		}	
};

/*-------------------------------------------------------*/

double Distance (const Point2D& p1, const Point2D& p2);
double Distance (const Point3D& p1, const Point3D& p2);

Point2D MidPoint (const Point2D& p1, const Point2D& p2);
Point3D MidPoint (const Point3D& p1, const Point3D& p2);

Point2D Min (const Point2D& p1, const Point2D& p2);
Point3D Min (const Point3D& p1, const Point3D& p2);
Point2D Max (const Point2D& p1, const Point2D& p2);
Point3D Max (const Point3D& p1, const Point3D& p2);

double Length (const Vector2D& v);
double Length (const Vector3D& v);

bool SamePoint (const Point2D& p1, const Point2D& p2, double toler = TOLER);
bool SamePoint (const Point3D& p1, const Point3D& p2, double toler = TOLER);
//Liu Shixia
bool SameVector (const Vector2D& v1, const Vector2D& v2, double toler = TOLER);
bool SameVector (const Vector3D& v1, const Vector3D& v2, double toler = TOLER);

void SwapPoint (Point2D& p1, Point2D& p2);
void SwapPoint (Point3D& p1, Point3D& p2);
void SwapVector (Vector2D& p1, Vector2D& p2);
void SwapVector (Vector3D& p1, Vector3D& p2);

#define ZeroP2D Point2D(0.0, 0.0)
#define ZeroP3D Point3D(0.0, 0.0, 0.0)
        
#define BaseXV2D Vector2D(1.0, 0.0)
#define BaseYV2D Vector2D(0.0, 1.0)
        
#define BaseXV3D Vector3D(1.0, 0.0, 0.0)
#define BaseYV3D Vector3D(0.0, 1.0, 0.0)
#define BaseZV3D Vector3D(0.0, 0.0, 1.0)

#include "Point.inl"

#endif
