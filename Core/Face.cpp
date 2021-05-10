// PolygonalMesh.cpp: PolygonalMesh 
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "stdafx.h"
#include "Mesh.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
CFace::CFace()
{
}

CFace::CFace(vector<CVertex*> vertexArray)
{
	m_vertexArray = vertexArray;
}

//********************************************
// Constructor
//********************************************
CFace::CFace(CVertex *pVertex1,CVertex *pVertex2,CVertex *pVertex3)
{
	m_vertexArray.clear();

	addVertex(pVertex1);
	addVertex(pVertex2);
	addVertex(pVertex3);
}

CFace::~CFace()
{
	m_vertexArray.clear();
}

void CFace::calculateNormal()
{
	int i, n;
	n = m_vertexArray.size();
	Vector3D nor;
	ASSERT(n >= 3);
	for(i=0; i<n-2; i++)
	{
		Point3D p0 = m_vertexArray[i]->getPoint();
		Point3D p1 = m_vertexArray[(i+1)%n]->getPoint();
		Point3D	p2 = m_vertexArray[(i+2)%n]->getPoint();
		Vector3D v1 = p2 - p1;
		Vector3D v2 = p0 - p1;
		nor += v1^v2;
	}
//	Point3D p0 = m_vertexArray[0]->getPoint();
//	Point3D p1 = m_vertexArray[1]->getPoint();
//	Point3D	p2 = m_vertexArray[2]->getPoint();
//	Vector3D v1 = p2 - p1;
//	Vector3D v2 = p0 - p1;
//	nor += v1^v2;
	nor.SetUnit();
	m_normal = nor;
}

int CFace::IndexFrom(CVertex *pV)
{
	ASSERT(hasVertex(pV));
	for(int i=0; i<m_vertexArray.size(); i++)
		if(m_vertexArray[i] == pV)
			return i;
	return 0;
}

int CFace::hasVertex(CVertex *pV)
{
	for(int i=0; i<m_vertexArray.size(); i++)
	{
		if(m_vertexArray[i] == pV)
			return 1;
	}
	return 0;
}

int CFace::hasFaceNeighbor(CFace *pF)
{
	for(int i=0; i<m_faceNeighborArray.size(); i++)
	{
		if(m_faceNeighborArray[i] == pF)
			return 1;
	}
	return 0;
}


float CFace::area()
{
	Point3D p0, p1, p2;
	Vector3D v1, v2, vn;
	float	s, A;
	int		i, n, sign;

	n = m_vertexArray.size();
	p0 = m_vertexArray[0]->getPoint();
	A = s = 0.0;
	for(i=1; i<n-1; i++)
	{
		p1 = m_vertexArray[i]->getPoint();
		p2 = m_vertexArray[(i+1)%n]->getPoint();
		v1 = p1 - p0;
		v2 = p2 - p0;
		vn = v1^v2;

		Vector3D tempV = vn;
		tempV.SetUnit();
		float dir = tempV*m_normal;
		if(dir < 0.0)
			sign = 0;

		sign = dir ? 1 : -1;
		s = sign*Length(vn)/2.0;
		A += s;
	}
	return A;
}