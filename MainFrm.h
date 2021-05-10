// MainFrm.h : interface of the CMainFrame class
//


#pragma once


#include "TrueColorToolBar.h"
#define TOOLBAR_BUTTON_WIDTH 40


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//--------------------------------------------------
	//add by ydm 2003-11-11
	CStatusBar& GetStatusBar()
	{ return m_wndStatusBar; }
	CProgressCtrl& GetProgressCtrl()
	{ return m_wndProgress; }
	CStatic& GetProgressText()
	{ return m_wndStatic; }
	//--------------------------------------------------

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	//--------------------------------------------------
	//add by ydm 2003-11-11
	CStatic			m_wndStatic;
	CProgressCtrl	m_wndProgress;
	//--------------------------------------------------

	CTrueColorToolBar    m_ToolBarDrawTop;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	//add by ydm 2003-11-11
	afx_msg LRESULT OnShowProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetProgressPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetProgressText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetStatusTextS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetStatusTextU(WPARAM wParam, LPARAM lParam);	

	DECLARE_MESSAGE_MAP()
};


