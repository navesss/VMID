// DlgSample.cpp : implementation file
//

#include "stdafx.h"
#include "VolumeVisualizer.h"
#include "DlgSample.h"


// CDlgSample dialog

IMPLEMENT_DYNAMIC(CDlgSample, CDialog)

CDlgSample::CDlgSample(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSample::IDD, pParent)
	, m_sampleNumber(0)
	, m_sampleMethod(0)
	, m_accuracy(1.732)
{

	EnableAutomation();

	m_sampleNumber = 50;
	m_sampleMethod = 1;

}

CDlgSample::~CDlgSample()
{
}

void CDlgSample::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CDlgSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SAMPLE_NUM, m_sampleNumber);
	DDV_MinMaxInt(pDX, m_sampleNumber, 0, 5000);
	DDX_Radio(pDX, IDC_ID_RANDOM, m_sampleMethod);
	DDX_Text(pDX, IDC_EDIT2, m_accuracy);
}


BEGIN_MESSAGE_MAP(CDlgSample, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgSample::OnDeltaposSpin1)
	ON_BN_CLICKED(IDOK, &CDlgSample::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDlgSample, CDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_IDlgSample to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {5B3873E7-32D0-4FB8-9E23-6AE92A819B7A}
static const IID IID_IDlgSample =
{ 0x5B3873E7, 0x32D0, 0x4FB8, { 0x9E, 0x23, 0x6A, 0xE9, 0x2A, 0x81, 0x9B, 0x7A } };

BEGIN_INTERFACE_MAP(CDlgSample, CDialog)
	INTERFACE_PART(CDlgSample, IID_IDlgSample, Dispatch)
END_INTERFACE_MAP()


// CDlgSample message handlers

void CDlgSample::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	UpdateData(true);
	if (pNMUpDown->iDelta == 1)
	{
		if (m_sampleNumber > 0)
		{
			m_sampleNumber = m_sampleNumber - 10;
		}
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_sampleNumber = m_sampleNumber + 10;
	}
	UpdateData(false);
}

void CDlgSample::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
