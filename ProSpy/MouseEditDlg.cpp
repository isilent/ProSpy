// MouseEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProSpy.h"
#include "MouseEditDlg.h"


// CMouseEditDlg �Ի���

IMPLEMENT_DYNAMIC(CMouseEditDlg, CDialog)

CMouseEditDlg::CMouseEditDlg(OpItem *pItem,CWnd* pParent /*=NULL*/)
	: CDialog(CMouseEditDlg::IDD, pParent)
	, m_nX(0)
	, m_nY(0)
	, m_nTimeSpan(50)
{
	m_pItem = pItem;
}

CMouseEditDlg::~CMouseEditDlg()
{
}

void CMouseEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X, m_nX);
	DDX_Text(pDX, IDC_EDIT_Y, m_nY);
	DDX_Text(pDX, IDC_EDIT3, m_nTimeSpan);
}


BEGIN_MESSAGE_MAP(CMouseEditDlg, CDialog)
END_MESSAGE_MAP()


// CMouseEditDlg ��Ϣ�������

void CMouseEditDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	m_pItem->detail.pos.x = m_nX;
	m_pItem->detail.pos.y = m_nY;

	if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == BST_CHECKED)
	{
		m_pItem->type = OP_LCLICK;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck() == BST_CHECKED)
	{
		m_pItem->type = OP_RCLICK;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck() == BST_CHECKED)
	{
		m_pItem->type = OP_DBCICK;
	}
	m_pItem->dwTimeSpan = m_nTimeSpan;
	CDialog::OnOK();
}

BOOL CMouseEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_nX = m_pItem->detail.pos.x;
	m_nY = m_pItem->detail.pos.y; 
	switch(m_pItem->type)
	{
	case OP_LCLICK:
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
		break;
	case OP_RCLICK:
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(BST_CHECKED);
		break;
	case OP_DBCICK:
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(BST_CHECKED);
		break;
	}
	if (m_pItem->dwTimeSpan >0 )
	{
		m_nTimeSpan = m_pItem->dwTimeSpan;
	} 
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
