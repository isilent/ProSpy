// ProSpyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProSpy.h"
#include "ProSpyDlg.h"
#include "RunTimeDlg.h"
#include "RecordEditDlg.h"
#include "MouseEditDlg.h"
#include "KeyEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HOTKEY_CAPTURE_MOUSE 0x0010
#define HOTKEY_STOP          0x0011

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CProSpyDlg �Ի���




CProSpyDlg::CProSpyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProSpyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pThread = NULL;
	m_bCaptureCursor =false;
}

void CProSpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_opList);
	DDX_Control(pDX, IDC_STATIC_IMG, m_cStatic);
}

BEGIN_MESSAGE_MAP(CProSpyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BTN_START, &CProSpyDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(ID_BTN_STOP, &CProSpyDlg::OnBnClickedBtnStop)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CProSpyDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProSpyDlg::OnNMRclickList1)
	ON_COMMAND(ID_FILE_NEW, &CProSpyDlg::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CProSpyDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CProSpyDlg::OnFileSave)
	ON_COMMAND(ID_FILE_SAVEAS, &CProSpyDlg::OnFileSaveAs)
	ON_COMMAND(ID_EDIT_ADDKEYBOARDEVENT, &CProSpyDlg::OnEditAddkeyboardevent)
	ON_COMMAND(ID_EDIT_ADDMOUSEEVENT, &CProSpyDlg::OnEditAddmouseevent)
	ON_COMMAND(ID_EDIT_ADDRESOURCERECORD, &CProSpyDlg::OnEditAddresourcerecord)
	ON_COMMAND(ID_HELP_ABOUT, &CProSpyDlg::OnHelpAbout)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_CONTEXT_EDIT, &CProSpyDlg::OnContextEdit)
	ON_COMMAND(ID_CONTEXT_DELETE, &CProSpyDlg::OnContextDelete)
	ON_COMMAND(ID_CONTEXT_MOVEUP, &CProSpyDlg::OnContextMoveup)
	ON_COMMAND(ID_CONTEXT_MOVEDOWN, &CProSpyDlg::OnContextMovedown)
	ON_MESSAGE(WM_THREAD_STOP,&CProSpyDlg::OnThreadStop)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CProSpyDlg::OnLvnKeydownList1)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CProSpyDlg ��Ϣ�������

BOOL CProSpyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_hCursor = GetCursor();
	BOOL bRet = RegisterHotKey(GetSafeHwnd(),HOTKEY_CAPTURE_MOUSE,MOD_SHIFT|MOD_CONTROL,'1');
	bRet |= RegisterHotKey(GetSafeHwnd(),HOTKEY_STOP,MOD_SHIFT|MOD_CONTROL,'2');
	if (!bRet)
	{
		AfxMessageBox(_T("Failed to set shortcut key"));
	}
	m_opList.SetExtendedStyle(m_opList.GetExtendedStyle()|LVS_EX_FULLROWSELECT); 

	m_opList.InsertColumn(0,_T("��������"),LVCFMT_LEFT,100);
	m_opList.InsertColumn(1,_T("����"),LVCFMT_LEFT,140);
	m_opList.InsertColumn(2,_T("ʱ����(��λ10ms)"),LVCFMT_LEFT,140);
	m_opList.InsertColumn(3,_T("��ע"),LVCFMT_LEFT,140);

	if (m_oProj.Open())
	{
		ShowProject();
	} 
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CProSpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CProSpyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CProSpyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProSpyDlg::AddItemToList( OpItem *pItem )
{
	int nIndex = m_opList.InsertItem(m_opList.GetItemCount(),_T("Item"));
	m_opList.SetItemData(nIndex,(DWORD_PTR)pItem);
	UpdateItem(nIndex,pItem);
}

void CProSpyDlg::ShowProject()
{
	m_opList.DeleteAllItems();
	list<OpItem*> &itemList = m_oProj.GetItemList();
	for(list<OpItem*>::iterator iT = itemList.begin(); iT != itemList.end();iT++)
	{
		AddItemToList(*iT);
	}
}

BOOL CProSpyDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_HOTKEY)
	{
		if(pMsg->wParam == HOTKEY_CAPTURE_MOUSE)
		{		
			POINT pt;
			GetCursorPos(&pt); 
			AddMouseOperation(pt.x,pt.y,false);
			return TRUE;
		}
		else if(pMsg->wParam == HOTKEY_STOP)
		{
			OnBnClickedBtnStop();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CProSpyDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pThread != NULL)
	{
		AfxMessageBox(_T("Already Running"));
		return;
	}
	list<OpItem*> &oplist = m_oProj.GetItemList();
	if (oplist.empty())
	{
		AfxMessageBox(_T("Project is empty"));
		return ;
	}
	CRunTimeDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	m_oProj.Save();
	m_pThread = new CRunThread(GetSafeHwnd(),oplist);
	m_pThread->Start(dlg.GetRunTime());
	GetDlgItem(ID_BTN_START)->EnableWindow(FALSE);
	ShowWindow(SW_MINIMIZE);
}

void CProSpyDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ���������� 
	if (m_pThread !=NULL)
	{
		m_pThread->Stop(); 
	}
}

void CProSpyDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	OnContextEdit();
}

void CProSpyDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( m_opList.GetSelectedCount() < 0)
		return;
	CMenu menu,*pSubMenu;
	menu.LoadMenu(IDR_MENU_CONTEXT);
	pSubMenu=menu.GetSubMenu(0);
	CPoint point;
	GetCursorPos(&point);//�õ���괦 
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0; 
}

void CProSpyDlg::OnFileNew()
{
	// TODO: �ڴ���������������
	if (!m_oProj.IsEmpty() && !m_oProj.Save())
	{
		if(MessageBox(_T("���湤��ʧ��,�Ƿ������ǰ����"),_T("Message"),MB_YESNO) != IDYES)
			return;
	}
	m_oProj.Discard();
	ShowProject();
}

void CProSpyDlg::OnFileOpen()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE,_T("ps"),NULL,0,_T("AutoGhost Files (*.ps)|*.ps|All Files (*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
		return;
	CString file = dlg.GetPathName();
	if (m_oProj.Open(file))
	{
		ShowProject();
	}
	else
	{
		AfxMessageBox(_T("�򿪹���ʧ��"));
	}
}

void CProSpyDlg::OnFileSave()
{
	// TODO: �ڴ���������������	
	if(!m_oProj.Save())
	{
		AfxMessageBox(_T("���湤��ʧ��"));
	}
}

void CProSpyDlg::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(FALSE,_T("ps"),NULL,0,_T("ProSpy Files (*.ps)|*.ps|All Files (*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
		return;
	CString file = dlg.GetPathName();
	if (!m_oProj.Save(file))
	{ 
		AfxMessageBox(_T("���湤��ʧ��"));
	}
}

void CProSpyDlg::OnEditAddkeyboardevent()
{
	// TODO: �ڴ��������������� 
	OpItem *pItem = new OpItem;
	pItem->type = OP_KEY_INPUT; 
	CKeyEditDlg dlg(pItem);
	if(dlg.DoModal()!=IDOK)
	{
		delete pItem;
		return;
	} 
	m_oProj.AddItem(pItem);
	AddItemToList(pItem);
}

void CProSpyDlg::OnEditAddmouseevent()
{
	// TODO: �ڴ���������������
	AddMouseOperation(0,0,true);
}

void CProSpyDlg::OnEditAddresourcerecord()
{
	// TODO: �ڴ���������������
	OpItem *pItem = new OpItem;
	pItem->type = OP_RECORD;
	pItem->detail.record.dwMask = RECORD_PID | RECORD_VIRTUAL_MEM | RECORD_HANDLE_COUNT;
 	CRecordEditDlg dlg(pItem);
 	if(dlg.DoModal()!=IDOK)
 	{
 		delete pItem;
 		return;
 	} 
 	m_oProj.AddItem(pItem);
 	AddItemToList(pItem);
}

void CProSpyDlg::OnHelpAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CProSpyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd *pWnd = ChildWindowFromPoint(point);
	if (pWnd != NULL && pWnd->GetSafeHwnd() == m_cStatic.GetSafeHwnd())
	{ 
		SetCapture();
		m_bCaptureCursor = true;
		SetCursor(::LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CAPTURE)));
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CProSpyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bCaptureCursor)
	{
		m_bCaptureCursor = false;
		POINT pt;
		GetCursorPos(&pt);
		AddMouseOperation(pt.x,pt.y,false); 
		ReleaseCapture();
		SetCursor(m_hCursor);
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CProSpyDlg::OnContextEdit()
{
	// TODO: �ڴ���������������
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	int index = m_opList.GetNextSelectedItem(pos);
	if(index<0)
		return;
	OpItem * pItem = (OpItem*)m_opList.GetItemData(index);
	switch(pItem->type)
	{
	case OP_RECORD:
		{
			CRecordEditDlg dlg(pItem);
			dlg.DoModal();
			break;
		}
	case OP_LCLICK:
	case OP_RCLICK:
	case OP_DBCICK:
		{
			CMouseEditDlg dlg(pItem);
		 	dlg.DoModal();
			break;
		}
	case OP_KEY_INPUT:
		{
			CKeyEditDlg dlg(pItem);
			dlg.DoModal();
			break;
		}
		break;
	}
	UpdateItem(index,pItem);
	m_oProj.Save();
}

void CProSpyDlg::UpdateItem( int index, OpItem *pItem )
{ 
	CString strItemName,strContent;
	switch(pItem->type)
	{
	case OP_RECORD:
		strItemName.Format(_T("Record")); 
		strContent.Format(_T("%s(%d)"),pItem->detail.record.szProcessName,pItem->detail.record.dwProcessID);
		break;
	case OP_RCLICK:
		strItemName.Format(_T("Right Click")); 
		strContent.Format(_T("%d,%d"),pItem->detail.pos.x,pItem->detail.pos.y);
		break;
	case OP_LCLICK:
		strItemName.Format(_T("Left Click")); 
		strContent.Format(_T("%d,%d"),pItem->detail.pos.x,pItem->detail.pos.y);
		break;
	case OP_DBCICK:
		strItemName.Format(_T("Double Click")); 
		strContent.Format(_T("%d,%d"),pItem->detail.pos.x,pItem->detail.pos.y);
		break;
	case OP_KEY_INPUT:
		strItemName.Format(_T("Keyboard Input")); 
		strContent = GetKeyInput(pItem->detail.keyinput);
		break;
	case OP_UNKNOWN:
		strItemName.Format(_T("Unknown")); 
		break;
	} 
	CString strTime;
	strTime.Format(_T("%u"),pItem->dwTimeSpan);
	m_opList.SetItemText(index,0,strItemName);
	m_opList.SetItemText(index,1,strContent);
	m_opList.SetItemText(index,2,strTime);
}

void CProSpyDlg::OnContextDelete()
{
	// TODO: �ڴ���������������
	POSITION pos = m_opList.GetFirstSelectedItemPosition(); 
	int maxIndex  = 0;
	while(pos != NULL)
	{
		int index = m_opList.GetNextSelectedItem(pos); 
		maxIndex = max(maxIndex,index);
		OpItem * pItem = (OpItem*)m_opList.GetItemData(index);
		m_oProj.DeleteItem(pItem); 
	}
	ShowProject();
	m_opList.SetItemState(maxIndex+1, LVIS_SELECTED, LVIS_SELECTED);
}

void CProSpyDlg::OnContextMoveup()
{
	// TODO: �ڴ���������������
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	int index = m_opList.GetNextSelectedItem(pos);
	if(index<=0)
		return;
	OpItem * pItem = (OpItem*)m_opList.GetItemData(index); 
	m_oProj.MoveUp(pItem);
	ShowProject(); 
	m_opList.SetItemState(index-1, LVIS_SELECTED, LVIS_SELECTED); 
}

void CProSpyDlg::OnContextMovedown()
{
	// TODO: �ڴ���������������
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	int index = m_opList.GetNextSelectedItem(pos);
	if(index<0 || (index == m_opList.GetItemCount()-1))
		return;
	OpItem * pItem = (OpItem*)m_opList.GetItemData(index); 
	m_oProj.MoveDown(pItem);
	ShowProject();      
	m_opList.SetItemState(index+1, LVIS_SELECTED, LVIS_SELECTED);  
}

LRESULT CProSpyDlg::OnThreadStop( WPARAM wparam,LPARAM lparam )
{ 
	GetDlgItem(ID_BTN_START)->EnableWindow(TRUE);
	CString str;
	str.Format(_T("�ѳɹ�ִ��%d��"),(int)wparam);
	AfxMessageBox(str);
	if (m_pThread != NULL)
	{
		delete m_pThread;
		m_pThread = NULL;
	}
	return TRUE;
}

void CProSpyDlg::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pLVKeyDow->wVKey == 'S')
	{
		OnContextMovedown();
	}
	else if (pLVKeyDow->wVKey == 'W')
	{
		OnContextMoveup();
	}
	else if (pLVKeyDow->wVKey == VK_DELETE)
	{
		OnContextDelete();
	}
	*pResult = 0;
}

void CProSpyDlg::AddMouseOperation( int x, int y, bool showDlg)
{
	OpItem *pItem = new OpItem;
	pItem->type = OP_LCLICK; //Ĭ����� 
	pItem->detail.pos.x = x;
	pItem->detail.pos.y = y;
	pItem->dwTimeSpan = 100;
	if (showDlg)
	{
		CMouseEditDlg dlg(pItem);
		if(dlg.DoModal()!=IDOK)
		{
			delete pItem;
			return;
		} 
	}
	
	m_oProj.AddItem(pItem);
	AddItemToList(pItem);
}

void CProSpyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (IsWindow(m_opList.GetSafeHwnd()))
	{
		m_opList.SetWindowPos(NULL,0,0,cx-20,cy-105,SWP_NOMOVE);
	}
	if (IsWindow(m_cStatic.GetSafeHwnd()))
	{
		m_cStatic.SetWindowPos(NULL,12,cy-38,0,0,SWP_NOSIZE);
	}
	CWnd *pBtnStart =GetDlgItem(ID_BTN_START);
	CWnd *pBtnStop =GetDlgItem(ID_BTN_STOP);
	if (IsWindow(pBtnStart->GetSafeHwnd()))
	{
		pBtnStart->SetWindowPos(NULL,cx-170,cy-35,0,0,SWP_NOSIZE);
	}
	if (IsWindow(pBtnStop->GetSafeHwnd()))
	{
		pBtnStop->SetWindowPos(NULL,cx-83,cy-35,0,0,SWP_NOSIZE);
	}
}

void CProSpyDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_oProj.IsModified())
	{
		int nRet = AfxMessageBox(_T("�����޸ĺ�δ���棬�Ƿ񱣴�"),MB_YESNOCANCEL);
		if (nRet == IDYES  && !m_oProj.Save())
		{
			AfxMessageBox(_T("���湤��ʧ��"));
			return;
		}
		else if (nRet == IDCANCEL)
		{
			return;
		}

	}
	CDialog::OnClose();
}

CString CProSpyDlg::GetKeyInput( const OpKeyInput&input )
{
	CString str;
	for (int i=0;i<KEYINPUT_MAX;i++)
	{
		DWORD dwKey = input.dwKey[i];
		if (dwKey == 0)
		{
			break;
		} 
		if(HIWORD(dwKey) == (VISIBLE_CHAR_MASK>>4))
		{
			str.AppendChar((WORD)input.dwKey[i]);
		}
	}
	return str;
}
