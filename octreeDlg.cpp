// octreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VolumeVisualizer.h"
#include "octreeDlg.h"


// CoctreeDlg dialog

IMPLEMENT_DYNAMIC(CoctreeDlg, CDialog)

CoctreeDlg::CoctreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CoctreeDlg::IDD, pParent)
	, m_depth(4)
{

}

CoctreeDlg::~CoctreeDlg()
{
}

void CoctreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_depth);
	DDV_MinMaxInt(pDX, m_depth, 0, INT_MAX);
}


BEGIN_MESSAGE_MAP(CoctreeDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CoctreeDlg::OnDeltaposSpin1)
	
END_MESSAGE_MAP()


// CoctreeDlg message handlers

void CoctreeDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (pNMUpDown->iDelta == 1)
	{
		if (m_depth > 0)
		{
			m_depth--;
		}
		
	}
	else if (pNMUpDown->iDelta == -1)
	{
		m_depth++;
	}
	UpdateData(false);
	*pResult = 0;
}


