// SetValueDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VolumeVisualizer.h"
#include "SetValueDlg.h"


// CSetValueDlg �Ի���

IMPLEMENT_DYNAMIC(CSetValueDlg, CDialog)

CSetValueDlg::CSetValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetValueDlg::IDD, pParent)
	, m_accuracy(1.414)
{

}

CSetValueDlg::~CSetValueDlg()
{
}

void CSetValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_accuracy);
}


BEGIN_MESSAGE_MAP(CSetValueDlg, CDialog)
END_MESSAGE_MAP()


// CSetValueDlg ��Ϣ�������
