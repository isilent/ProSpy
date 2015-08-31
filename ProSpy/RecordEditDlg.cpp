// RecordEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProSpy.h"
#include "RecordEditDlg.h"
#include "EnumProcDlg.h"


// CRecordEditDlg 对话框

IMPLEMENT_DYNAMIC(CRecordEditDlg, CDialog)

CRecordEditDlg::CRecordEditDlg(LPITEM pItem, CWnd* pParent /*=NULL*/)
	: CDialog(CRecordEditDlg::IDD, pParent)
	, m_strPID(_T(""))
	, m_strFilePath(_T("")) 
	, m_nTimeSpan(0)
	, m_strProcess(_T(""))
{
	m_pItem = pItem;
}

CRecordEditDlg::~CRecordEditDlg()
{
}

void CRecordEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProcInfoList);
	DDX_Text(pDX, IDC_EDIT1, m_strPID);
	DDX_Text(pDX, IDC_EDIT2, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT3, m_nTimeSpan);
	DDX_Text(pDX, IDC_EDIT4, m_strProcess);
}


BEGIN_MESSAGE_MAP(CRecordEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEL_PROC, &CRecordEditDlg::OnBnClickedBtnSelProc)
	ON_BN_CLICKED(IDC_BTN_SEL_PATH, &CRecordEditDlg::OnBnClickedBtnSelPath)
	ON_BN_CLICKED(IDC_BTN_ALL, &CRecordEditDlg::OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_BTN_NONE, &CRecordEditDlg::OnBnClickedBtnNone)
END_MESSAGE_MAP()


// CRecordEditDlg 消息处理程序

void CRecordEditDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(!UpdateData(TRUE))
		return;
	if (m_strPID.IsEmpty())
	{
		AfxMessageBox(_T("Please select a process"));
		return;
	}	
	if (m_strFilePath.IsEmpty())
	{
		AfxMessageBox(_T("Please set the file path"));
		return;
	}
	if(m_nTimeSpan == 0)
	{
		AfxMessageBox(_T("Please set time interval"));
		return;
	}
	m_pItem->dwTimeSpan = m_nTimeSpan;
	m_pItem->detail.record.dwProcessID = _wtoi(m_strPID);
	wcscpy_s(m_pItem->detail.record.szFilePath,MAX_PATH,m_strFilePath); 
	wcscpy_s(m_pItem->detail.record.szProcessName,PROCESS_NAME_LEN,m_strProcess);
	DWORD mask = 0;
	for (int i=0 ;i<m_ProcInfoList.GetItemCount(); i++)
	{
		if (m_ProcInfoList.GetCheck(i))
		{
			mask |= (DWORD)m_ProcInfoList.GetItemData(i);
		}
	}
	m_pItem->detail.record.dwMask = mask;
	CDialog::OnOK();
}

BOOL CRecordEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProcInfoList.SetExtendedStyle(m_ProcInfoList.GetExtendedStyle()|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT); 

	int row = 0;
	m_ProcInfoList.InsertColumn(0,_T(""),LVCFMT_LEFT,140);
	m_ProcInfoList.InsertItem(row,_T("PID"));
	m_ProcInfoList.SetItemData(row++,RECORD_PID); 
	m_ProcInfoList.InsertItem(row,_T("CPU Usage(%)"));
	m_ProcInfoList.SetItemData(row++, RECORD_CPU_USAGE);
	m_ProcInfoList.InsertItem(row, _T("CPU Kernel(%)"));
	m_ProcInfoList.SetItemData(row++, RECORD_KERNEL_CPU);
	m_ProcInfoList.InsertItem(row,_T("Physical Memory"));
	m_ProcInfoList.SetItemData(row++,RECORD_WORK_SET); 
	m_ProcInfoList.InsertItem(row,_T("Virtual Memory"));
	m_ProcInfoList.SetItemData(row++,RECORD_VIRTUAL_MEM); 
	m_ProcInfoList.InsertItem(row,_T("Page Fault"));
	m_ProcInfoList.SetItemData(row++,RECORD_PAGE_FAULT); 
	m_ProcInfoList.InsertItem(row,_T("Handle"));
	m_ProcInfoList.SetItemData(row++,RECORD_HANDLE_COUNT);
	m_ProcInfoList.InsertItem(row,_T("Thread"));
	m_ProcInfoList.SetItemData(row++,RECORD_THREAD_COUNT);
	m_ProcInfoList.InsertItem(row,_T("User Object"));
	m_ProcInfoList.SetItemData(row++,RECORD_USER_OBJECT);
	m_ProcInfoList.InsertItem(row,_T("GDI Object"));
	m_ProcInfoList.SetItemData(row++,RECORD_GDI_COUNT);
	
	m_nTimeSpan = m_pItem->dwTimeSpan;

	if (m_pItem->detail.record.dwProcessID >0)
	{
		m_strPID.Format(_T("%u"),m_pItem->detail.record.dwProcessID);
	}
	m_strProcess= m_pItem->detail.record.szProcessName;
	m_strFilePath = m_pItem->detail.record.szFilePath;

	for (int i=0 ;i<m_ProcInfoList.GetItemCount(); i++)
	{
		if ((m_ProcInfoList.GetItemData(i) & m_pItem->detail.record.dwMask) !=0 )
		{
			m_ProcInfoList.SetCheck(i,BST_CHECKED);
		}
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRecordEditDlg::OnBnClickedBtnSelProc()
{
	// TODO: 在此添加控件通知处理程序代码
	CEnumProcDlg dlg;
	if(dlg.DoModal()== IDOK)
	{
		m_strPID = dlg.GetSelectedPID();
		m_strProcess = dlg.GetProcessName();
		UpdateData(FALSE);
	}
}

void CRecordEditDlg::OnBnClickedBtnSelPath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE,_T(".csv"),NULL,0,_T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));
	if(dlg.DoModal()== IDOK)
	{
		m_strFilePath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CRecordEditDlg::OnBnClickedBtnAll()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0 ;i<m_ProcInfoList.GetItemCount(); i++)
	{ 
		m_ProcInfoList.SetCheck(i,BST_CHECKED); 
	}
}

void CRecordEditDlg::OnBnClickedBtnNone()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0 ;i<m_ProcInfoList.GetItemCount(); i++)
	{ 
		m_ProcInfoList.SetCheck(i,BST_UNCHECKED); 
	}
}
