// RemarkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProSpy.h"
#include "RemarkDlg.h"


// CRemarkDlg 对话框

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


// CRemarkDlg 消息处理程序

void CRemarkDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CRemarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT1)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
