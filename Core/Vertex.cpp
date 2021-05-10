// PolygonalMesh.cpp: PolygonalMesh 
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mesh.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4244 4018 4996)

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

CVertex::CVertex()
{
	init();
}

CVertex::CVertex(Point3D pt)
{
	init();

	m_point = m_oriPoint = pt;
}

CVertex::CVertex(CVertex *v)
{
	init();

	this->SetID(v->GetID());
	this->setPoint(v->getPoint());
	this->setNormal(v->getNormal());	
	this->setOriPoint(v->getOriPoint());
}

void CVertex::init()
{
	m_point = m_oriPoint = Point3D(0, 0, 0);
	m_normal = Vector3D(0, 0, 0);

	m_group = -1;
}

CVertex::~CVertex()
{
}

int CVertex::hasFaceNeighbor(CFace *pF)
{
	for(int i=0; i<m_faceNeighborArray.size(); i++)
	{
		if(m_faceNeighborArray[i] == pF)
			return 1;
	}
	return 0;
}

int CVertex::hasVertexNeighbor(CVertex *pV)
{
	for(int i=0; i<m_vertexNeighborArray.size(); i++)
	{
		if(m_vertexNeighborArray[i] == pV)
			return 1;
	}
	return 0;
}

void CVertex::addFaceNeighbor(CFace *pF)
{
	if(!hasFaceNeighbor(pF))
		m_faceNeighborArray.push_back(pF);
}

void CVertex::addVertexNeighbor(CVertex *pV)
{
	if(pV == this)
		return;
	if(!hasVertexNeighbor(pV))
		m_vertexNeighborArray.push_back(pV);
}

// no implement
void CVertex::calculateNormal()
{
	int i;
	CVertex *pV = this;
	Vector3D vector;
	for(i=0; i<pV->NbFaceNeighbor(); i++)
	{
		CFace *pF = pV->getFaceNeighbor(i);
		vector += pF->getNormal();
	}
	vector.SetUnit();
	m_normal = vector;
}