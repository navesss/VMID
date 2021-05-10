// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VolumeVisualizer.h"

#include "MainFrm.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_G_SHOWPROGRESS, OnShowProgress)
	ON_MESSAGE(WM_G_SETPROGRESSPOS, OnSetProgressPos)
	ON_MESSAGE(WM_G_SETPROGRESSTEXT, OnSetProgressText)
	ON_MESSAGE(WM_G_SETSTATUSTEXTS, OnSetStatusTextS)
	ON_MESSAGE(WM_G_SETSTATUSTEXTU, OnSetStatusTextU)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//--------------------------------------------------------------
	// add by ydm 2003-11-11 Add Progress Bar
	CRect rect;
	rect.SetRect(5, 2, 205, 18);
	m_wndProgress.Create(0, rect, &m_wndStatusBar, 1);
	rect.SetRect(210, 2, 410, 22);
	m_wndStatic.Create(0, SS_LEFT, rect, &m_wndStatusBar);
	m_wndStatic.SetFont(m_wndStatusBar.GetFont(), FALSE);
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	// load true cool tool bar
	m_ToolBarDrawTop.SetBorders(1, 1, 1, 1);
	if (!m_ToolBarDrawTop.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBarDrawTop.LoadToolBar(IDR_TOOLBAR_TOOL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_ToolBarDrawTop.ModifyStyle(0, TBSTYLE_FLAT);

	m_ToolBarDrawTop.LoadTrueColorToolBar(TOOLBAR_BUTTON_WIDTH,
		IDB_TOP,
		IDB_TOPHOT);
	//--------------------------------------------------------------


	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_ToolBarDrawTop.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	DockControlBar(&m_ToolBarDrawTop);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


LRESULT CMainFrame::OnShowProgress(WPARAM show, LPARAM total)
{
	static UINT nID, nStyle;
	static int cxWidth;

	CProgressCtrl& pc = GetProgressCtrl();
	CStatic& sc = GetProgressText();

	if (show)
	{
		m_wndStatusBar.GetPaneInfo(1, nID, nStyle, cxWidth);
		m_wndStatusBar.SetPaneInfo(1, nID, nStyle, 5);

		SetStatusText("");

		pc.ShowWindow(SW_SHOW);
		sc.ShowWindow(SW_SHOW);

		pc.SetRange(0, (short)total);
	}
	else
	{
		m_wndStatusBar.SetPaneInfo(1, nID, nStyle, cxWidth);

		pc.ShowWindow(SW_HIDE);
		sc.ShowWindow(SW_HIDE);
	}
	return 0;
}

LRESULT CMainFrame::OnSetProgressPos(WPARAM pos, LPARAM)
{
	CProgressCtrl& pc = GetProgressCtrl();
	pc.SetPos(pos);
	return 0;
}

LRESULT CMainFrame::OnSetProgressText(WPARAM text, LPARAM)
{
	CStatic& sc = GetProgressText();
	sc.SetWindowText((LPCSTR)text);
	return 0;
}

LRESULT CMainFrame::OnSetStatusTextS(WPARAM text, LPARAM pane)
{
	CWnd* pMessageBar = GetMessageBar();
	if (pMessageBar)
		((CStatusBar *)pMessageBar)->SetPaneText(pane, (LPCSTR)text);
	return 0;
}

LRESULT CMainFrame::OnSetStatusTextU(WPARAM textid, LPARAM pane)
{
	CString buffer;
	buffer.LoadString(textid);
	OnSetStatusTextS((WPARAM)(LPCSTR)buffer, pane);
	return 0;
}

// CMainFrame message handlers



