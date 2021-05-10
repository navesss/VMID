// PointSet.cpp
//
#include "stdafx.h"
#include "ProteinSignature.h"
#include "SignatureRender.h"
#include "Material.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4305 4244 4018)

void glDrawFrontFace(Point3D* b, Point3D* e)
{
	/*draw the front whole surface of a node, we just need the begin and end pointf that node*/
	float norm[] = {0.0, 0.0, -1.0};
	//glColor3f(0.4f, 0.4f, 0.4f);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_POLYGON);
		glNormal3fv(norm);
		glVertex3f(b->x, b->y, b->z);
		glVertex3f(b->x, e->y, b->z);
		glVertex3f(e->x, e->y, b->z);
		glVertex3f(e->x, b->y, b->z);
		glVertex3f(b->x, b->y, b->z);
	glEnd();
}

void glDrawFrontRect(Point3D* b, Point3D* e)
{
	/*draw the rectangle of the front surface of a node, we just need the begin and end pointf that node*/
	glColor3f(0.2f, 0.2f, 0.2f);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glDepthFunc(GL_ALWAYS);
	glBegin(GL_LINE_LOOP);
		glVertex3f(b->x, b->y, b->z);
		glVertex3f(e->x, b->y, b->z);
		glVertex3f(e->x, e->y, b->z);
		glVertex3f(b->x, e->y, b->z);
	glEnd();
	//glDepthFunc(GL_LESS);
}

void glDrawBackFace(Point3D* b, Point3D* e)
{
	/*same as above with the back face*/
	float norm[] = {0.0, 0.0, 1.0};
	//glColor3f(0.4f, 0.4f, 0.4f);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_POLYGON);
		glNormal3fv(norm);
		glVertex3f(e->x, e->y, e->z);
		glVertex3f(b->x, e->y, e->z);
		glVertex3f(b->x, b->y, e->z);
		glVertex3f(e->x, b->y, e->z);
		glVertex3f(e->x, e->y, e->z);
	glEnd();
}

void glDrawBackRect(Point3D* b, Point3D* e)
{
	/*same as above with the back face*/
	glColor3f(0.2f, 0.2f, 0.2f);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glDepthFunc(GL_ALWAYS);
	glBegin(GL_LINE_LOOP);
	glVertex3f(e->x, e->y, e->z);
	glVertex3f(b->x, e->y, e->z);
	glVertex3f(b->x, b->y, e->z);
	glVertex3f(e->x, b->y, e->z);
	glEnd();
	//glDepthFunc(GL_LESS);
}

void glDrawEdgeFaces(Point3D* b, Point3D* e)
{
	/*same as above, with the side faces*/
	//glColor3f(0.4f, 0.4f, 0.4f);
	glColor3f(0.8f, 0.8f, 0.8f);
	float norm1[] = {0.0, 1.0, 0.0};
	glBegin(GL_POLYGON);
		glNormal3fv(norm1);
		glVertex3f(b->x, e->y, b->z);
		glVertex3f(b->x, e->y, e->z);
		glVertex3f(e->x, e->y, e->z);
		glVertex3f(e->x, e->y, b->z);
		glVertex3f(b->x, e->y, b->z);
	glEnd();
	float norm2[] = {1.0, 0.0, 0.0};
	glBegin(GL_POLYGON);
		glNormal3fv(norm2);
		glVertex3f(e->x, e->y, b->z);
		glVertex3f(e->x, e->y, e->z);
		glVertex3f(e->x, b->y, e->z);
		glVertex3f(e->x, b->y, b->z);
		glVertex3f(e->x, e->y, b->z);
	glEnd();
	float norm3[] = {0.0, -1.0, 0.0};
	glBegin(GL_POLYGON);
		glNormal3fv(norm3);
		glVertex3f(e->x, b->y, e->z);
		glVertex3f(b->x, b->y, e->z);
		glVertex3f(b->x, b->y, b->z);
		glVertex3f(e->x, b->y, b->z);
		glVertex3f(e->x, b->y, e->z);
	glEnd();

	float norm4[] = {-1.0, 0.0, 0.0};
	glBegin(GL_POLYGON);
		glNormal3fv(norm4);
		glVertex3f(b->x, b->y, b->z);
		glVertex3f(b->x, b->y, e->z);
		glVertex3f(b->x, e->y, e->z);
		glVertex3f(b->x, e->y, b->z);
		glVertex3f(b->x, b->y, b->z);
	glEnd();
}

void glDrawEdgeRects(Point3D* b, Point3D* e)
{
	/*same as above, with the side faces*/
	glColor3f(0.2f, 0.2f, 0.2f);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glDepthFunc(GL_ALWAYS);
	glBegin(GL_LINES);
	glVertex3f(b->x, b->y, b->z);
	glVertex3f(b->x, b->y, e->z);
	
	glVertex3f(e->x, e->y, b->z);
	glVertex3f(e->x, e->y, e->z);
	
	glVertex3f(e->x, b->y, b->z);
	glVertex3f(e->x, b->y, e->z);
	
	glVertex3f(b->x, e->y, e->z);
	glVertex3f(b->x, e->y, b->z);
	glEnd();
	//glDepthFunc(GL_LESS);
}


//void SurfelRenderer::DisplayOctTree(Node* n, OCT_DRAW_TYPE drwType)
//{
//	/*draw the tree of boxes, to gether with the merging lines*/
//	
//	vector3f b(-0.5,-0.5,-0.5), e(0.5,0.5,0.5);
//	//glDisable(GL_LIGHTING);
//	//glFrontFace(GL_CW);
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//	//front face
//	drawFrontFace(&b,&e);
//	drawFrontRect(&b,&e);
//	//back face
//	drawBackFace(&b,&e);
//	drawBackRect(&b,&e);
//	//edge faces
//	drawEdgeFaces(&b,&e);
//	drawEdgeRects(&b,&e);
//	//drawPoints(*b,*e,n->GetType2());
//	
//}
void Vector3fToPoint3D(vector3f *v, Point3D *p)
{
	p->x=v->x;
	p->y=v->y;
	p->z=v->z;
}


void Normalize(Point3D *p,Point3D *pCenter)
{
	*p = *p - *pCenter;
}

void DisplayCell(ProteinSignature *scene,Point3D *pCenter)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPolygonOffset(1.0,1.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	// draw center
	CMaterial material;
	//material = g_material[7];
	material.SetAmbient((float)100/255,(float)80/255,(float)0/255,0.5);
	material.SetDiffuse((float)100/255,(float)80/255,(float)0/255,0.5);
	material.SetSpecular(1.0, 0.0f, 0.0f, 0.5f);
	material.SetShininess(128.0f);
	material.SetEmission(0.0f,0.0f,0.0f,1.0f);


	::glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material.GetAmbient());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material.GetDiffuse());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material.GetSpecular());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material.GetEmission());
	::glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,material.GetShininess());

	float clr[4] = {1.0f, 0.0f, 0.f, 0.4f};
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LEQUAL);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, clr);

	Point3D *b=new Point3D();
	Point3D *e=new Point3D();

	vector3f *b1,*e1;

	vector<OCNode*>	*emptycell;

	emptycell = &scene->GetBoundaryCell();
		
	for (int i = 0; i < emptycell->size(); i++)
	{
		//these compensates for the fact that surfels are not just points, but a disk.
		b1 = ((*emptycell)[i]->GetBegin());
		e1 = ((*emptycell)[i]->GetEnd());

		Vector3fToPoint3D(b1,b);
		Vector3fToPoint3D(e1,e);

		Normalize(b,pCenter);
		Normalize(e,pCenter);
		//front face
		glDrawFrontFace(b,e);
		glDrawFrontRect(b,e);
		//back face
		glDrawBackFace(b,e);
		glDrawBackRect(b,e);
		//edge faces
		glDrawEdgeFaces(b,e);
		glDrawEdgeRects(b,e);
	}

	delete b;
	delete e;
}
