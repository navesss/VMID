#include "stdafx.h"
#include "entity.h"
//#include "viewstru.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

C_EntityBox::C_EntityBox(void)
{
	m_min = Point3D(-2000,-2000,-2000);
	m_max = Point3D(2000,2000,2000);
}

C_EntityBox::C_EntityBox(Point3D min, Point3D max)
{
	m_min = min;
	m_max = max;
}

C_EntityBox C_EntityBox::GetBox(void)
{
	return *this;
}

void C_EntityBox::GetBox(Point3D &min, Point3D &max)
{
	min = m_min;
	max = m_max;
}

void C_EntityBox::SetBox(C_EntityBox box)
{
	//box.GetBox(m_min,m_max);
    m_min = box.m_min;
    m_max = box.m_max;
}

void C_EntityBox::SetBox(Point3D min, Point3D max)
{
	m_min = min;
	m_max = max;
}

// 判相交
BOOL C_EntityBox::operator & (const C_EntityBox& b) const
{
    if (m_min.x > b.m_max.x + TOLER || m_max.x < b.m_min.x - TOLER ||
        m_min.y > b.m_max.y + TOLER || m_max.y < b.m_min.y - TOLER ||
        m_min.z > b.m_max.z + TOLER || m_max.z < b.m_min.z - TOLER)
        return FALSE;
    else
        return TRUE;
}

// 求并
C_EntityBox C_EntityBox::operator + (const C_EntityBox& b) const
{
    Point3D p1, p2;
	p1 = Min ( m_min, b.m_min );
	p2 = Max ( m_max, b.m_max );
    return C_EntityBox( p1, p2 );
}

// 求交
C_EntityBox C_EntityBox::operator * (const C_EntityBox& b) const
{
    Point3D p1, p2;
	p1 = Max ( m_min, b.m_min );
	p2 = Min ( m_max, b.m_max );
    return C_EntityBox( p1, p2 );
}
