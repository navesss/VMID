// GLVisualizer.h : interface of the GLVisualizer class
//

#pragma once

#include <vector>
#include "Point.h"
#include "Object3d.h"
#include "Vrmlobject.h "
#include "Entity.h"


using namespace std;

class CObject3d;
class Volume;
class ProteinSignature;
class GLVolume;
class CCell;
class CMesh;
class Matrix2D;


// The class in this file is for display
class GLVolumeVertex : public CObject3d
{
public:
	Point3D m_point;

public:
	GLVolumeVertex();
	GLVolumeVertex(Point3D pt);
	
	~GLVolumeVertex();

	Point3D getPoint(){return m_point;}
	void setPoint(Point3D pt){m_point = pt;}
};

class GLVolume : public CObject3d
{
public:
	ProteinSignature		*m_signature;

	int						m_dimx, m_dimy, m_dimz;
	vector<GLVolumeVertex*>	m_borderVertices;
	vector<GLVolumeVertex*>	m_inVertices;
	vector<GLVolumeVertex*>	m_outVertices;

	vector<GLVolumeVertex*> m_sampleBorderPts;

	Matrix2D				*m_matrixID;

public:
	GLVolume();
	GLVolume(ProteinSignature *signature);
	~GLVolume();

	void Free();
	void Create();

	C_EntityBox CalBox();
	Point3D calculateCenter();
	void normalizeScale(Point3D center, float scale);
	void normalizeScaleSamplePoints(Point3D center, float scale);
	
	// METHODS
	void drawVolumeBorderPoints();
	void drawVolumeBorderPoints2();
	void drawVolumeFullPoints();
	void drawVolumeMRCPoints();
	void drawVolumeSamplePoints();
	void drawVolumeIDVisibility();
	void drawVolumeIDVisibility2();

	void buildSamplePoints(int sample_num, int sample_method);
	void buildEuclideanDistance(int sample_num, int sample_method);
	void buildIDVisibility(int sample_num, int sample_method);
};

class GLVisualizer 
{
public:

	ProteinSignature	*m_signature;

	// display model
	GLVolume			*m_volGLVolume;
	CCell				*m_volCell;
	CCell				*m_volCell2;
	CMesh				*m_volMesh;

	Point3D				m_center;
	float				m_scale;

public:

	GLVisualizer();
	GLVisualizer(ProteinSignature *sig);
	~GLVisualizer();

	void Draw(int type);
	void drawSamplePoints();
	void drawVolumeIDVisibility(int type);

	void buildGLVolume();
	void buildCell();
	void buildCell2();
	void buildMesh();

	// for Inner Distance
	void buildSamplePoints(int sample_num, int sample_method);

	void buildEuclideanDistance(int sample_num, int sample_method);

	void buildIDVisibility(int sample_num, int sample_method);

	void recalculate();

	void normalizeScale();
	
};

CMesh* vrml2mesh(VRMLobject *vrml);

// Draw cube
void DrawGLCylinder(Point3D p1, Point3D p2, float r1, float r2 );

void DrawGLCylinder2(Point3D p1, Point3D p2, float r1, float r2 );

