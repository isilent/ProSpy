// ProSpyDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ProjectFile.h"
#include "RunThread.h"

// CProSpyDlg �Ի���
class CProSpyDlg : public CDialog
{
// ����
public:
	CProSpyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PROSPY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	void AddItemToList( OpItem *pItem );
	void ShowProject();
	void AddMouseOperation(int x, int y, bool showDlg);
	CString GetKeyInput(const OpKeyInput&input);
// ʵ��
protected:
	HICON m_hIcon;
	CListCtrl m_opList;
	ProjectFile m_oProj; 
	CRunThread *m_pThread;
	CStatic m_cStatic;
	bool m_bCaptureCursor;
	HCURSOR m_hCursor;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditAddkeyboardevent();
	afx_msg void OnEditAddmouseevent();
	afx_msg void OnEditAddresourcerecord();
	afx_msg void OnHelpAbout();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextEdit();
	void UpdateItem( int index, OpItem *pItem );
	afx_msg void OnContextDelete();
	afx_msg void OnContextMoveup();
	afx_msg void OnContextMovedown();
	afx_msg LRESULT OnThreadStop(WPARAM wparam,LPARAM lparam);
	afx_msg void OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};
