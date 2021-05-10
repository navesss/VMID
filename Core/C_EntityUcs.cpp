#include "stdafx.h"
#include "entity.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

C_EntityUcs::C_EntityUcs()
{
    m_origin = Point3D( 0.0, 0.0, 0.0 );
    m_xaxis = Vector3D( 1.0, 0.0, 0.0 );
    m_zaxis = Vector3D( 0.0, 0.0, 1.0 );
	m_scale = 1.0;
}

C_EntityUcs::C_EntityUcs(const C_EntityUcs& ucs)
{
    m_origin = ucs.m_origin;
    m_xaxis = ucs.m_xaxis;
    m_zaxis = ucs.m_zaxis;
	m_scale = ucs.m_scale;
}

C_EntityUcs::C_EntityUcs(Point3D origin, Vector3D xaxis, Vector3D zaxis, double scale)
{
    m_origin = origin;
    m_xaxis = xaxis;
    m_zaxis = zaxis;
	m_scale = scale;
}

Point3D C_EntityUcs::GetOrigin()
{
	return m_origin;
}

Vector3D C_EntityUcs::GetUcsX()
{
	return m_xaxis;
}

Vector3D C_EntityUcs::GetUcsY()
{
    return m_zaxis ^ m_xaxis;
}

Vector3D C_EntityUcs::GetUcsZ()
{
    return m_zaxis;
}

Matrix4 C_EntityUcs::GetUcToWc() const// UCS to WCS
{
	Matrix4 m1, m2;
	
	//double scale = 1.0 / m_scale;
	double scale = m_scale;
	m1.SetScale(scale, scale, scale);
	m2.SetLocal(m_origin, m_zaxis, m_xaxis);
	return m1 * (!m2);
}

Matrix4 C_EntityUcs::GetWcToUc() const// WCS to UCS
{
	Matrix4 m1, m2;
	
	double scale = 1.0 / m_scale;
	m1.SetLocal(m_origin, m_zaxis, m_xaxis);
	m2.SetScale(scale, scale, scale);
	return m1 * m2;
}

void C_EntityUcs::SetUcs(Point3D origin, Vector3D xaxis, Vector3D zaxis, double scale)
{
    m_origin = origin;
    m_xaxis = xaxis;
	m_zaxis = zaxis;
	m_scale = scale;
}

void C_EntityUcs::SetUcs(double a, double b, double c, double d)
{
/*
	if (fabs (a) > TOLER)
		m_origin = Point3D (-d/a, 0, 0);
	else if (fabs (b) > TOLER)
		m_origin = Point3D (0, -d/b, 0);
	else if (fabs (c) > TOLER)
		m_origin = Point3D (0, 0, -d/c);
*/

	m_zaxis = Vector3D (a, b, c);
	if (fabs (c) > 0.01)
		m_xaxis = Vector3D (0, 1, 0) ^ m_zaxis;
	else
		m_xaxis = Vector3D (0, 0, 1) ^ m_zaxis;
    m_xaxis.SetUnit();

	m_origin = m_zaxis * (-d / (m_zaxis * m_zaxis));
}

Point3D C_EntityUcs::UcToWc(Point3D point)
{
    return point * GetUcToWc();
}

Point3D C_EntityUcs::WcToUc(Point3D point)
{
    return point * GetWcToUc();
}

void C_EntityUcs::Transform(Matrix4 matrix)
{
    m_origin = m_origin * matrix;
    m_xaxis = m_xaxis * matrix;
    m_zaxis = m_zaxis * matrix;
	m_scale *= (Length(m_xaxis) + Length(m_zaxis)) * 0.5;
	m_xaxis.SetUnit();
	m_zaxis.SetUnit();
}
/*
void C_EntityUcs::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_origin << m_xaxis << m_zaxis << m_scale;
	}
	else
	{
		ar >> m_origin >> m_xaxis >> m_zaxis >> m_scale;
	}
}
*/
C_EntityUcs C_EntityUcs::operator * (const C_EntityUcs& ucs)
{
	Matrix4 m = GetUcToWc();

	Point3D  no = ucs.m_origin * m;
	Vector3D nx = ucs.m_xaxis  * m;
	Vector3D nz = ucs.m_zaxis  * m;

	double scale = GetScale()*ucs.m_scale;

	return C_EntityUcs(no, nx, nz,scale);
}

void C_EntityUcs::operator *= (const C_EntityUcs& ucs)
{
	Matrix4 m = GetUcToWc();

	m_origin = ucs.m_origin * m;
	m_xaxis  = ucs.m_xaxis  * m;
	m_zaxis  = ucs.m_zaxis  * m;
	m_scale *= ucs.m_scale;
}

BOOL C_EntityUcs::operator == (const C_EntityUcs& ucs)
{
    if (m_origin != ucs.m_origin) return FALSE;
    if (Length(m_xaxis - ucs.m_xaxis) > TOLER) return FALSE;
    if (Length(m_zaxis - ucs.m_zaxis) > TOLER) return FALSE;
    return TRUE;
}

double C_EntityUcs::GetScale()
{
	return m_scale;
}

void C_EntityUcs::SetScale(double scale)
{
	m_scale = scale;
}