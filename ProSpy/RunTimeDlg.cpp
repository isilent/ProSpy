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
	if(!UpdateData(TRUE))
		return;
	CDialog::OnOK();
}

BOOL CRunTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CEdit*)GetDlgItem(IDC_EDIT_RUN_TIME))->SetSel(0,-1);
	((CEdit*)GetDlgItem(IDC_EDIT_RUN_TIME))->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
