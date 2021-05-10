#include "stdafx.h"
#include "Matrix.h"
//#include "mathlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Point2D operator * (const Point2D& p, const Matrix3& mat)
{
	double x, y, w;

	const double *mt = &mat.mat[0][0];
	x = p.x*mt[0] + p.y*mt[3] + mt[6];
	y = p.x*mt[1] + p.y*mt[4] + mt[7];
	w = p.x*mt[2] + p.y*mt[5] + mt[8];

	if (fabs (w) < TOLER)
		return Point2D (0, 0);
	else
		return Point2D (x/w, y/w);
}

Vector2D operator * (const Vector2D& p, const Matrix3& mat)
{
	double x, y;

	const double *mt = &mat.mat[0][0];
	x = p.x*mt[0] + p.y*mt[3];
	y = p.x*mt[1] + p.y*mt[4];

	return Vector2D (x, y);
}

Point3D operator * (const Point3D& p, const Matrix4& mat)
{
	double x, y, z, w;

	const double *mt = &mat.mat[0][0];
	x = p.x*mt[0] + p.y*mt[4] + p.z*mt[8]  + mt[12];
	y = p.x*mt[1] + p.y*mt[5] + p.z*mt[9]  + mt[13];
	z = p.x*mt[2] + p.y*mt[6] + p.z*mt[10] + mt[14];
	w = p.x*mt[3] + p.y*mt[7] + p.z*mt[11] + mt[15];

	if (fabs (w) < TOLER)
		return Point3D (0, 0, 0);
	else
		return Point3D (x/w, y/w, z/w);
}

Vector3D operator * (const Vector3D& p, const Matrix4& mat)
{
	double x, y, z;

	const double *mt = &mat.mat[0][0];
	x = p.x*mt[0] + p.y*mt[4] + p.z*mt[8];
	y = p.x*mt[1] + p.y*mt[5] + p.z*mt[9];
	z = p.x*mt[2] + p.y*mt[6] + p.z*mt[10];

	return Vector3D (x, y, z);
}

/*-------------------------------------------------------*/

Matrix3::Matrix3 ()
{
	memset (mat, 0, sizeof (mat));
}

Matrix3::Matrix3 (const Matrix<3>& m)
{
	memcpy (mat, m.mat, sizeof (mat));
}

void Matrix3::Dummy ()
{
	Matrix3 m1, m2, m;

	m += m1;
	m -= m2;
	m *= m1;
	m *= 2.0;
	m /= 2.0;
	
	m = m1 + m2;
	m = m1 - m2;
	m = m1 * m2;
	m = m1 * 2.0;
	m = m1 / 2.0;
	m = !m1;
}

void Matrix3::SetShift (double dx, double dy)
{
	SetUnit ();
	mat[2][0] = dx;
	mat[2][1] = dy;
}

void Matrix3::SetScale (double sx, double sy)
{
	SetZero ();
	mat[0][0] = sx;
	mat[1][1] = sy;
	mat[2][2] = 1.;
}

void Matrix3::SetRotate (double angle)
{
	SetUnit ();
	mat[0][0] = cos (angle);
	mat[0][1] = sin (angle);
	mat[1][0] =-mat[0][1];
	mat[1][1] = mat[0][0];
}

/*-------------------------------------------------------*/

Matrix4::Matrix4 ()
{
	memset (mat, 0, sizeof (mat));
}

Matrix4::Matrix4 (const Matrix<4>& m)
{
	memcpy (mat, m.mat, sizeof (mat));
}

void Matrix4::Dummy ()
{
	Matrix4 m1, m2, m;

	m += m1;
	m -= m2;
	m *= m1;
	m *= 2.0;
	m /= 2.0;
	
	m = m1 + m2;
	m = m1 - m2;
	m = m1 * m2;
	m = m1 * 2.0;
	m = m1 / 2.0;
	m = !m1;
}

void Matrix4::SetShift (double dx, double dy, double dz)
{
	SetUnit ();
	mat[3][0] = dx;
	mat[3][1] = dy;
	mat[3][2] = dz;
}

void Matrix4::SetScale (double sx, double sy, double sz)
{
	SetZero ();
	mat[0][0] = sx;
	mat[1][1] = sy;
	mat[2][2] = sz;
	mat[3][3] = 1.;
}

void Matrix4::SetRotate (int axis, double angle)
{
	SetUnit ();
	switch (axis)
	{
		case 0: mat[1][1] = cos (angle);
				mat[1][2] = sin (angle);
				mat[2][1] =-mat[1][2];
				mat[2][2] = mat[1][1];
				break;
		case 1: mat[0][0] = cos (angle);
				mat[0][2] =-sin (angle);
				mat[2][0] =-mat[0][2];
				mat[2][2] = mat[0][0];
				break;
		case 2: mat[0][0] = cos (angle);
				mat[0][1] = sin (angle);
				mat[1][0] =-mat[0][1];
				mat[1][1] = mat[0][0];
				break;
	}
}

void Matrix4::SetRotateV (const Vector3D& v, double ang)
{
	double  a, b, c, aa, bb, cc, ab, bc, ac, siu, cou;

	a = v.x;
	b = v.y;
	c = v.z;
	aa = a * a;
	bb = b * b;
	cc = c * c;
	ab = a * b;
	bc = b * c;
	ac = a * c;
	cou = cos (ang);
	siu = sin (ang);

	mat[0][0] = aa + (1-aa ) * cou;
	mat[1][0] = ab * (1-cou) - c*siu;
	mat[2][0] = ac * (1-cou) + b*siu;
	mat[3][0] = 0.;
	mat[0][1] = ab * (1-cou) + c*siu;
	mat[1][1] = bb + (1-bb ) * cou;
	mat[2][1] = bc * (1-cou) - a*siu;
	mat[3][1] = 0.;
	mat[0][2] = ac * (1-cou) - b*siu;
	mat[1][2] = bc * (1-cou) + a*siu;
	mat[2][2] = cc + (1-cc ) * cou;
	mat[3][2] = 0.;
	mat[0][3] = 0.;
	mat[1][3] = 0.;
	mat[2][3] = 0.;
	mat[3][3] = 1.;
}

void Matrix4::SetRotateZ (const Vector3D& v)
{
	Vector3D va;
	double ang, radius;

	// (a, b, c) is rotate axis
	radius = v.x*v.x + v.y*v.y;
	if (radius < 1.0e-4)
	{
		if (v.z < 0.)
		{
			va = Vector3D (0, 1, 0);
			ang = PI;
		}
		else
		{
			SetUnit ();
		}
	}
	else
	{
		radius = sqrt (radius);
		va = Vector3D (v.y / radius, -v.x / radius, 0);
		ang = getasin (radius);
		if (v.z < 0.) ang = PI - ang;
	}

	SetRotateV (va, ang);
}

void Matrix4::SetLocal (const Point3D& p0, const Vector3D& zv, const Vector3D& xv)
{
    Vector3D yv = zv ^ xv;
    mat[0][0] = xv.x;
    mat[1][0] = xv.y;
    mat[2][0] = xv.z;
    mat[3][0] = -(xv.x * p0.x + xv.y * p0.y + xv.z * p0.z);
    mat[0][1] = yv.x;
    mat[1][1] = yv.y;
    mat[2][1] = yv.z;
    mat[3][1] = -(yv.x * p0.x + yv.y * p0.y + yv.z * p0.z);
    mat[0][2] = zv.x;
    mat[1][2] = zv.y;
    mat[2][2] = zv.z;
    mat[3][2] = -(zv.x * p0.x + zv.y * p0.y + zv.z * p0.z);
    mat[0][3] = 0.0;
    mat[1][3] = 0.0;
    mat[2][3] = 0.0;
    mat[3][3] = 1.0;
}

void Matrix4::SetMirror (double a, double b, double c, double d)
{
	double l = sqrt(a*a + b*b + c*c);
	
	if (l > TOLER)
	{
		a /= l;
		b /= l;
		c /= l;
		d /= l;

		mat[0][0] = 1 - 2 * a * a;
		mat[1][0] = - 2 * a * b;
		mat[2][0] = - 2 * a * c;
		mat[3][0] = - 2 * a * d;
		mat[0][1] = - 2 * a * b;
		mat[1][1] = 1 - 2 * b * b;
		mat[2][1] = - 2 * b * c;
		mat[3][1] = - 2 * b * d;
		mat[0][2] = - 2 * a * c;
		mat[1][2] = - 2 * b * c;
		mat[2][2] = 1 - 2 * c * c;
		mat[3][2] = - 2 * c * d;
		mat[0][3] = 0;
		mat[1][3] = 0;
		mat[2][3] = 0;
		mat[3][3] = 1;
	}
}
