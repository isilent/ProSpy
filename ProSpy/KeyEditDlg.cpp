// KeyEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProSpy.h"
#include "KeyEditDlg.h" 


// CKeyEditDlg 对话框

IMPLEMENT_DYNAMIC(CKeyEditDlg, CDialog)

CKeyEditDlg::CKeyEditDlg(OpItem *pItem, CWnd* pParent /*=NULL*/)
	: CDialog(CKeyEditDlg::IDD, pParent)
	, m_strInput(_T(""))
	, m_nTimeSpan(50)
{
	m_pItem = pItem;
}

CKeyEditDlg::~CKeyEditDlg()
{
}

void CKeyEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInput);
	DDX_Text(pDX, IDC_EDIT3, m_nTimeSpan);
	DDV_MaxChars(pDX, m_strInput, 16);
	DDX_Control(pDX, IDC_EDIT1, m_inputEdit);
}


BEGIN_MESSAGE_MAP(CKeyEditDlg, CDialog)  
END_MESSAGE_MAP()


// CKeyEditDlg 消息处理程序


BOOL CKeyEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for (int i=0;i<KEYINPUT_MAX;i++)
	{
		DWORD dwKey =m_pItem->detail.keyinput.dwKey[i];
		if(dwKey == 0)
			break;
		switch (LOWORD(dwKey))
		{
		case VK_CONTROL:
			((CButton*)GetDlgItem(IDC_CHK_CTRL))->SetCheck(BST_CHECKED);
			break;
		case VK_SHIFT:
			((CButton*)GetDlgItem(IDC_CHK_SHIFT))->SetCheck(BST_CHECKED);
			break;
		case VK_MENU:
			((CButton*)GetDlgItem(IDC_CHK_ALT))->SetCheck(BST_CHECKED);
			break;
		case VK_LWIN:
			((CButton*)GetDlgItem(IDC_CHK_WIN))->SetCheck(BST_CHECKED);
			break;
		case VK_DELETE:
			((CButton*)GetDlgItem(IDC_CHK_DELETE))->SetCheck(BST_CHECKED);
			break;
		case VK_SNAPSHOT:
			((CButton*)GetDlgItem(IDC_CHK_PRINT))->SetCheck(BST_CHECKED);
			break;
		case VK_RETURN:
			((CButton*)GetDlgItem(IDC_CHK_ENTER))->SetCheck(BST_CHECKED);
			break;
		default:
			m_strInput.AppendChar(LOWORD(dwKey));
			break;
		} 
	}
	if (m_pItem->dwTimeSpan >0 )
	{
		m_nTimeSpan = m_pItem->dwTimeSpan;
	} 
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CKeyEditDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(!UpdateData(TRUE))
		return;
	m_pItem->detail.keyinput.Clear();
	int index = 0;
	GetContrlKeys(index);
	for (int i=0;i<m_strInput.GetLength();i++)
	{
		m_pItem->detail.keyinput.dwKey[index++] = m_strInput.GetAt(i);
	}
	GetSpecialKeys(index);
	m_pItem->dwTimeSpan = m_nTimeSpan;
	CDialog::OnOK();
}

void CKeyEditDlg::GetContrlKeys(int &index)
{
	if(((CButton*)GetDlgItem(IDC_CHK_CTRL))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_CONTROL | CONTROL_KEY_MASK;
	}
	if (((CButton*)GetDlgItem(IDC_CHK_SHIFT))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_SHIFT | CONTROL_KEY_MASK;
	}
	if (((CButton*)GetDlgItem(IDC_CHK_ALT))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_MENU | CONTROL_KEY_MASK;
	}	
	if (((CButton*)GetDlgItem(IDC_CHK_WIN))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_LWIN | CONTROL_KEY_MASK;
	} 
}

void CKeyEditDlg::GetSpecialKeys(int &index)
{
	if(((CButton*)GetDlgItem(IDC_CHK_DELETE))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_DELETE | VIRTUAL_KEY_MASK; 
	}
	if (((CButton*)GetDlgItem(IDC_CHK_PRINT))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_SNAPSHOT | VIRTUAL_KEY_MASK; 
	}
	if (((CButton*)GetDlgItem(IDC_CHK_ENTER))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_RETURN | VIRTUAL_KEY_MASK; 
	}
}
