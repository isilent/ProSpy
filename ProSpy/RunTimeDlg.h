#pragma once


// CRunTimeDlg �Ի���

class CRunTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(CRunTimeDlg)

public:
	CRunTimeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRunTimeDlg();

// �Ի�������
	enum { IDD = IDD_DLG_RUNTIME };
	int GetRunTime(){return m_nRunTime;};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	int m_nRunTime;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
