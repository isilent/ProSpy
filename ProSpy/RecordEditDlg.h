#pragma once

#include "Define.h"

// CRecordEditDlg 对话框

class CRecordEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecordEditDlg)

public:
	CRecordEditDlg(LPITEM pItem, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordEditDlg();

// 对话框数据
	enum { IDD = IDD_DLG_RECORD };
protected:
	CListCtrl m_ProcInfoList;
	CString m_strProcess;
	CString m_strPID;
	CString m_strFilePath; 
	LPITEM m_pItem;
	UINT m_nTimeSpan;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSelProc();
	afx_msg void OnBnClickedBtnSelPath();
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnBnClickedBtnNone();
};
