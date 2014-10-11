#pragma once


// CRunTimeDlg 对话框

class CRunTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CRunTimeDlg)

public:
	CRunTimeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRunTimeDlg();

// 对话框数据
	enum { IDD = IDD_DLG_RUNTIME };
	int GetRunTime(){return m_nRunTime;};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_nRunTime;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
