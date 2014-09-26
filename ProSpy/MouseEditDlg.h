#pragma once

#include "Define.h"

// CMouseEditDlg �Ի���

class CMouseEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CMouseEditDlg)

public:
	CMouseEditDlg(OpItem *pItem,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMouseEditDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MOUSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
