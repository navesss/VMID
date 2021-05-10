// GLVisualizer.cpp : implementation of the GLVisualizer class
//

#include "stdafx.h"
#include "GLVisualizer.h"

#include "volume.h"
#include "ProteinSignature.h"
#include "_global.h"
#include "_common.h"
#include "Cell.h"
#include "mrc2surf.h"
#include "Mesh.h"

#include "Material.h"
#include "Matrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////
// class GLVolumeVertex 
///////////////////////////////////////////////////////

GLVolumeVertex::GLVolumeVertex()
{
}

GLVolumeVertex::GLVolumeVertex(Point3D pt)
{
	m_point = pt;
}

GLVolumeVertex::~GLVolumeVertex()
{

}

///////////////////////////////////////////////////////
// class GLVolume 
///////////////////////////////////////////////////////

GLVolume::GLVolume()
{
	m_dimx = m_dimy = m_dimz = 0;
	m_signature = NULL;

	Free();	
}

GLVolume::GLVolume(ProteinSignature *signature)
{
	Volume *vol = signature->m_vol;
	m_signature = signature;

	m_dimx = vol->getSizeX();
	m_dimy = vol->getSizeY();
	m_dimz = vol->getSizeZ();

	m_matrixID = NULL;

	Create();
}

GLVolume::~GLVolume()
{
	Free();
}

void GLVolume::Free()
{
	int i;

	for(i=0; i<m_borderVertices.size(); i++)
		delete m_borderVertices[i];
	m_borderVertices.clear();

	for(i=0; i<m_inVertices.size(); i++)
		delete m_inVertices[i];
	m_inVertices.clear();

	for(i=0; i<m_sampleBorderPts.size(); i++)
		delete m_sampleBorderPts[i];
	m_sampleBorderPts.clear();

	for(i=0; i<m_outVertices.size(); i++)
		delete m_outVertices[i];
	m_outVertices.clear();

	if(m_matrixID != NULL)
	{
		delete m_matrixID;
		m_matrixID = NULL;
	}
}

void GLVolume::Create()
{
	int i;

	int id = 0;
	for(i=0; i<m_signature->m_borderPts.size(); i++)
	{
		Point3D *pt = m_signature->m_borderPts[i];
		GLVolumeVertex *vt = new GLVolumeVertex(*pt);

		vt->SetID(id);
		id ++;

		m_borderVertices.push_back(vt);
	}

	id = 0;
	for(i=0; i<m_signature->m_inPts.size(); i++)
	{
		Point3D *pt = m_signature->m_inPts[i];
		GLVolumeVertex *vt = new GLVolumeVertex(*pt);

		vt->SetID(id);
		id ++;

		m_inVertices.push_back(vt);
	}	

	id = 0;
	for(i=0; i<m_signature->m_sampleBorderPts.size(); i++)
	{
		Point3D pt = m_signature->m_sampleBorderPts[i];
		GLVolumeVertex *vt = new GLVolumeVertex(pt);

		vt->SetID(id);
		id ++;

		m_sampleBorderPts.push_back(vt);
	}

	// collect outside points
	Volume *vol = m_signature->m_vol;

	int j, k, sizex, sizey, sizez;

	sizex = vol->getSizeX();
	sizey = vol->getSizeY();
	sizez = vol->getSizeZ();
	
	id = 0;
	// collect the boundary points
	for ( k = 0 ; k < sizex ; k ++ )
	{
		for ( j = 0 ; j < sizey ; j ++ )
		{
			for ( i = 0 ; i < sizez ; i ++ )
			{
				if ( vol->getDataAt( i, j, k ) == 0 )
				{
					Point3D pt = Point3D(i, j, k);
					GLVolumeVertex *vt = new GLVolumeVertex(pt);

					vt->SetID(id);
					id ++;

					m_outVertices.push_back(vt);
				}
			}
		}
	}
}

C_EntityBox GLVolume::CalBox()
{
	int i, n;
	Point3D pmin, pmax, pt;

	n = m_borderVertices.size();
	pt = m_borderVertices[0]->getPoint();

	pmin = pmax = pt;
	for(i=1; i<n; i++)
	{
		pt = m_borderVertices[i]->getPoint();

		pmin = Min(pt, pmin);
		pmax = Max(pt, pmax);
	}

	return C_EntityBox(pmin, pmax);
}

Point3D GLVolume::calculateCenter()
{
	int i, num;

	Point3D cp;

	num = m_borderVertices.size();
	for(i=0; i<num; i++)
	{
		Point3D pt = m_borderVertices[i]->getPoint();
		cp = cp + pt;
	}

	if(num > 0)
		cp = (1.0/num)*cp;

	return cp;	
}

void GLVolume::normalizeScale(Point3D center, float scale)
{
	int i, n1, n2, n3;

	n1 = m_borderVertices.size();

	for(i=0; i<n1; i++)
	{
		Point3D pt = m_borderVertices[i]->getPoint();
		pt = pt - center;

		m_borderVertices[i]->setPoint(pt);
	}

	n2 = m_inVertices.size();

	for(i=0; i<n2; i++)
	{
		Point3D pt = m_inVertices[i]->getPoint();
		pt = (pt - center)*scale;

		m_inVertices[i]->setPoint(pt);
	}

	n3 = m_outVertices.size();

	for(i=0; i<n3; i++)
	{
		Point3D pt = m_outVertices[i]->getPoint();
		pt = (pt - center)*scale;

		m_outVertices[i]->setPoint(pt);
	}
}

void GLVolume::normalizeScaleSamplePoints(Point3D center, float scale)
{
	int i, n;
	n = m_sampleBorderPts.size();

	for(i=0; i<n; i++)
	{
		Point3D pt = m_sampleBorderPts[i]->getPoint();
		pt = (pt - center)*scale;

		m_sampleBorderPts[i]->setPoint(pt);
	}
}

void GLVolume::drawVolumeBorderPoints()
{
	glShadeModel(GL_SMOOTH);

	int i, nPts;

	nPts = m_borderVertices.size();

	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	glColor3f(0.7,0.7,0.7);
	for(i=0; i<nPts; i++)
	{
		Point3D pt = m_borderVertices[i]->getPoint();
		glVertex3f(pt.x, pt.y, pt.z);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void GLVolume::drawVolumeBorderPoints2()
{
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	int i;
	Point3D pt;
	float r, g, b;

	r = 155; g = 155; b = 155;

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

	GLUquadricObj *quadObj;
	double scale = 1.0;
	for(i=0; i<m_borderVertices.size(); i++)
	{
		Point3D pt = m_borderVertices[i]->getPoint();

		double radius = scale*0.15;
		glPushMatrix();
		glTranslated(pt.x, pt.y, pt.z);
		quadObj = gluNewQuadric();
		gluSphere(quadObj, radius, 5, 5);
		gluDeleteQuadric(quadObj);
		glPopMatrix();
	}
}

void GLVolume::drawVolumeFullPoints()
{
	glShadeModel(GL_SMOOTH);

	int i;

	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);
	glColor3f(0.5,0.5,0.5);

	for(i=0; i<m_inVertices.size(); i++)
	{
		Point3D pt = m_inVertices[i]->getPoint();
		glVertex3f(pt.x, pt.y, pt.z);
	}

	for(i=0; i<m_borderVertices.size(); i++)
	{
		Point3D pt = m_borderVertices[i]->getPoint();
		glVertex3f(pt.x, pt.y, pt.z);
	}

	glEnd();
	glEnable(GL_LIGHTING);
}

void GLVolume::drawVolumeMRCPoints()
{
	glShadeModel(GL_SMOOTH);

	int i;

	glDisable(GL_LIGHTING);
	glBegin(GL_POINTS);

	glColor3f(0,0,1);
	for(i=0; i<m_outVertices.size(); i++)
	{
		Point3D pt = m_outVertices[i]->getPoint();
		glVertex3f(pt.x, pt.y, pt.z);
	}

	glColor3f(0.7,0.7,0.7);

	for(i=0; i<m_inVertices.size(); i++)
	{
		Point3D pt = m_inVertices[i]->getPoint();
		glVertex3f(pt.x, pt.y, pt.z);
	}

	for(i=0; i<m_borderVertices.size(); i++)
	{
		Point3D pt = m_borderVertices[i]->getPoint();
		glVertex3f(pt.x, pt.y, pt.z);
	}

	glEnd();
	glEnable(GL_LIGHTING);
}

void GLVolume::drawVolumeSamplePoints()
{
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	int i;
	Point3D pt;
	float r, g, b;

	r = 255; g = 0; b = 0;

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

	GLUquadricObj *quadObj;
	double scale = 2.0;
	for(i=0; i<m_sampleBorderPts.size(); i++)
	{
		Point3D pt = m_sampleBorderPts[i]->getPoint();

		double radius = scale*0.25;
		glPushMatrix();
		glTranslated(pt.x, pt.y, pt.z);
		quadObj = gluNewQuadric();
		gluSphere(quadObj, radius, 10, 10);
		gluDeleteQuadric(quadObj);
		glPopMatrix();
	}
}


void GLVolume::drawVolumeIDVisibility2()
{
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	int i, j;
	float r, g, b;

	r = 0; g = 255; b = 0;

	// draw the ID Visibility
	if(m_matrixID == NULL)
		return;

	int nPts = m_matrixID->m_size;
	float	**mat = m_matrixID->m_mat;
	float	err = 1e-8;

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

	
	for(i=0; i<nPts; i++)
	{
		for(j=i+1; j<nPts; j++)
		{
			if(mat[i][j] > err && mat[j][i] > err)
			{

				Point3D p1 = m_sampleBorderPts[i]->getPoint();
				Point3D p2 = m_sampleBorderPts[j]->getPoint();

				float r = 0.2;
				//DrawGLCylinder(p1, p2, r, r);
				DrawGLCylinder2(p1, p2, r, r);

			}
		}
	}

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
}

/*
void DrawGLCylinder(Point3D p1, Point3D p2, float r1, float r2 )	// Draw cube
{

	GLUquadricObj *cyl, *disk;  
	cyl  = gluNewQuadric();
	disk = gluNewQuadric();

	double pi = 3.1415926535;
	double r2d = 180.0/pi;	// radians to degrees conversion factor
	double sf = 1.5;		// scale factor
	double sfi = 1.0/sf;	// inverse scale factor

	double rb = 0.2;	// radius of cylinder bottom
	double rt = 0.2;	// radius of cylinder bottom
	int dth = 10;		// no. of angular cylinder subdivsions 
	int dz = 10;		// no. of cylinder subdivsions in z direction

	// orientation vector 
	double vx = p2[0]-p1[0];	//	component in x-direction
	double vy = p2[1]-p1[1];	//	component in y-direction
	double vz = p2[2]-p1[2];	//	component in z-direction

	double v = sqrt( vx*vx + vy*vy + vz*vz );	// cylinder length

	// rotation vector, z x r  
	double rx = -vy*vz;
	double ry = +vx*vz;
	double ax = 0.0;

	double zero = 1.0e-3;
	if (fabs(vz) < zero)
	{
		ax = r2d*acos( vx/v );	// rotation angle in x-y plane
		if ( vx <= 0.0 ) ax = -ax;
	}
	else
	{
		ax = r2d*acos( vz/v );	// rotation angle
		if ( vz <= 0.0 ) ax = -ax;
	}

	//	Debug:
	//	cstr.Format("vz =%f", vz); 
	//	::MessageBox(NULL, cstr, "Information", MB_OK); 

	//glColor3f(1.0f, 0.0f, 0.5f);	// set cylinder color

	glPushMatrix();

	glTranslated( p1[0], p1[1], p1[2] );	// translate to point 1

	if (fabs(vz) < zero)
	{
		glRotated(90.0, 0, 1, 0.0);			// Rotate & align with x axis
		glRotated(ax, -1.0, 0.0, 0.0);		// Rotate to point 2 in x-y plane
	}
	else
	{
		glRotated(ax, rx, ry, 0.0);			// Rotate about rotation vector
	}

	gluCylinder(cyl, rb, rt, v, dth, dz);	// draw side faces of cylinder
	gluDisk( disk, 0.0, rb, dth, 1);		// draw bottom of cylinder

	glPopMatrix();

	glPushMatrix();

	glTranslated( p2[0], p2[1], p2[2] );	// translate to point 2

	if (fabs(vz) < zero)
	{
		glRotated(90.0, 0, 1, 0.0);			// Rotate & align with x axis
		glRotated(ax, -1.0, 0.0, 0.0);		// Rotate to point 2 in the x-y plane
	}
	else
	{
		glRotated(ax, rx, ry, 0.0);			// Rotate about rotation vector
	}

	gluDisk( disk, 0.0, rt, dth, 1);		// draw top of cylinder

	glPopMatrix();

	gluDeleteQuadric( cyl );
	gluDeleteQuadric( disk );

}*/

void DrawGLCylinder(Point3D p1, Point3D p2, float r1, float r2 )	// Draw cube
{

	GLUquadricObj *cyl, *disk;  
	cyl  = gluNewQuadric();
	disk = gluNewQuadric();

	double pi = 3.1415926535;
	double r2d = 180.0/pi;	// radians to degrees conversion factor
	double sf = 1.5;		// scale factor
	double sfi = 1.0/sf;	// inverse scale factor

	double rb = 0.2;	// radius of cylinder bottom
	double rt = 0.2;	// radius of cylinder bottom
	int dth = 10;		// no. of angular cylinder subdivsions 
	int dz = 10;		// no. of cylinder subdivsions in z direction

	// orientation vector 
	double vx = p2[0]-p1[0];	//	component in x-direction
	double vy = p2[1]-p1[1];	//	component in y-direction
	double vz = p2[2]-p1[2];	//	component in z-direction

	double v = sqrt( vx*vx + vy*vy + vz*vz );	// cylinder length

	// rotation vector, z x r  
	double rx = -vy*vz;
	double ry = +vx*vz;
	double ax = 0.0;

	double zero = 1.0e-3;
	if (fabs(vz) < zero)
	{
		ax = r2d*acos( vx/v );	// rotation angle in x-y plane
		if ( vx <= 0.0 ) ax = -ax;
	}
	else
	{
		ax = r2d*acos( vz/v );	// rotation angle
		if ( vz <= 0.0 ) ax = -ax;
	}

	//	Debug:
	//	cstr.Format("vz =%f", vz); 
	//	::MessageBox(NULL, cstr, "Information", MB_OK); 

	//glColor3f(1.0f, 0.0f, 0.5f);	// set cylinder color

	glPushMatrix();

	glTranslated( p1[0], p1[1], p1[2] );	// translate to point 1

	if (fabs(vz) < zero)
	{
		glRotated(90.0, 0, 1, 0.0);			// Rotate & align with x axis
		glRotated(ax, -1.0, 0.0, 0.0);		// Rotate to point 2 in x-y plane
	}
	else
	{
		glRotated(ax, rx, ry, 0.0);			// Rotate about rotation vector
	}

	gluCylinder(cyl, rb, rt, v, dth, dz);	// draw side faces of cylinder
	gluDisk( disk, 0.0, rb, dth, 1);		// draw bottom of cylinder

	glPopMatrix();

	glPushMatrix();

	glTranslated( p2[0], p2[1], p2[2] );	// translate to point 2

	if (fabs(vz) < zero)
	{
		glRotated(90.0, 0, 1, 0.0);			// Rotate & align with x axis
		glRotated(ax, -1.0, 0.0, 0.0);		// Rotate to point 2 in the x-y plane
	}
	else
	{
		glRotated(ax, rx, ry, 0.0);			// Rotate about rotation vector
	}

	gluDisk( disk, 0.0, rt, dth, 1);		// draw top of cylinder

	glPopMatrix();

	gluDeleteQuadric( cyl );
	gluDeleteQuadric( disk );

}


void DrawGLCylinder2(Point3D p1, Point3D p2, float r1, float r2 )	// Draw cylinder
{
	GLUquadricObj *quadObj;
	//int j, k;

	// ---------------------------------------
	// draw cylinder for normal direction
/*	Matrix4 matrix;
	double  pMatrix[16];
	C_EntityUcs ucs;
	Vector3D xaxis, zaxis;


	glPushMatrix();

	glTranslated(p1.x, p1.y, p1.z);

	xaxis = Vector3D(0, 0, 1);
	zaxis = p2 - p1;
	zaxis.SetUnit();

	float err = 1e-6;
	Vector3D temp = xaxis^zaxis;
	temp.SetUnit();
	if(Length(temp) <= err)
		xaxis = Vector3D(0, 1, 0);

	xaxis = xaxis ^ zaxis;
	xaxis.SetUnit();

	ucs.SetUcs(p1, xaxis, zaxis);
	matrix = ucs.GetWcToUc();
	for(j=0; j<4; j++)
		for(k=0; k<4; k++)
			pMatrix[k+j*4] = matrix.mat[k][j];

	glMultMatrixd(pMatrix);

	quadObj = gluNewQuadric();
	float len = Distance(p1, p2);
	
	gluCylinder (quadObj, r1, r2, len, 10, 6);

	gluDeleteQuadric(quadObj);

	glPopMatrix();*/

	// draw line for normal
	glBegin(GL_LINES);
	glVertex3f(p1[0], p1[1], p1[2]);
	glVertex3f(p2[0], p2[1], p2[2]);
	glEnd();

	// draw cone for normal direction
	glPushMatrix();
	glTranslated(p1[0], p1[1], p1[2]);

	// 采用gems的矩阵变换,可行
	Matrix4 matrix;
	double  pMatrix[16];
	C_EntityUcs ucs;
	Vector3D xaxis(0, 0, 1);
	Vector3D zaxis = p2 - p1;
	zaxis.SetUnit();
	xaxis = xaxis ^ zaxis;
	xaxis.SetUnit();
	ucs.SetUcs(Point3D(0,0,0), xaxis, zaxis);
	matrix = ucs.GetWcToUc();
	for(int j=0; j<4; j++)
		for(int k=0; k<4; k++)
			pMatrix[k+j*4] = matrix.mat[k][j];

	glMultMatrixd(pMatrix);

	quadObj = gluNewQuadric();
	float len = Distance(p1, p2);

	gluCylinder (quadObj, r1, r2, len, 12, 6);
	gluDeleteQuadric(quadObj);
	glPopMatrix();

}


void GLVolume::drawVolumeIDVisibility()
{
	int i, j;

	float r, g, b;
	r = 0;	g = 255; b = 0;


	// draw the ID Visibility
	if(m_matrixID == NULL)
		return;

	int nPts = m_matrixID->m_size;
	float	**mat = m_matrixID->m_mat;
	float	err = 1e-8;

	glDisable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glLineWidth(2.0f);
	glColor3f(r/255.0, g/255.0, b/255.0);

	for(i=0; i<nPts; i++)
	{
		for(j=i+1; j<nPts; j++)
		{
			if(mat[i][j] > err && mat[j][i] > err)
			{
				glBegin(GL_LINES);
				{
					Point3D p1 = m_sampleBorderPts[i]->getPoint();
					Point3D p2 = m_sampleBorderPts[j]->getPoint();

					glVertex3f(p1[0], p1[1], p1[2]);
					glVertex3f(p2[0], p2[1], p2[2]);
				}	
				glEnd();
			}
		}
	}

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
}

void GLVolume::buildSamplePoints(int sample_num, int sample_method)
{
	int i, id;

	for(i=0; i<m_sampleBorderPts.size(); i++)
		delete m_sampleBorderPts[i];
	m_sampleBorderPts.clear();

	sample_method = sample_method + 1;
	VolumeSampleBorderPoints_fy(m_signature->m_borderPts, sample_num, 
		m_signature->m_sampleBorderPts, sample_method);

	id = 0;
	for(i=0; i<m_signature->m_sampleBorderPts.size(); i++)
	{
		Point3D pt = m_signature->m_sampleBorderPts[i];
		GLVolumeVertex *vt = new GLVolumeVertex(pt);

		vt->SetID(id);
		id ++;

		m_sampleBorderPts.push_back(vt);
	}
}

void GLVolume::buildEuclideanDistance(int sample_num, int sample_method)
{
	int i, id;

	m_sampleBorderPts.clear();

	if(m_matrixID != NULL)
	{
		delete m_matrixID;
		m_matrixID = NULL;
	}

	sample_method = sample_method + 1;
	m_matrixID = m_signature->ED_matrix(sample_num, sample_method);

	id = 0;
	for(i=0; i<m_signature->m_sampleBorderPts.size(); i++)
	{
		Point3D pt = m_signature->m_sampleBorderPts[i];
		GLVolumeVertex *vt = new GLVolumeVertex(pt);

		vt->SetID(id);
		id ++;

		m_sampleBorderPts.push_back(vt);
	}
}

void GLVolume::buildIDVisibility(int sample_num, int sample_method)
{
	int i, id;

	for(i=0; i<m_sampleBorderPts.size(); i++)
		delete m_sampleBorderPts[i];
	m_sampleBorderPts.clear();

	if(m_matrixID != NULL)
	{
		delete m_matrixID;
		m_matrixID = NULL;
	}

	sample_method = sample_method + 1;
	m_matrixID = m_signature->ID_matrix(sample_num, sample_method);

	id = 0;
	for(i=0; i<m_signature->m_sampleBorderPts.size(); i++)
	{
		Point3D pt = m_signature->m_sampleBorderPts[i];
		GLVolumeVertex *vt = new GLVolumeVertex(pt);

		vt->SetID(id);
		id ++;

		m_sampleBorderPts.push_back(vt);
	}
}
///////////////////////////////////////////////////////
// class GLVisualizer 
///////////////////////////////////////////////////////
GLVisualizer::GLVisualizer()
{
	m_signature = NULL;
	m_volCell = NULL;
	m_volCell2 = NULL;
	m_volGLVolume = NULL;
	m_volMesh = NULL;

	m_center = Point3D(0, 0, 0);
	m_scale = 1.0f;
}

GLVisualizer::GLVisualizer(ProteinSignature *sig)
{

	m_signature = NULL;
	m_volCell = NULL;
	m_volCell2 = NULL;
	m_volGLVolume = NULL;
	m_volMesh = NULL;

	m_signature = sig;

	recalculate();
}

GLVisualizer::~GLVisualizer()
{
	if(m_volGLVolume)
	{
		delete m_volGLVolume;
		m_volGLVolume = NULL;
	}

	if(m_volCell)
	{
		delete m_volCell;
		m_volCell = NULL;
	}

	if(m_volCell2)
	{
		delete m_volCell2;
		m_volCell2 = NULL;
	}

	if(m_volMesh)
	{
		delete m_volMesh;
		m_volMesh = NULL;
	}
}

void GLVisualizer::drawSamplePoints()
{
	m_volGLVolume->drawVolumeSamplePoints();
}

void GLVisualizer::drawVolumeIDVisibility(int type)
{
	if(type == 1)
		m_volGLVolume->drawVolumeIDVisibility();
	else if(type == 2)
		m_volGLVolume->drawVolumeIDVisibility2();
}

void GLVisualizer::Draw(int type)
{
	//int mode = DRAW_MESH_SMOOTH;
	int mode = DRAW_MESH_WIRE;

	switch (type)
	{
	case DRAW_VOLUME_BORDER_POINTS:
		if(m_volGLVolume)
			m_volGLVolume->drawVolumeBorderPoints();
		break;
	case DRAW_VOLUME_BORDER_POINTS2:
		if(m_volGLVolume)
			m_volGLVolume->drawVolumeBorderPoints2();
		break;
	case DRAW_VOLUME_FULL_POINTS:
		if(m_volGLVolume)
			m_volGLVolume->drawVolumeFullPoints();
		break;
	case DRAW_VOLUME_MRC:
		if(m_volGLVolume)
			m_volGLVolume->drawVolumeMRCPoints();
		break;
	case DRAW_VOLUME_CELL:
		if(m_volCell)
			m_volCell->draw();
		break;
	case DRAW_VOLUME_CELL2:
		if(m_volCell2)
			m_volCell2->draw();
		break;
	case DRAW_VOLUME_SURFACE:
		if(m_volMesh)
			m_volMesh->draw(mode);
		break;
	default:
		break;
	}
}


void GLVisualizer::buildGLVolume()
{
	if(m_volGLVolume)
		delete m_volGLVolume;

	m_volGLVolume = new GLVolume(m_signature);
	m_volGLVolume->m_signature = m_signature;
}

void GLVisualizer::buildCell()
{
	float thr = 0.0001f;

	if(m_volCell != NULL)
		delete m_volCell;

	//m_volCell = vol2cell2(m_sig->m_vol, thr);
	m_volCell = vol2cell(m_signature->m_vol, thr);
}

void GLVisualizer::buildCell2()
{
	float thr = 0.0001f;

	if(m_volCell2 != NULL)
		delete m_volCell2;

	m_volCell2 = vol2cell2(m_signature->m_vol, thr);
}

void GLVisualizer::buildMesh()
{
	Volume * vol = m_signature->m_vol;

	float tmax = vol->getMax();;
	float tmin = vol->getMin();
	float threshold = 0.5*(tmax+tmin);
	vol->threshold2( threshold, 0, 1 ) ;

	// 1. mrc --> vrml
	VRMLobject *MyVRMLobject = mrc2vrml(vol, threshold);

	// 2. vrml --> mesh
	m_volMesh = vrml2mesh(MyVRMLobject);

	VRMLobject_relese(MyVRMLobject);
}

void GLVisualizer::buildSamplePoints(int sample_num, int sample_method)
{
	if(m_volGLVolume == NULL)
		return;

	m_volGLVolume->buildSamplePoints(sample_num, sample_method);

	m_volGLVolume->normalizeScaleSamplePoints(m_center, 1.0f);
}

void GLVisualizer::buildEuclideanDistance(int sample_num, int sample_method)
{
	if(m_volGLVolume == NULL)
		return;

	m_volGLVolume->buildEuclideanDistance(sample_num, sample_method);

	m_volGLVolume->normalizeScaleSamplePoints(m_center, 1.0f);
}

void GLVisualizer::buildIDVisibility(int sample_num, int sample_method)
{
	if(m_volGLVolume == NULL)
		return;

	m_volGLVolume->buildIDVisibility(sample_num, sample_method);

	m_volGLVolume->normalizeScaleSamplePoints(m_center, 1.0f);
}

CMesh* vrml2mesh(VRMLobject *vrml)
{
	CVertex *pV;
	CFace	*pF;
	int id;

	CMesh *mesh = new CMesh;

	Point3DList *tmp1 = vrml->PointListHead;
	Triangle *tmp2 = vrml->TriangleListHead;

	id = 0;
	while (tmp1!=0)
	{
		pV = new CVertex(Point3D(tmp1->x,tmp1->y,tmp1->z));
		pV->SetID(id);
		mesh->addVertex(pV);

		id ++;
		tmp1 = tmp1->next;
	}
	
	id = 0;
	while (tmp2!=0)
	{
		pF = new CFace;

		CVertex *v1 = mesh->getVertex(tmp2->b);
		CVertex *v2 = mesh->getVertex(tmp2->a);
		CVertex *v3 = mesh->getVertex(tmp2->c);
		
		pF->addVertex(v1);
		pF->addVertex(v2);
		pF->addVertex(v3);

		pF->SetID(id);
		mesh->addFace(pF);

		id ++;
		tmp2 = tmp2->next;
	}

	mesh->buildAdjacency();

	mesh->calculateFaceNormal();
	mesh->calculateVertexNormal();

	return mesh;
}

void GLVisualizer::recalculate()
{
	buildGLVolume();

	Point3D pmin, pmax;
	C_EntityBox box = m_volGLVolume->CalBox();
	box.GetBox(pmin, pmax);

	m_scale = Distance(pmax, pmin);
	m_center = m_volGLVolume->calculateCenter();

	//buildCell();
	buildCell2();
	buildMesh();

	normalizeScale();
}

void GLVisualizer::normalizeScale()
{
	if(m_volGLVolume)
		m_volGLVolume->normalizeScale(m_center, 1.0f);

	if(m_volCell)
		m_volCell->normalizeScale(m_center, 1.0f);
	if(m_volCell2)
		m_volCell2->normalizeScale(m_center, 1.0f);

	//m_volMesh->normalizeScale(m_center, 1.0f);
	if(m_volMesh)
		m_volMesh->normalizeScale2(m_scale);
}