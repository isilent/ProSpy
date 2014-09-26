#pragma once

#include "Define.h"

// CMouseEditDlg 对话框

class CMouseEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CMouseEditDlg)

public:
	CMouseEditDlg(OpItem *pItem,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMouseEditDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MOUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP() 
	int m_nX;
	int m_nY;
	UINT m_nTimeSpan;
	OpItem *m_pItem;

protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
