// VolumeVisualizerView.h : interface of the CVolumeVisualizerView class
//


#pragma once

class ResultDisplayingDlg;
class CShapeRetrieve;

class CVolumeVisualizerView : public CView
{
protected: // create from serialization only
	CVolumeVisualizerView();
	DECLARE_DYNCREATE(CVolumeVisualizerView)

// Attributes
public:
	CVolumeVisualizerDoc* GetDocument() const;

// Operations
public:
	void init();
	void initGL();
	void initGL2();

	void RenderScene();
	void DrawText(CDC* pDC);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CVolumeVisualizerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HGLRC m_hRC;
	GLdouble eye[3], center[3];
	GLfloat Rx,Ry,Rz;
	int px, py;
	float vx[3], vy[3], vz[3];
	BOOL isLPressed, isRPressed;
	float view_angle;
	float shift_x, shift_y, shift_z;

	ResultDisplayingDlg		*m_DispDlg;
	CShapeRetrieve			*m_ShapeRetrieve;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnGlPoints();
public:
	afx_msg void OnGlPoints2();
public:
	afx_msg void OnGlSurface();
public:
	afx_msg void OnGlFullpoints();
public:
	afx_msg void OnGlCell();
public:
	afx_msg void OnIDSample();
public:
	afx_msg void OnGlSamplepoints();
public:
	afx_msg void OnGlCell2();
public:
	afx_msg void OnIdVisibility();
public:
	afx_msg void OnGlVisibility();
public:
	afx_msg void OnGlVisibility2();
public:
	afx_msg void OnGlMRC();
public:
	afx_msg void OnIdExport();
public:
	afx_msg void OnIdEd();
public:
	afx_msg void OnGlText();
public:
	afx_msg void OnRetrieveID();
	afx_msg void OnRetrieveED();
	afx_msg void OnRetrieveGD();
public:
	afx_msg void OnInforIdPrc();
	afx_msg void OnInforWeb();
public:
	afx_msg LRESULT OnDispSeletedModel(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnDisplayShowoctree();
};

#ifndef _DEBUG  // debug version in VolumeVisualizerView.cpp
inline CVolumeVisualizerDoc* CVolumeVisualizerView::GetDocument() const
   { return reinterpret_cast<CVolumeVisualizerDoc*>(m_pDocument); }
#endif

