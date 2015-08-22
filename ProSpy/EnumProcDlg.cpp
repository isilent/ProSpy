// EnumProcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProSpy.h"
#include "EnumProcDlg.h"


// CEnumProcDlg �Ի���

IMPLEMENT_DYNAMIC(CEnumProcDlg, CDialog)

CEnumProcDlg::CEnumProcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnumProcDlg::IDD, pParent)
{

}

CEnumProcDlg::~CEnumProcDlg()
{
}

void CEnumProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_procList);
}


BEGIN_MESSAGE_MAP(CEnumProcDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CEnumProcDlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// CEnumProcDlg ��Ϣ�������

BOOL CEnumProcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_procList.SetExtendedStyle(m_procList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_procList.InsertColumn(0,_T("Process"),LVCFMT_LEFT,180);
	m_procList.InsertColumn(1,_T("PID"),LVCFMT_LEFT,60);
	DWORD aProcesses[2048], cbNeeded, cProcesses; 
	if (!EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ))
		return TRUE;
	cProcesses = cbNeeded / sizeof(DWORD);

	for(UINT i = 0; i < cProcesses; i++ )
		PrintProcessNameAndID( aProcesses[i]);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEnumProcDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	POSITION ps =m_procList.GetFirstSelectedItemPosition();
	int nIndex = m_procList.GetNextSelectedItem(ps);
	if (nIndex>=0)
	{
		m_strName = m_procList.GetItemText(nIndex,0);
		m_strPID = m_procList.GetItemText(nIndex,1);
	}
	CDialog::OnOK();
}

void CEnumProcDlg::PrintProcessNameAndID( DWORD processID )
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );

	if (NULL != hProcess )
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
			&cbNeeded) )
		{
			GetModuleBaseName( hProcess, hMod, szProcessName, 
				sizeof(szProcessName)/sizeof(TCHAR) );
			int nIndex = m_procList.InsertItem(m_procList.GetItemCount(),szProcessName);
			CString strPID;
			strPID.Format(_T("%u"),processID);
			m_procList.SetItemText(nIndex,1,strPID);
		}
	}

	CloseHandle( hProcess );
}


void CEnumProcDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	OnOK();
}