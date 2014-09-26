#pragma once


// CEnumProcDlg �Ի���

class CEnumProcDlg : public CDialog
{
	DECLARE_DYNAMIC(CEnumProcDlg)

public:
	CEnumProcDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEnumProcDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ENUM_PROC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString GetSelectedPID(){return m_strPID;}
	CString GetProcessName(){return m_strName;}
protected:
	virtual void OnOK();
	void PrintProcessNameAndID( DWORD processID );
	CListCtrl m_procList;
	CString m_strPID;
	CString m_strName;
public:
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
