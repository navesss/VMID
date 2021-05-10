#pragma once


// CDlgSample dialog

class CDlgSample : public CDialog
{
	DECLARE_DYNAMIC(CDlgSample)

public:
	CDlgSample(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSample();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_SAMPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	int m_sampleNumber;
	int m_sampleMethod;
public:
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	float m_accuracy;
public:
	afx_msg void OnBnClickedOk();
};
