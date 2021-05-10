// PolygonalMesh.cpp: PolygonalMesh 
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "stdafx.h"
#include "Mesh.h"
#include <stdio.h>
#include "_global.h"
#include "_common.h"

#include "Material.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4244 4018)

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	int i;
	for(i = 0; i<m_vertexArray.size(); i++)
		delete m_vertexArray[i];
	m_vertexArray.clear();
	
	for(i=0; i<m_faceArray.size(); i++)
		delete m_faceArray[i];
	m_faceArray.clear();
}

void CMesh::calculateFaceNormal()
{
	int i;
	for(i=0; i<m_faceArray.size(); i++)
		m_faceArray[i]->calculateNormal();
}

void CMesh::calculateVertexNormal()
{
	int i;
	for(i=0; i<m_vertexArray.size(); i++)
		m_vertexArray[i]->calculateNormal();
}

void CMesh::reverseNormal()
{
	int i;
	for(i=0; i<m_faceArray.size(); i++)	
	{
		Vector3D normal = m_faceArray[i]->getNormal();
		normal = normal*(-1.0);
		m_faceArray[i]->setNormal(normal);
	}
}

int CMesh::buildAdjacency()
{
	CMesh *mesh = this;
	int i, j, k, n, NbVertex, NbFace, index;
	CFace *pF;
	CVertex *pV;

	NbVertex = m_vertexArray.size();
	NbFace = m_faceArray.size();

	if(NbVertex < 3 || NbFace <= 0)
		return 0;

	for(i=0; i<NbFace; i++)
		m_faceArray[i]->removeAllFaceNeighbor();
	for(i=0; i<NbVertex; i++)
	{
		pV = m_vertexArray[i];
		pV->removeAllFaceNeighbor();
		pV->removeAllVertexNeighbor();
	}

	for(i=0; i<NbFace; i++)
	{
		pF = m_faceArray[i];
		n = pF->NbVertex();
		pF->m_faceNeighborArray.resize(n);
		for(k=0; k<pF->NbVertex(); k++)
			pF->setFaceNeighbor(k, NULL);
	}
	//---------------------------------------------
	// For each face, set face neighboring vertices 
	//---------------------------------------------
	for(i=0; i<NbFace; i++)
	{
		pF = m_faceArray[i];
		for(j=0; j<pF->NbVertex(); j++)
			pF->getVertex(j)->addFaceNeighbor(pF);
	}

	//---------------------------------------------
	// For each vertex, set vertex neighboring,
	// just look on neighboring faces 
	//---------------------------------------------
	for(i=0; i<NbVertex; i++)
	{
		CVertex *pV = m_vertexArray[i];
		for(j=0; j<pV->NbFaceNeighbor(); j++)
		{
			pF = pV->getFaceNeighbor(j);
			n = pF->NbVertex();
			index = pF->IndexFrom(pV);
			pV->addVertexNeighbor(pF->getVertex((index-1+n)%n));
			pV->addVertexNeighbor(pF->getVertex((index+1)%n));
		}
	}

	//---------------------------------------------
	// For each face, set face neighboring,
	// just look on faces neighboring vertices
	//---------------------------------------------
	for(i=0; i<NbFace; i++)
	{
		pF = m_faceArray[i];
		n = pF->NbVertex();
		for(j=0; j<n; j++)
		{
			CVertex *pVertex = pF->getVertex(j);
			CVertex *pNextVertex = pF->getVertex((j+1)%n);
			int NbFaceNeighbor = pVertex->NbFaceNeighbor();
			for(k=0; k<NbFaceNeighbor; k++)
			{
				CFace *pFace = pVertex->getFaceNeighbor(k);
				if(pFace != pF)
					if(pF->getFaceNeighbor(j) == NULL)
						if(pFace->hasVertex(pVertex))
							if(pFace->hasVertex(pNextVertex))
								pF->setFaceNeighbor(j, pFace);
			}
		}
	}

	return 1;
}

void CMesh::draw(int mode)
{
	float r, g, b;

	r = 100; g = 100; b = 100;

	CMaterial material;
	material.SetAmbient((float)r/255,(float)g/255,(float)b/255,1);
	material.SetDiffuse((float)r/255,(float)g/255,(float)b/255,1);
	material.SetSpecular(0,0,0,1);
	material.SetEmission(0,0,0,1);
	material.SetShininess(0);

	::glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material.GetAmbient());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material.GetDiffuse());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material.GetSpecular());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material.GetEmission());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,material.GetShininess());

	switch(mode) 
	{
	case DRAW_MESH_FLAT:
		drawFlatShade();
		break;
	case DRAW_MESH_WIRE:
		drawWireframe();
		break;
	case DRAW_MESH_SMOOTH:
		drawSmoothShade();
		break;
	default:
		break;
	}
}

void CMesh::drawFlatShade()
{
	int i, j, n, face_N;
	glShadeModel(GL_SMOOTH);
	face_N = NbFace();
	for(i=0; i<face_N; i++){
		CFace *face = getFace(i);
		n = face->NbVertex();
		Vector3D normal = face->getNormal();
		glBegin(GL_POLYGON);
		glNormal3f(normal.x, normal.y, normal.z);
		for(j=0; j<n; j++){
			Point3D pt = face->getVertex(j)->getPoint();
			glVertex3f(pt.x, pt.y, pt.z);
		}
		glEnd();
	}	
}

void CMesh::drawWireframe()
{
	int i, vertex_N = NbVertex();
	int face_N = NbFace();

	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);

	glColor3f(0.0f, 0.0f, 0.0f);
	for(i=0; i<face_N; i++){

		CFace *pF = getFace(i);
		int p_N = pF->NbVertex();
		for(int j=0; j<p_N; j++){
			Point3D p1 = pF->getVertex(j)->getPoint();
			Point3D p2 = pF->getVertex((j+1)%p_N)->getPoint();
			//			int i1 = f[j];
			//			int i2 = f[(j+1)%p_N];
			//			if(i1 > i2){
			glBegin(GL_LINES);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glEnd();
			//			}
		}
	}

	glEnable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	float d = (float)0.05;
	for(i=0; i<face_N; i++){

		CFace *pF = getFace(i);
		int n = pF->NbVertex();
		Vector3D normal = pF->getNormal();
		glBegin(GL_POLYGON);
		glNormal3f(normal.x, normal.y, normal.z);
		for(int j=0; j<n; j++){
			Point3D p1 = pF->getVertex(j)->getPoint();
			Point3D p2 = p1 - normal*d;
			glVertex3f(p2.x, p2.y, p2.z);
		}
		glEnd();
	}

	glEnable(GL_LIGHTING);	
}

void CMesh::drawSmoothShade()
{
	glShadeModel(GL_SMOOTH);
	int i, j, n, face_N;
	face_N = NbFace();
	for(i=0; i<face_N; i++){
		CFace *face = getFace(i);
		n = face->NbVertex();
		glBegin(GL_POLYGON);
		for(j=0; j<n; j++)
		{
			CVertex *vt = getVertex(j);
			Point3D pt = vt->getPoint();
			Vector3D normal = vt->getNormal();
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(pt.x, pt.y, pt.z);
		}
		glEnd();
	}
}

C_EntityBox CMesh::CalBox()
{
	int i, n;
	Point3D pmin, pmax, pt;

	n = NbVertex();
	pt = getVertex(0)->getPoint();

	pmin = pmax = pt;
	for(i=1; i<n; i++)
	{
		pt = getVertex(i)->getPoint();

		pmin = Min(pt, pmin);
		pmax = Max(pt, pmax);
	}

	return C_EntityBox(pmin, pmax);
}

void CMesh::normalizeScale(Point3D center, float scale)
{
	int i, n1;

	n1 = NbVertex();

	for(i=0; i<n1; i++)
	{
		CVertex *pV = getVertex(i);
		Point3D pt = pV->getOriPoint();
		pt = (pt - center)*scale;

		pV->setPoint(pt);
	}

}

void CMesh::normalizeScale2(float scale)
{
	Point3D pmin, pmax;
	C_EntityBox box = CalBox();
	box.GetBox(pmin, pmax);

	float ss = Distance(pmax, pmin);
	ss = 1.0/ss;
	Point3D center = (pmin + pmax)*0.5;

	int i, n1;

	n1 = NbVertex();

	for(i=0; i<n1; i++)
	{
		CVertex *pV = getVertex(i);
		Point3D pt = pV->getOriPoint();
		pt = (pt - center)*ss*scale;

		pV->setPoint(pt);
	}

}