#pragma once

#include "Define.h"

// CRecordEditDlg �Ի���

class CRecordEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecordEditDlg)

public:
	CRecordEditDlg(OpItem *pItem,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordEditDlg();

// �Ի�������
	enum { IDD = IDD_DLG_RECORD };
protected:
	CListCtrl m_ProcInfoList;
	CString m_strPID;
	CString m_strProcessName;
	CString m_strFilePath; 
	OpItem *m_pItem;
	UINT m_nTimeSpan;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSelProc();
	afx_msg void OnBnClickedBtnSelPath();
};
