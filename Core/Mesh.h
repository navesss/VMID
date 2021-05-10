// Mesh.h: CMesh class
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "point.h"
#include "object3d.h"
#include "Entity.h"


using namespace std;

class CObject3d;
class CVertex;
class CFace;
class CMesh;

class CVertex : public CObject3d
{
private:
	Point3D		m_oriPoint; // backup original point

	Point3D		m_point;
	Vector3D	m_normal;
	vector<CFace*>		m_faceNeighborArray;
	vector<CVertex*>	m_vertexNeighborArray;

public:
	int			m_group;
	
public:
	CVertex();
	CVertex(Point3D pt);
	CVertex(CVertex *v);
	CVertex(const float x,const float y,const float z);
	~CVertex();

	void init();

	// color


	Point3D getPoint(){return m_point;}
	void setPoint(Point3D pt){m_point = pt;}

	Point3D getOriPoint(){return m_oriPoint;}
	void setOriPoint(Point3D pt){m_oriPoint = pt;}
	
	Vector3D getNormal(){return m_normal;}
	void setNormal(Vector3D n){m_normal = n;}

	int hasFaceNeighbor(CFace *pF);
	int NbFaceNeighbor(){return m_faceNeighborArray.size();}
	CFace* getFaceNeighbor(int i){return m_faceNeighborArray[i];}
	void addFaceNeighbor(CFace *pF);
	void removeAllFaceNeighbor(){m_faceNeighborArray.clear();}

	int hasVertexNeighbor(CVertex *pV);
	int NbVertexNeighbor(){return m_vertexNeighborArray.size();}
	CVertex* getVertexNeighbor(int i){return m_vertexNeighborArray[i];}
	void addVertexNeighbor(CVertex *pV);
	void removeAllVertexNeighbor(){m_vertexNeighborArray.clear();}

	void calculateNormal();
};

class CFace : public CObject3d
{
public:
	vector<CVertex*>	m_vertexArray;
	Vector3D			m_normal;

	vector<CFace*>		m_faceNeighborArray;
public:
	CFace();
	CFace(vector<CVertex*> vertexArray);
	CFace(CVertex *pVertex1,CVertex *pVertex2,CVertex *pVertex3);
	~CFace();

	int NbVertex(){return m_vertexArray.size();}
	CVertex* getVertex(int index){return m_vertexArray[index];}
	void addVertex(CVertex* v){m_vertexArray.push_back(v);}
	vector<CVertex*> getAllVertex(){return m_vertexArray;}
	void setVertex(int index, CVertex *v){m_vertexArray[index] = v;}

	Vector3D getNormal(){return m_normal;}
	void setNormal(Vector3D n){m_normal = n;}

	int IndexFrom(CVertex *pV);

	int hasVertex(CVertex *pV);
	int hasFaceNeighbor(CFace *pF);
	int NbFaceNeighbor(){return m_faceNeighborArray.size();}
	CFace* getFaceNeighbor(int i){return m_faceNeighborArray[i];}
	void setFaceNeighbor(int index, CFace *pF){m_faceNeighborArray[index] = pF;}
	void removeAllFaceNeighbor(){m_faceNeighborArray.clear();}
	
	void calculateNormal();

	float area();
};

class CMesh : public CObject3d
{
public:
	vector<CVertex*>	m_vertexArray;
	vector<CFace*>		m_faceArray;

public:
	CMesh();
	~CMesh();

	int NbFace(){return m_faceArray.size();}
	int NbVertex(){return m_vertexArray.size();}

	CFace* getFace(int index){return m_faceArray[index];}
	void addFace(CFace *face){m_faceArray.push_back(face);}
	void setFace(CFace *face, int index){m_faceArray[index] = face;}
	vector<CFace*> *getFaceArray(){return &m_faceArray;}

	CVertex* getVertex(int index){return m_vertexArray[index];}
	void addVertex(CVertex *vertex){m_vertexArray.push_back(vertex);}
	void setVertex(CVertex *vertex, int index){m_vertexArray[index] = vertex;}
	vector<CVertex*> *getVertexArray(){return &m_vertexArray;}

	void calculateFaceNormal();
	void calculateVertexNormal();
	void reverseNormal();
	int buildAdjacency();

	void draw(int mode);
	void drawFlatShade();
	void drawWireframe();
	void drawSmoothShade();

	C_EntityBox CalBox();
	void normalizeScale(Point3D center, float scale);
	void normalizeScale2(float scale);
};


// Constructor
inline CVertex::CVertex(const float x,const float y,const float z) 
{
	m_oriPoint[0] = x; 
	m_oriPoint[1] = y; 
	m_oriPoint[2] = z; 
	init();
}