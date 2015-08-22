#pragma once
#include "Define.h"
#include "afxwin.h"
#include "LimitEdit.h"

// CKeyEditDlg �Ի���

class CKeyEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyEditDlg)

public:
	CKeyEditDlg(LPITEM pItem, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKeyEditDlg();

// �Ի�������
	enum { IDD = IDD_DLG_KEYBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CString m_strInput;
	UINT m_nTimeSpan;
	LPITEM m_pItem;
	CLimitEdit m_inputEdit;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();  
private:
	void GetControlKeys(int &index);
	void SetContrlKeys(int &index);
	void GetSpecialKeys(int &index); 
};