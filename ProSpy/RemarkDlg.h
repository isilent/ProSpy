#pragma once


// CRemarkDlg �Ի���

class CRemarkDlg : public CDialog
{
	DECLARE_DYNAMIC(CRemarkDlg)

public:
	CRemarkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRemarkDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRemark;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
