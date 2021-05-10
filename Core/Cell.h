// Mesh.h: CMesh class
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "point.h"


using namespace std;

class Volume;

class CCellFace
{
public:
	int m_vts[4];

public:
	CCellFace(){};
	CCellFace(int v1, int v2, int v3, int v4)
	{
		m_vts[0] = v1;
		m_vts[1] = v2;
		m_vts[2] = v3;
		m_vts[3] = v4;
	};

	~CCellFace(){};
};
 
class CCell
{
public:
	vector<Point3D>			m_vertices;
	vector<CCellFace*>		m_faces;

public:
	CCell();
	CCell(Volume *vol);
	~CCell();

	int NbFace(){return m_faces.size();}
	int NbVertex(){return m_vertices.size();}
	
	void normalizeScale(Point3D center, float scale);

	void draw();
};

CCell* vol2cell2(Volume *vol, float thr);

CCell* vol2cell(Volume *vol, float thr);