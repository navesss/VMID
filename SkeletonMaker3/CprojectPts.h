#pragma once

#include <iostream>
#include "point.h"

using namespace std;
class CprojectPts
{
public:
	CprojectPts(void);
	void SetValue(float f,Point3D pt);

	float m_f;
	Point3D m_pt;

	bool operator < (const CprojectPts &m)
	{
		return m_f<m.m_f;
	}

public:
	~CprojectPts(void);
};
