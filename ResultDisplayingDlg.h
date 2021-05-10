#if !defined(AFX_RESULTDISPLAYINGDLG_H__F180FFE0_79DB_4CAA_813A_90750BF5DEC2__INCLUDED_)
#define AFX_RESULTDISPLAYINGDLG_H__F180FFE0_79DB_4CAA_813A_90750BF5DEC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultDisplayingDlg.h : header file
//

#include "ximajpg.h"
#include "afxtempl.h"


/////////////////////////////////////////////////////////////////////////////
// ResultDisplayingDlg dialog

class ResultDisplayingDlg : public CDialog
{
// Construction
public:
	ResultDisplayingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ResultDisplayingDlg();

// Dialog Data
	//{{AFX_DATA(ResultDisplayingDlg)
	enum { IDD = IDD_DIALOG_DISPLAYRESULTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResultDisplayingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
    CArray<CString, CString> RetrievedResults; 
	CArray<int>				 RetrievedGroupID;
	int     PageNum, MousePos, PickedModel;
	CRect   region[15];
	CxImage *image[15];	
	BOOL    state, m_AlwaysDisp;

	void    Stopwatch(int start0stop1);
	BOOL    OpenImageFile( CString filename, int num );
	int     FindType(const CString& ext);
    CString FindExtension(const CString& name);
	void    SlideWindow();
	BOOL    SetTransparent(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	void    HideDlg(); 
	void    InvalidateImageRect(); 
	void    FreeMem();
	void    FreeImage();

protected:
	LARGE_INTEGER m_swFreq, m_swStart, m_swStop; 
	float m_etime; 

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ResultDisplayingDlg)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnActivate( WPARAM wParam, LPARAM lParam );
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPreviouspage();
	afx_msg void OnButtonNextpage();
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTDISPLAYINGDLG_H__F180FFE0_79DB_4CAA_813A_90750BF5DEC2__INCLUDED_)
