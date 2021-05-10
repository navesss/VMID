// VolumeVisualizerView.cpp : implementation of the CVolumeVisualizerView class
//

#include "stdafx.h"
#include "VolumeVisualizer.h"

#include "VolumeVisualizerDoc.h"
#include "VolumeVisualizerView.h"

#include <math.h>
#include "GLVisualizer.h"
#include "_global.h"
#include "_common.h"

#include "DlgSample.h"
#include "ProteinSignature.h"

#include "ResultDisplayingDlg.h"
#include "ShapeRetrieve.h"
#include "FileManager.h"
#include "SignatureRender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVolumeVisualizerView

IMPLEMENT_DYNCREATE(CVolumeVisualizerView, CView)

BEGIN_MESSAGE_MAP(CVolumeVisualizerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_GL_POINTS, &CVolumeVisualizerView::OnGlPoints)
	ON_COMMAND(ID_GL_POINTS2, &CVolumeVisualizerView::OnGlPoints2)
	ON_COMMAND(ID_GL_SURFACE, &CVolumeVisualizerView::OnGlSurface)
	ON_COMMAND(ID_GL_FULLPOINTS, &CVolumeVisualizerView::OnGlFullpoints)
	ON_COMMAND(ID_GL_CELL, &CVolumeVisualizerView::OnGlCell)
	ON_COMMAND(ID_GL_SAMPLEPOINTS, &CVolumeVisualizerView::OnGlSamplepoints)
	ON_COMMAND(ID_GL_CELL2, &CVolumeVisualizerView::OnGlCell2)
	ON_COMMAND(ID_GL_VISIBILITY, &CVolumeVisualizerView::OnGlVisibility)
	ON_COMMAND(ID_GL_VISIBILITY2, &CVolumeVisualizerView::OnGlVisibility2)
	ON_COMMAND(ID_GL_MRC, &CVolumeVisualizerView::OnGlMRC)
	ON_COMMAND(ID_ID_SAMPLE, &CVolumeVisualizerView::OnIDSample)
	ON_COMMAND(ID_ID_VISIBILITY, &CVolumeVisualizerView::OnIdVisibility)
	ON_COMMAND(ID_ID_EXPORT, &CVolumeVisualizerView::OnIdExport)
	ON_COMMAND(ID_ID_ED, &CVolumeVisualizerView::OnIdEd)
	ON_COMMAND(ID_GL_TEXT, &CVolumeVisualizerView::OnGlText)
	ON_COMMAND(RETRIEVE_ID, &CVolumeVisualizerView::OnRetrieveID)
	ON_COMMAND(RETRIEVE_ED, &CVolumeVisualizerView::OnRetrieveED)
	ON_COMMAND(RETRIEVE_GD, &CVolumeVisualizerView::OnRetrieveGD)
	ON_COMMAND(ID_INFOR_ID_PRC, &CVolumeVisualizerView::OnInforIdPrc)
	ON_COMMAND(ID_INFOR_WEB, &CVolumeVisualizerView::OnInforWeb)

	ON_MESSAGE(WM_USER_DISPICKEDMODEL, OnDispSeletedModel)
	ON_COMMAND(ID_DISPLAY_SHOWOCTREE, &CVolumeVisualizerView::OnDisplayShowoctree)
END_MESSAGE_MAP()

// CVolumeVisualizerView construction/destruction

CVolumeVisualizerView::CVolumeVisualizerView()
{
	init();

	m_ShapeRetrieve = NULL;

	m_DispDlg = new ResultDisplayingDlg;;
	if( m_DispDlg->GetSafeHwnd() )  m_DispDlg->DestroyWindow();
	m_DispDlg->Create( IDD_DIALOG_DISPLAYRESULTS, NULL );
	m_DispDlg->ShowWindow(SW_HIDE);	
}

CVolumeVisualizerView::~CVolumeVisualizerView()
{
	if (m_DispDlg) 
	{
		m_DispDlg->DestroyWindow();
		m_DispDlg = NULL;
	}

	if(m_ShapeRetrieve)
	{
		delete m_ShapeRetrieve;
		m_ShapeRetrieve = NULL;
	}
}

void CVolumeVisualizerView::init()
{
	eye[0] = 26;
	eye[1] = -48;
	eye[2] = 40;

	center[0] = 0.0;
	center[1] = 0.0;
	center[2] = 0.0;

	Rx = 0;
	Ry = -3; //-157; //60; //-33; //76;
	Rz = 122; //116; //29; //242; //23;

	/*
	Rx = 0;
	Ry = -45;
	Rz = 134;
	*/

	vx[0] = 0;
	vx[1] = 0;
	vx[2] = 1;

	vy[0] = -(float)(eye[1]/sqrt(eye[0]*eye[0]+eye[1]*eye[1]));
	vy[1] = (float)(eye[0]/sqrt(eye[0]*eye[0]+eye[1]*eye[1]));
	vy[2] = 0;

	vz[0] = -vy[1];
	vz[1] = vy[0];
	vz[2] = 0;

	view_angle = 48.0f;
	//view_angle = 8.65;

	//view_angle = 22.5;
	//view_angle = 1.05;

	isLPressed = FALSE;
	isRPressed = FALSE;

	shift_x = 0;
	shift_y = 0;
	shift_z = 0;
}

void CVolumeVisualizerView::initGL()
{
	GLfloat light_position1[4] = {-52, -16, -50, 0};
	GLfloat light_position2[4] = {-26, -48, -50, 0};
	GLfloat light_position3[4] = { 16, -52, -50, 0};

	GLfloat direction1[3] = {52,16,50};
	GLfloat direction2[3] = {26,48,50};
	GLfloat direction3[3] = {-16,52,50};

	GLfloat light_position4[4] = {52, 16, 50, 0};
	GLfloat light_position5[4] = {26, 48, 50, 0};
	GLfloat light_position6[4] = {-16, 52, 50, 0};

	GLfloat direction4[3] = {-52,-16,-50};
	GLfloat direction5[3] = {-26,-48,-50};
	GLfloat direction6[3] = {16,-52,-50};

	GLfloat color1[4] = {1,0,0,1};
	GLfloat color2[4] = {0,1,0,1};
	GLfloat color3[4] = {0,0,1,1};

	GLfloat color4[4] = {1,0,0,1};
	GLfloat color5[4] = {0,1,0,1};
	GLfloat color6[4] = {0,0,1,1};

	/*
	GLfloat color1[4] = {0.7,0.7,0.7,0.7};
	GLfloat color2[4] = {0.7,0.7,0.7,0.7};
	GLfloat color3[4] = {0.7,0.7,0.7,0.7};

	GLfloat color4[4] = {0.7,0.7,0.7,0.7};
	GLfloat color5[4] = {0.7,0.7,0.7,0.7};
	GLfloat color6[4] = {0.7,0.7,0.7,0.7};
	*/



	GLfloat ambient[4] = {0.3f,0.3f,0.3f,0.5f};

	GLfloat material_color[4] = {1,1,1,0.3f};
	GLfloat material_specular[4] = {0.5,0.5,0.5,0.5};
	GLfloat material_ambient[4] = {0.0,0.0,0.0,0.0};

	glClearColor(1,1,1,0);
	//glClearColor(0,0,0,0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color1);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, color2);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, color3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, color3);

	glLightfv(GL_LIGHT3, GL_POSITION, light_position4);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction4);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, color4);
	glLightfv(GL_LIGHT3, GL_SPECULAR, color4);

	glLightfv(GL_LIGHT4, GL_POSITION, light_position5);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direction5);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, color5);
	glLightfv(GL_LIGHT4, GL_SPECULAR, color5);

	glLightfv(GL_LIGHT5, GL_POSITION, light_position6);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, direction6);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, color6);
	glLightfv(GL_LIGHT5, GL_SPECULAR, color6);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	glEnable(GL_LIGHTING);

	/*
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	*/

	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glLineWidth(0.5);
	//glPointSize(1.0f);
	glPointSize(2.0);
	//glPointSize(10.0);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void CVolumeVisualizerView::initGL2()
{
	GLfloat light_position1[4] = {-52, -16, -50, 0};
	GLfloat light_position2[4] = {-26, -48, -50, 0};
	GLfloat light_position3[4] = { 16, -52, -50, 0};

	GLfloat direction1[3] = {52,16,50};
	GLfloat direction2[3] = {26,48,50};
	GLfloat direction3[3] = {-16,52,50};

	GLfloat light_position4[4] = {52, 16, 50, 0};
	GLfloat light_position5[4] = {26, 48, 50, 0};
	GLfloat light_position6[4] = {-16, 52, 50, 0};

	GLfloat direction4[3] = {-52,-16,-50};
	GLfloat direction5[3] = {-26,-48,-50};
	GLfloat direction6[3] = {16,-52,-50};

	//	GLfloat color1[4] = {1,0,0,1};
	//	GLfloat color2[4] = {0,1,0,1};
	//	GLfloat color3[4] = {0,0,1,1};
	//
	//	GLfloat color4[4] = {1,0,0,1};
	//	GLfloat color5[4] = {0,1,0,1};
	//	GLfloat color6[4] = {0,0,1,1};


	//	GLfloat color1[4] = {0.7,0.7,0.7,0.7};
	//	GLfloat color2[4] = {0.7,0.7,0.7,0.7};
	//	GLfloat color3[4] = {0.7,0.7,0.7,0.7};
	//
	//	GLfloat color4[4] = {0.7,0.7,0.7,0.7};
	//	GLfloat color5[4] = {0.7,0.7,0.7,0.7};
	//	GLfloat color6[4] = {0.7,0.7,0.7,0.7};

	// revised by liuyushen 2004.10.08
	GLfloat color1[4] = {0.38,0.38,0.38,0.38};
	GLfloat color2[4] = {0.38,0.38,0.38,0.38};
	GLfloat color3[4] = {0.38,0.38,0.38,0.38};

	GLfloat color4[4] = {0.38,0.38,0.38,0.38};
	GLfloat color5[4] = {0.38,0.38,0.38,0.38};
	GLfloat color6[4] = {0.38,0.38,0.38,0.38};



	//	GLfloat ambient[4] = {0.3f,0.3f,0.3f,0.5f};
	//
	//	GLfloat material_color[4] = {1,1,1,0.3f};
	//	GLfloat material_specular[4] = {0.5,0.5,0.5,0.5};
	//	GLfloat material_ambient[4] = {0.0,0.0,0.0,0.0};

	// revised by liuyushen 2004.10.08
	GLfloat ambient[4] = {0.3f,0.3f,0.3f,0.5f};

	GLfloat material_color[4] = {1,1,1,0.3f};
	GLfloat material_specular[4] = {0.5,0.5,0.5,0.5};
	GLfloat material_ambient[4] = {0.0,0.0,0.0,0.0};


	glClearColor(1,1,1,0);
	//glClearColor(0,0,0,0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color1);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, color2);

	glLightfv(GL_LIGHT2, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, color3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, color3);

	glLightfv(GL_LIGHT3, GL_POSITION, light_position4);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction4);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, color4);
	glLightfv(GL_LIGHT3, GL_SPECULAR, color4);

	glLightfv(GL_LIGHT4, GL_POSITION, light_position5);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direction5);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, color5);
	glLightfv(GL_LIGHT4, GL_SPECULAR, color5);

	glLightfv(GL_LIGHT5, GL_POSITION, light_position6);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, direction6);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, color6);
	glLightfv(GL_LIGHT5, GL_SPECULAR, color6);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	glEnable(GL_LIGHTING);

	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);


	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);
	glEnable(GL_LIGHT5);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glLineWidth(0.5);
	//glPointSize(1.0f);
	glPointSize(2.0);
	//glPointSize(10.0);

	glPolygonOffset(1.0,2.0f);
	glEnable(GL_POLYGON_OFFSET_FILL);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

BOOL CVolumeVisualizerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVolumeVisualizerView drawing

void CVolumeVisualizerView::OnDraw(CDC* pDC)
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	wglMakeCurrent(pDC->m_hDC, m_hRC);

	float r,g,b;
	// 以白色为背景
	//	r = 189.0f/255.0f;
	//	g = 222.0f/255.0f;
	//	b = 1.0f;
	r = g = b = 1.0f;
	glClearColor(r, g, b, 1.0f);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(shift_x,shift_y,shift_z);
	//glTranslatef(0,0,10);
	glRotatef(Rx,vz[0],vz[1],vz[2]);
	glRotatef(Ry,vy[0],vy[1],vy[2]);
	glRotatef(Rz,vx[0],vx[1],vx[2]);

	//view_angle = 62.600010;
	//Rx = 0; Ry = 162.00000; Rz = -97.000000;
	//vx[0]=0.000000; vx[1]=0.000000; vx[2]=1.000000;
	//vy[0]=0.87929195; vy[1]=0.476283; vy[2]=0.000000;
	//vz[0]=-0.47628316; vz[1]=0.87929195; vz[2]=0.000000;

	RenderScene();

	glPopMatrix();

	glFlush();
	SwapBuffers(wglGetCurrentDC());
	wglMakeCurrent(pDC->m_hDC, NULL);

	
	if(pDoc->m_drawText)
		DrawText(pDC);
}

void CVolumeVisualizerView::RenderScene()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	
	GLVisualizer *vis = pDoc->m_visualizer;
	
	if(vis)
	{
		int drawMode = pDoc->m_drawMode;
		vis->Draw(drawMode);

		if(pDoc->m_drawSamplePoints)
			vis->drawSamplePoints();

		if(pDoc->m_drawIDvisibility)
			vis->drawVolumeIDVisibility(1);
		if(pDoc->m_drawIDvisibility2)
			vis->drawVolumeIDVisibility(2);

		if(pDoc->m_drawOctree)
			DisplayCell(pDoc->m_signature,&vis->m_center);
	}
	
}

void CVolumeVisualizerView::DrawText(CDC* pDC)
{
	CVolumeVisualizerDoc* pDoc = GetDocument();

	CString str;
	int r,g,b;
	//r = 189; g = 222; b = 255;
	r = g = b = 255;

	Volume * vol = pDoc->m_vol;
	if(vol == NULL)
		return;
	
	ProteinSignature *sig = pDoc->m_signature;
	if(sig == NULL)
		return;
	int n_boundaryPts = sig->m_borderPts.size();
	int n_allPts = sig->m_borderPts.size() + sig->m_inPts.size(); 

	pDC->SetBkColor(RGB(r,g,b));

	float threshold = pDoc->m_threshold;
	int rx, ry, rz;
	rx = vol->getSizeX(); ry = vol->getSizeY(); rz = vol->getSizeZ();

	if(vol != NULL)
	{
		pDC->SetTextColor(RGB(255,0,0));

		char nv[256];

		sprintf(nv, "Filename: %s", pDoc->m_filename.c_str());
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 8, str);

		sprintf(nv, "Volume size: %d * %d * %d", rx, ry, rz);
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 25, str);

		sprintf(nv, "Threshold: %f", threshold);	
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 42, str);

		sprintf(nv, "Boundary Points: %d", n_boundaryPts);	
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 59, str);

		sprintf(nv, "Object Points: %d", n_allPts);	
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 76, str);

		int group_id = pDoc->m_group_id;
		if(group_id != -1)
		{
			sprintf(nv, "Retrived Group: %d", group_id);	
			str = (LPCTSTR)nv;
			pDC->TextOut(8, 79, str);
		}

		int depth;
		if(pDoc->m_signature->m_octtree==NULL)
			depth=0;
		else
			depth=pDoc->m_depth;

		sprintf(nv, "Octree Depth: %d", depth);	
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 93, str);


		double costtime=0;
		if(pDoc->m_signature!=NULL)
		{
			costtime=pDoc->m_signature->m_CostByTree;
		}
		sprintf(nv, "Octree Time: %f(S)", costtime);	
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 110, str);


		costtime=0;
		if(pDoc->m_signature!=NULL)
		{
			costtime=pDoc->m_signature->m_CostByCheckVisibility;
		}
		sprintf(nv, "Visibility Time: %f(S)", costtime);	
		str = (LPCTSTR)nv;
		pDC->TextOut(8, 127, str);
	}
}

// CVolumeVisualizerView diagnostics

#ifdef _DEBUG
void CVolumeVisualizerView::AssertValid() const
{
	CView::AssertValid();
}

void CVolumeVisualizerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVolumeVisualizerDoc* CVolumeVisualizerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVolumeVisualizerDoc)));
	return (CVolumeVisualizerDoc*)m_pDocument;
}
#endif //_DEBUG


// CVolumeVisualizerView message handlers

void CVolumeVisualizerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	int i;
	GLdouble up[3];
	GLdouble vector[3];
	GLdouble norm;

	CClientDC clientDC(this);

	wglMakeCurrent(clientDC.m_hDC, m_hRC);

	/////////////////////////////////////////////////////////////////  
	//glViewport(0, 0, cx, cy);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(view_angle, (GLfloat)cx/(GLfloat)cy, 10.0, 4000.0);	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	/////////////////////////////////////////////////////////////////  
	glViewport(0,0,cx,cy);  
	GLdouble   aspect_ratio =   (GLdouble)cx/(GLdouble)cy;  
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	GLdouble   nRange=view_angle;  
	if   (cx   <=   cy)  
		glOrtho   (-nRange,   nRange,   -nRange*aspect_ratio,   nRange*aspect_ratio,   -20*nRange,   20*nRange);  
	else  
		glOrtho   (-nRange*aspect_ratio,   nRange*aspect_ratio,   -nRange,   nRange,   -20*nRange,   20*nRange);  
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();

	/////////////////////////////////////////////////////////////////  

	for(i=0; i<3; i++)
		vector[i] = center[i] - eye[i];

	up[0] =  vector[0] * vector[2];
	up[1] =  vector[1] * vector[2];
	up[2] =  vector[0] * vector[0] + vector[1] * vector[1];

	norm = up[0] * up[0] + up[1] * up[1] + up[2] * up[2];
	norm = sqrt(norm);
	for(i=0; i<3; i++)
		up[i] = up[i] / norm;

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2],
		up[0], up[1], up[2]);

	wglMakeCurrent(clientDC.m_hDC, NULL);	
}

void CVolumeVisualizerView::OnDestroy()
{
	CView::OnDestroy();

	wglDeleteContext(m_hRC);	

}

int CVolumeVisualizerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		32,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	CClientDC  clientDC(this);

	int pixelFormat = ChoosePixelFormat(clientDC.m_hDC, &pfd);
	if(!SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd))
		return -1;

	m_hRC = wglCreateContext(clientDC.m_hDC);

	wglMakeCurrent(clientDC.m_hDC,m_hRC);
	//initGL();
	initGL2();
	wglMakeCurrent(clientDC.m_hDC,NULL);

	return 0;
}

void CVolumeVisualizerView::OnMouseMove(UINT nFlags, CPoint point)
{
	int nx = point.x;
	int ny = point.y;

	// TODO: Add your message handler code here and/or call default
	if(isLPressed  && isRPressed)
	{
		shift_z -= (float)(nx - px)/30;
		shift_y += (float)(ny - py)/30;
		px = nx;
		py = ny;
		CClientDC* dc = new CClientDC(this); 
		OnDraw(dc);
		delete dc;
	}
	else
	{
		if(isLPressed){

			//		Rz += (nx - px)/3;
			//		Ry += (ny - py)/3;
			Rz += (nx - px)/3;
			Ry += (ny - py)/3;
			px = nx;
			py = ny;
			CClientDC* dc = new CClientDC(this); 
			OnDraw(dc);
			delete dc;
		}
		else if(isRPressed){

			view_angle += (ny - py)/20.0f;
			//view_angle += (ny - py)/280.0f;
			px = nx;
			py = ny;
			CClientDC* dc = new CClientDC(this);

			RECT* rect = new RECT;
			GetClientRect(rect);
			OnSize(SIZE_RESTORED, rect->right, rect->bottom);
			delete rect;
			OnDraw(dc);
			delete dc;
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CVolumeVisualizerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isLPressed = TRUE;
	px = point.x;
	py = point.y;

	CView::OnLButtonDown(nFlags, point);
}

void CVolumeVisualizerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isLPressed = FALSE;

	CView::OnLButtonUp(nFlags, point);
}

void CVolumeVisualizerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isRPressed = true;
	px = point.x;
	py = point.y;

	CView::OnRButtonDown(nFlags, point);
}

void CVolumeVisualizerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isRPressed = false;

	CView::OnRButtonUp(nFlags, point);
}

void CVolumeVisualizerView::OnGlPoints()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawMode = DRAW_VOLUME_BORDER_POINTS;
	
	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlPoints2()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawMode = DRAW_VOLUME_BORDER_POINTS2;

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlFullpoints()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawMode = DRAW_VOLUME_FULL_POINTS;

	InvalidateRect(NULL, FALSE);
}


void CVolumeVisualizerView::OnGlMRC()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawMode = DRAW_VOLUME_MRC;

	InvalidateRect(NULL, FALSE);}

void CVolumeVisualizerView::OnGlCell()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawMode = DRAW_VOLUME_CELL;

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlCell2()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawMode = DRAW_VOLUME_CELL2;

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlSurface()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawMode = DRAW_VOLUME_SURFACE;
	
	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlSamplepoints()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawSamplePoints = !pDoc->m_drawSamplePoints;

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlVisibility()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawIDvisibility = !pDoc->m_drawIDvisibility;

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlVisibility2()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawIDvisibility2 = !pDoc->m_drawIDvisibility2;

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnGlText()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawText = !pDoc->m_drawText;
	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnIDSample()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();

	GLVisualizer *visualizer = pDoc->m_visualizer;

	if(visualizer == NULL)
		return;

	CDlgSample dlg;

	if(dlg.DoModal() == IDOK)
	{
		int sample_num = dlg.m_sampleNumber;
		int sample_method = dlg.m_sampleMethod;
		
		BeginWaitCursor();

		visualizer->buildSamplePoints(sample_num, sample_method);
		pDoc->m_drawSamplePoints = true;

		InvalidateRect(NULL, FALSE);
		EndWaitCursor();
	}
}

void CVolumeVisualizerView::OnIdEd()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();

	GLVisualizer *visualizer = pDoc->m_visualizer;

	if(visualizer == NULL)
		return;

	CDlgSample dlg;

	if(dlg.DoModal() == IDOK)
	{
		int sample_num = dlg.m_sampleNumber;
		int sample_method = dlg.m_sampleMethod;

		BeginWaitCursor();

		visualizer->buildEuclideanDistance(sample_num, sample_method);
		pDoc->m_drawSamplePoints = true;
		pDoc->m_drawIDvisibility = true;
		pDoc->m_drawIDvisibility2 = false;

		InvalidateRect(NULL, FALSE);
		EndWaitCursor();
	}
}

void CVolumeVisualizerView::OnIdVisibility()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();

	GLVisualizer *visualizer = pDoc->m_visualizer;

	if(visualizer == NULL)
		return;

	CDlgSample dlg;

	if(dlg.DoModal() == IDOK)
	{
		int sample_num = dlg.m_sampleNumber;
		int sample_method = dlg.m_sampleMethod;

		pDoc->m_signature->m_accuracy=dlg.m_accuracy;

		BeginWaitCursor();

		visualizer->buildIDVisibility(sample_num, sample_method);
		pDoc->m_drawSamplePoints = true;
		pDoc->m_drawIDvisibility = false;
		pDoc->m_drawIDvisibility2 = true;

		InvalidateRect(NULL, FALSE);
		EndWaitCursor();
	}
}


static char file_export[] = "Matrix distance files(*.txt)|*.txt|";

void CVolumeVisualizerView::OnIdExport()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();

	GLVisualizer *visualizer = pDoc->m_visualizer;
	if(visualizer == NULL)
		return;

	GLVolume *glVolume = visualizer->m_volGLVolume;
	if(glVolume == NULL)
		return;

	CFileDialog dia(FALSE, "*.txt", "1.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		file_export, NULL);

	if(dia.DoModal() != IDOK){
		return;
	}

	CString name = dia.GetPathName();
	// Extension
	CString ext = name;
	int pos = ext.ReverseFind('.');
	pos = ext.GetLength() - pos;
	ext = ext.Right(pos);
	ext.MakeLower();

	// Path "c:\path\file.wrl" -> c:\path
	CString path = name;
	path = path.Left(path.ReverseFind('\\'));
	SetCurrentDirectory(path);

	BeginWaitCursor();

	if(ext == ".txt")
	{
		glVolume->m_matrixID->toFile(name.GetBuffer());
	}
	else
		return;

	EndWaitCursor();
}




void CVolumeVisualizerView::OnRetrieveID()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;
	
	if(m_ShapeRetrieve)
	{
		delete m_ShapeRetrieve;
		m_ShapeRetrieve = NULL;
	}

	m_ShapeRetrieve = new CShapeRetrieve(pDoc);

	int method = 1;
	m_ShapeRetrieve->DoRetrieve(method);

	
	CArray<CString, CString>	RetrievedModels; 
	CArray<int>					RetrievedGroupID;
	int							i, nb, id;

	nb = m_ShapeRetrieve->m_RetrievedModels.size();

	for(i=0; i<nb; i++)
	{
		string name = m_ShapeRetrieve->m_RetrievedModels[i].m_dir_name;
		RetrievedModels.Add(CString(name.c_str()));

		id = m_ShapeRetrieve->m_RetrievedModels[i].m_group_id;
		RetrievedGroupID.Add(id);
	}


	m_DispDlg->RetrievedResults.SetSize(0);
	m_DispDlg->RetrievedResults.FreeExtra();
	m_DispDlg->RetrievedResults.RemoveAll();

	m_DispDlg->RetrievedResults.Copy(RetrievedModels);
	m_DispDlg->PageNum = 0;

	m_DispDlg->RetrievedGroupID.SetSize(0);
	m_DispDlg->RetrievedGroupID.FreeExtra();
	m_DispDlg->RetrievedGroupID.RemoveAll();
	m_DispDlg->RetrievedGroupID.Copy(RetrievedGroupID);

	if ( RetrievedModels.GetSize() > 0 )
		m_DispDlg->SlideWindow();

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnRetrieveED()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;

	if(m_ShapeRetrieve)
	{
		delete m_ShapeRetrieve;
		m_ShapeRetrieve = NULL;
	}

	m_ShapeRetrieve = new CShapeRetrieve(pDoc);

	int method = 2;
	m_ShapeRetrieve->DoRetrieve(method);


	CArray<CString, CString>	RetrievedModels; 
	CArray<int>					RetrievedGroupID;
	int							i, nb, id;

	nb = m_ShapeRetrieve->m_RetrievedModels.size();

	for(i=0; i<nb; i++)
	{
		string name = m_ShapeRetrieve->m_RetrievedModels[i].m_dir_name;
		RetrievedModels.Add(CString(name.c_str()));

		id = m_ShapeRetrieve->m_RetrievedModels[i].m_group_id;
		RetrievedGroupID.Add(id);
	}


	m_DispDlg->RetrievedResults.SetSize(0);
	m_DispDlg->RetrievedResults.FreeExtra();
	m_DispDlg->RetrievedResults.RemoveAll();

	m_DispDlg->RetrievedResults.Copy(RetrievedModels);
	m_DispDlg->PageNum = 0;

	m_DispDlg->RetrievedGroupID.SetSize(0);
	m_DispDlg->RetrievedGroupID.FreeExtra();
	m_DispDlg->RetrievedGroupID.RemoveAll();
	m_DispDlg->RetrievedGroupID.Copy(RetrievedGroupID);

	if ( RetrievedModels.GetSize() > 0 )
		m_DispDlg->SlideWindow();

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnRetrieveGD()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;

	if(m_ShapeRetrieve)
	{
		delete m_ShapeRetrieve;
		m_ShapeRetrieve = NULL;
	}

	m_ShapeRetrieve = new CShapeRetrieve(pDoc);

	int method = 3;
	m_ShapeRetrieve->DoRetrieve(method);


	CArray<CString, CString>	RetrievedModels; 
	CArray<int>					RetrievedGroupID;
	int							i, nb, id;

	nb = m_ShapeRetrieve->m_RetrievedModels.size();

	for(i=0; i<nb; i++)
	{
		string name = m_ShapeRetrieve->m_RetrievedModels[i].m_dir_name;
		RetrievedModels.Add(CString(name.c_str()));

		id = m_ShapeRetrieve->m_RetrievedModels[i].m_group_id;
		RetrievedGroupID.Add(id);
	}


	m_DispDlg->RetrievedResults.SetSize(0);
	m_DispDlg->RetrievedResults.FreeExtra();
	m_DispDlg->RetrievedResults.RemoveAll();

	m_DispDlg->RetrievedResults.Copy(RetrievedModels);
	m_DispDlg->PageNum = 0;

	m_DispDlg->RetrievedGroupID.SetSize(0);
	m_DispDlg->RetrievedGroupID.FreeExtra();
	m_DispDlg->RetrievedGroupID.RemoveAll();
	m_DispDlg->RetrievedGroupID.Copy(RetrievedGroupID);

	if ( RetrievedModels.GetSize() > 0 )
		m_DispDlg->SlideWindow();

	InvalidateRect(NULL, FALSE);
}
void CVolumeVisualizerView::OnInforIdPrc()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	if(pDoc == NULL)
		return;

	CFileDialog dia(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Index files(*.idx)|*.idx||", NULL);

	if(dia.DoModal() != IDOK){
		return;
	}

	CString name = dia.GetPathName();

	if(m_ShapeRetrieve)
	{
		delete m_ShapeRetrieve;
		m_ShapeRetrieve = NULL;
	}

	m_ShapeRetrieve = new CShapeRetrieve(pDoc);

	BeginWaitCursor();
	m_ShapeRetrieve->get_PRCurves_data(string(name.GetBuffer()));
	EndWaitCursor();

	if(m_ShapeRetrieve)
	{
		delete m_ShapeRetrieve;
		m_ShapeRetrieve = NULL;
	}

	InvalidateRect(NULL, FALSE);
}

void CVolumeVisualizerView::OnInforWeb()
{
	CVolumeVisualizerDoc* pDoc = GetDocument();
	std::string link(pDoc->m_filename);
	std::string link2;
	CString str;
	int pos = link.find_last_of('-');

	link2=link.substr(0,pos);
	char nv[256];
	sprintf(nv, "http://www.molmovdb.org/cgi-bin/morph.cgi?ID=%s", link2.c_str());
	str = (LPCTSTR)nv;
	ShellExecute(GetSafeHwnd(),   NULL,   str,   NULL,   NULL,   SW_SHOWNORMAL) ;
}

LRESULT CVolumeVisualizerView::OnDispSeletedModel(WPARAM wParam, LPARAM lParam)
{
	if ( m_DispDlg->PickedModel < 0 ) return 1;

	string name = m_ShapeRetrieve->m_RetrievedModels[m_DispDlg->PickedModel].m_dir_name;

	string ext = get_file_extension_name(name);
	
	if(ext != "mrc" && ext != "sof")
	{
		string name2 = get_file_name(name);
		name2 = get_file_name2(name2);
		name2 = name2 + ".mrc";
		string dir = get_file_direction(name);
		name = dir + name2;
	}
	CString fileName = CString(name.c_str());

	CVolumeVisualizerDoc* pDoc = GetDocument();

	pDoc->FileOpen(fileName);

	OnRetrieveID();
	
	return 0;
}
void CVolumeVisualizerView::OnDisplayShowoctree()
{
	// TODO: 在此添加命令处理程序代码
	CVolumeVisualizerDoc* pDoc = GetDocument();
	pDoc->m_drawOctree=!pDoc->m_drawOctree;
	InvalidateRect(NULL, FALSE);
}
