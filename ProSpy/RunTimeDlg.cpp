// RunTimeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProSpy.h"
#include "RunTimeDlg.h"


// CRunTimeDlg �Ի���

IMPLEMENT_DYNAMIC(CRunTimeDlg, CDialog)

CRunTimeDlg::CRunTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunTimeDlg::IDD, pParent)
	, m_nRunTime(0)
{

}

CRunTimeDlg::~CRunTimeDlg()
{
}

void CRunTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RUN_TIME, m_nRunTime);
}


BEGIN_MESSAGE_MAP(CRunTimeDlg, CDialog)
END_MESSAGE_MAP()


// CRunTimeDlg ��Ϣ�������

void CRunTimeDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	CDialog::OnOK();
}
