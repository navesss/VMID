#pragma once


// CoctreeDlg dialog

class CoctreeDlg : public CDialog
{
	DECLARE_DYNAMIC(CoctreeDlg)

public:
	CoctreeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CoctreeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_depth;
public:
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	//afx_msg void OnEnChangeEdit1();
};
