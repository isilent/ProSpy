#pragma once

#include "Define.h"

// CRecordEditDlg �Ի���

class CRecordEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecordEditDlg)

public:
	CRecordEditDlg(LPITEM pItem, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecordEditDlg();

// �Ի�������
	enum { IDD = IDD_DLG_RECORD };
protected:
	CListCtrl m_ProcInfoList;
	CString m_strProcess;
	CString m_strPID;
	CString m_strFilePath; 
	LPITEM m_pItem;
	UINT m_nTimeSpan;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSelProc();
	afx_msg void OnBnClickedBtnSelPath();
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnBnClickedBtnNone();
};
