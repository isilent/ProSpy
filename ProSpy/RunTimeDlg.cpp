// RunTimeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProSpy.h"
#include "RunTimeDlg.h"


// CRunTimeDlg 对话框

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


// CRunTimeDlg 消息处理程序

void CRunTimeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	CDialog::OnOK();
}
