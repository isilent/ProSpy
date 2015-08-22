// KeyEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProSpy.h"
#include "KeyEditDlg.h" 

#define GET_SPECIAL_KEY(buttonId,vkey)  \
	if(((CButton*)GetDlgItem(buttonId))->GetCheck() == BST_CHECKED){\
	m_pItem->detail.keyinput.dwKey[index++] = vkey | ((DWORD)VIRTUAL_KEY_MASK<<MASK_SHIFT_COUNT);}

#define SWITCH_KEY_VALUE(vkey,buttonId) \
	case vkey:{((CButton*)GetDlgItem(buttonId))->SetCheck(BST_CHECKED);break;}

// CKeyEditDlg 对话框

IMPLEMENT_DYNAMIC(CKeyEditDlg, CDialog)

CKeyEditDlg::CKeyEditDlg(LPITEM pItem, CWnd* pParent /*=NULL*/)
	: CDialog(CKeyEditDlg::IDD, pParent)
	, m_strInput(_T(""))
	, m_nTimeSpan(0)
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
			SWITCH_KEY_VALUE(VK_CONTROL,IDC_CHK_CTRL)
			SWITCH_KEY_VALUE(VK_SHIFT,IDC_CHK_SHIFT)
			SWITCH_KEY_VALUE(VK_MENU,IDC_CHK_ALT)
			SWITCH_KEY_VALUE(VK_LWIN,IDC_CHK_WIN)
			SWITCH_KEY_VALUE(VK_DELETE,IDC_CHK_DELETE)
			SWITCH_KEY_VALUE(VK_SNAPSHOT,IDC_CHK_SNAPSHOT)
			SWITCH_KEY_VALUE(VK_RETURN,IDC_CHK_ENTER)
			SWITCH_KEY_VALUE(VK_PRIOR,IDC_CHK_UP)
			SWITCH_KEY_VALUE(VK_NEXT,IDC_CHK_DOWN)
			default:
				m_strInput.AppendChar(LOWORD(dwKey));
				break;
		} 
	}
	m_nTimeSpan = m_pItem->dwTimeSpan;
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
	GetControlKeys(index);
	for (int i=0;i<m_strInput.GetLength();i++)
	{
		m_pItem->detail.keyinput.dwKey[index++] = m_strInput.GetAt(i);
	}
	GetSpecialKeys(index);
	m_pItem->dwTimeSpan = m_nTimeSpan;
	CDialog::OnOK();
}

void CKeyEditDlg::GetControlKeys(int &index)
{
	if(((CButton*)GetDlgItem(IDC_CHK_CTRL))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_CONTROL | ((DWORD)CONTROL_KEY_MASK << MASK_SHIFT_COUNT);
	}
	if (((CButton*)GetDlgItem(IDC_CHK_SHIFT))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_SHIFT | ((DWORD)CONTROL_KEY_MASK << MASK_SHIFT_COUNT);
	}
	if (((CButton*)GetDlgItem(IDC_CHK_ALT))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_MENU | ((DWORD)CONTROL_KEY_MASK << MASK_SHIFT_COUNT);
	}	
	if (((CButton*)GetDlgItem(IDC_CHK_WIN))->GetCheck() == BST_CHECKED)
	{
		m_pItem->detail.keyinput.dwKey[index++] = VK_LWIN | ((DWORD)CONTROL_KEY_MASK << MASK_SHIFT_COUNT);
	} 
}

void CKeyEditDlg::GetSpecialKeys(int &index)
{ 
	GET_SPECIAL_KEY(IDC_CHK_DELETE,VK_DELETE)
	GET_SPECIAL_KEY(IDC_CHK_SNAPSHOT,VK_SNAPSHOT)
	GET_SPECIAL_KEY(IDC_CHK_ENTER,VK_RETURN)
	GET_SPECIAL_KEY(IDC_CHK_UP,VK_PRIOR)
	GET_SPECIAL_KEY(IDC_CHK_DOWN,VK_NEXT)
}
