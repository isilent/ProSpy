// RemarkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProSpy.h"
#include "RemarkDlg.h"


// CRemarkDlg �Ի���

IMPLEMENT_DYNAMIC(CRemarkDlg, CDialog)

CRemarkDlg::CRemarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemarkDlg::IDD, pParent)
	, m_strRemark(_T(""))
{

}

CRemarkDlg::~CRemarkDlg()
{
}

void CRemarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRemark);
}


BEGIN_MESSAGE_MAP(CRemarkDlg, CDialog)
END_MESSAGE_MAP()


// CRemarkDlg ��Ϣ�������

void CRemarkDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CRemarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT1)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
