#pragma once


// CRemarkDlg 对话框

class CRemarkDlg : public CDialog
{
	DECLARE_DYNAMIC(CRemarkDlg)

public:
	CRemarkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRemarkDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRemark;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
