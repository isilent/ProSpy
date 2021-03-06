// ProSpyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProSpy.h"
#include "ProSpyDlg.h"
#include "RunTimeDlg.h"
#include "RecordEditDlg.h"
#include "MouseEditDlg.h"
#include "KeyEditDlg.h"
#include "RemarkDlg.h"
#include "RegEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HOTKEY_CAPTURE_MOUSE 0x0010
#define HOTKEY_STOP          0x0011

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CProSpyDlg 对话框




CProSpyDlg::CProSpyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProSpyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	ON_COMMAND(ID_CONTEXT_ADDREMARK, &CProSpyDlg::OnContextAddremark)
	ON_COMMAND(ID_CONTEXT_COPY, &CProSpyDlg::OnContextCopy) 
	ON_COMMAND(ID_CONTEXT_PASTE, &CProSpyDlg::OnContextPaste)
END_MESSAGE_MAP()


// CProSpyDlg 消息处理程序

BOOL CProSpyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hCursor = GetCursor();
	BOOL bRet = RegisterHotKey(GetSafeHwnd(),HOTKEY_CAPTURE_MOUSE,MOD_SHIFT|MOD_CONTROL,'1');
	bRet |= RegisterHotKey(GetSafeHwnd(),HOTKEY_STOP,MOD_SHIFT|MOD_CONTROL,'2');
	if (!bRet)
	{
		AfxMessageBox(_T("Failed to register hotkey"));
	}
	m_opList.SetExtendedStyle(m_opList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 

	m_opList.InsertColumn(0,_T("Operation"),LVCFMT_LEFT,100);
	m_opList.InsertColumn(1,_T("Parameter"),LVCFMT_LEFT,140);
	m_opList.InsertColumn(2,_T("Interval(ms)"),LVCFMT_LEFT,100);
	m_opList.InsertColumn(3,_T("Note"),LVCFMT_LEFT,140);

	CString strFile;
	if (CRegEdit::ReadValue(HKEY_LOCAL_MACHINE,REG_PATH_APP, REG_ITEM_RUNFILE, strFile))
	{
		m_oProj.Open(strFile);
	}
	else
	{
		m_oProj.Open();
	}
	ShowProject(); 
	return TRUE;
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProSpyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CProSpyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProSpyDlg::AddItemToList(LPITEM pItem)
{
	int nIndex = m_opList.InsertItem(m_opList.GetItemCount(),_T("Item"));
	m_opList.SetItemData(nIndex,(DWORD_PTR)pItem);
	UpdateItem(nIndex,pItem);
}

void CProSpyDlg::ShowProject()
{
	UpdateTitle();
	m_opList.DeleteAllItems();
	auto &itemList = m_oProj.GetItemList();
	for(auto item:itemList)
	{
		AddItemToList(item);
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
	// TODO: 在此添加控件通知处理程序代码
	if (m_pThread != nullptr)
	{
		AfxMessageBox(_T("Already Running"));
		return;
	}
	auto &oplist = m_oProj.GetItemList();
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
	m_pThread = make_shared<CRunThread>(GetSafeHwnd(),oplist);
	m_pThread->Start(dlg.GetRunTime());
	GetDlgItem(ID_BTN_START)->EnableWindow(FALSE);
	ShowWindow(SW_MINIMIZE);
}

void CProSpyDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码 
	if (m_pThread !=nullptr)
	{
		m_pThread->Stop(); 
	}
}

void CProSpyDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	OnContextEdit();
}

void CProSpyDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_opList.GetSelectedCount() < 0)
		return;
	CMenu menu,*pSubMenu;
	menu.LoadMenu(IDR_MENU_CONTEXT);
	pSubMenu=menu.GetSubMenu(0);
	CPoint point;
	GetCursorPos(&point);//得到光标处 
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0; 
}

void CProSpyDlg::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_oProj.IsEmpty() && !m_oProj.Save())
	{
		if(MessageBox(_T("Save Project failed,Do you want to discard it?"),_T("Warnning"),MB_YESNO) != IDYES)
			return;
	}
	m_oProj.Destroy();
	ShowProject();
}

void CProSpyDlg::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE,_T("ps"),NULL,0,_T("ProcSpy Files (*.ps)|*.ps|All Files (*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
		return;
	CString file = dlg.GetPathName();
	if (m_oProj.Open(file))
	{
		ShowProject();
	}
	else
	{
		AfxMessageBox(_T("Open project failed"));
	}
}

void CProSpyDlg::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码	
	if(!m_oProj.Save())
	{
		AfxMessageBox(_T("Save project failed")); 
	}
	UpdateTitle();
}

void CProSpyDlg::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE,_T("ps"),NULL,0,_T("ProSpy Files (*.ps)|*.ps|All Files (*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
		return;
	CString file = dlg.GetPathName();
	if (!m_oProj.Save(file))
	{ 
		AfxMessageBox(_T("Save project failed"));
	}
	UpdateTitle();
}

void CProSpyDlg::OnEditAddkeyboardevent()
{
	// TODO: 在此添加命令处理程序代码 
	auto pItem = new OpItem;
	pItem->type = OP_KEY_INPUT;
	pItem->dwTimeSpan = DEFAULT_INTERVAL;
	CKeyEditDlg dlg(pItem);
	if(dlg.DoModal()!=IDOK)
	{
		delete pItem;
		return;
	} 
	AddItem(pItem);
}

void CProSpyDlg::OnEditAddmouseevent()
{
	// TODO: 在此添加命令处理程序代码
	AddMouseOperation(0,0,true);
}

void CProSpyDlg::OnEditAddresourcerecord()
{
	// TODO: 在此添加命令处理程序代码
	auto pItem = new OpItem;
	pItem->type = OP_RECORD;
	pItem->dwTimeSpan = DEFAULT_INTERVAL;
	pItem->detail.record.dwMask = RECORD_PID | RECORD_CPU_USAGE | RECORD_VIRTUAL_MEM | RECORD_HANDLE_COUNT;
 	CRecordEditDlg dlg(pItem);
 	if(dlg.DoModal()!=IDOK)
 	{
		delete pItem;
 		return;
	}
	AddItem(pItem);
}

void CProSpyDlg::OnHelpAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CProSpyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	int index = m_opList.GetNextSelectedItem(pos);
	if(index<0)
		return;
	OpItem * pItem = (OpItem*)m_opList.GetItemData(index);
	bool bModified = false;
	switch(pItem->type)
	{
	case OP_RECORD:
		{
			CRecordEditDlg dlg(pItem);
			bModified = (dlg.DoModal() == IDOK);
			break;
		}
	case OP_LCLICK:
	case OP_RCLICK:
	case OP_DBCICK:
		{
		    CMouseEditDlg dlg(pItem);
			bModified = (dlg.DoModal() == IDOK);
			break;
		}
	case OP_KEY_INPUT:
		{
			CKeyEditDlg dlg(pItem);
			bModified = (dlg.DoModal() == IDOK);
			break;
		}
		break;
	}
	UpdateItem(index,pItem);
	if (bModified)
	{
		m_oProj.SetModified();
	}
}

void CProSpyDlg::UpdateItem(int index, LPITEM pItem)
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
	m_opList.SetItemText(index,3,pItem->szNote);
}

void CProSpyDlg::OnContextDelete()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_opList.GetFirstSelectedItemPosition(); 
	int maxIndex  = 0;
	while(pos != NULL)
	{
		int index = m_opList.GetNextSelectedItem(pos); 
		maxIndex = index;
		OpItem * pItem = (OpItem*)m_opList.GetItemData(index);
		m_oProj.DeleteItem(pItem); 
	}
	ShowProject();
	m_opList.SetItemState(maxIndex, LVIS_SELECTED, LVIS_SELECTED);
}

void CProSpyDlg::OnContextMoveup()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	} 
	int firstIndex = m_opList.GetNextSelectedItem(pos); 
	int nCount = 1; 
	while (pos != NULL)
	{
		int index = m_opList.GetNextSelectedItem(pos);
		nCount++;
	} 
	if (m_oProj.MoveUp(firstIndex, nCount))
	{
		ShowProject();
		for (int i = 0; i < nCount; i++)
		{
			m_opList.SetItemState(firstIndex + i - 1, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void CProSpyDlg::OnContextMovedown()
{
	// TODO: 在此添加命令处理程序代码 
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int firstIndex = m_opList.GetNextSelectedItem(pos);
	int nCount = 1;
	while (pos != NULL)
	{
		int index = m_opList.GetNextSelectedItem(pos);
		nCount++;
	}
	if (m_oProj.MoveDown(firstIndex, nCount))
	{
		ShowProject();
		for (int i = 0; i < nCount; i++)
		{
			m_opList.SetItemState(firstIndex + i + 1, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

LRESULT CProSpyDlg::OnThreadStop( WPARAM wparam,LPARAM lparam )
{ 
	ShowWindow(SW_NORMAL);
	GetDlgItem(ID_BTN_START)->EnableWindow(TRUE);
	CString str;
	str.Format(_T("Run %d times successfully"), (int)wparam);
	if (m_pThread != nullptr)
	{
		m_pThread.reset(); 
	}
	AfxMessageBox(str);
	return TRUE;
}

void CProSpyDlg::OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
	else if (pLVKeyDow->wVKey == 'C' && (GetAsyncKeyState(VK_CONTROL)))
	{
		OnContextCopy();
	}
	else if (pLVKeyDow->wVKey == 'V' && (GetAsyncKeyState(VK_CONTROL)))
	{
		OnContextPaste();
	}
	*pResult = 0;
}

void CProSpyDlg::AddMouseOperation( int x, int y, bool showDlg)
{
	OpItem *pItem = new OpItem;
	pItem->type = OP_LCLICK; //默认左击 
	pItem->detail.pos.x = x;
	pItem->detail.pos.y = y;
	pItem->dwTimeSpan = DEFAULT_INTERVAL;
	if (showDlg)
	{
		CMouseEditDlg dlg(pItem);
		if(dlg.DoModal()!=IDOK)
		{
			delete pItem;
			return;
		} 
	}
	AddItem(pItem);

}

void CProSpyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_oProj.IsModified())
	{
		int nRet = AfxMessageBox(_T("Project is modified,Save it?"),MB_YESNOCANCEL);
		if (nRet == IDYES  && !m_oProj.Save())
		{
			AfxMessageBox(_T("Save Project failed"));
			return;
		}
		else if (nRet == IDCANCEL)
		{
			return;
		}

	}
	m_oProj.Destroy();
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
		WORD mask = HIWORD(dwKey);
		if (mask == CONTROL_KEY_MASK)
		{
			switch (LOWORD(dwKey))
			{
			case VK_CONTROL:
				str.Append(_T("[Ctrl]"));
				break;
			case VK_SHIFT:
				str.Append(_T("[Shift]")); 
				break;
			case VK_MENU:
				str.Append(_T("[Alt]"));
				break;
			case VK_LWIN: 
				str.Append(_T("[Win]"));
				break; 
			}
		}
		else if (mask == VIRTUAL_KEY_MASK)
		{
			switch (LOWORD(dwKey))
			{
			case VK_DELETE:
				str.Append(_T("[Delete]"));
				break;
			case VK_SNAPSHOT:
				str.Append(_T("[PrintScreen]"));
				break;
			case VK_RETURN:
				str.Append(_T("[Enter]"));
				break;
			case VK_BACK:
				str.Append(_T("[Backspace]"));
				break;
			case VK_PRIOR:
				str.Append(_T("[Page Up]"));
				break;
			case VK_NEXT:
				str.Append(_T("[Page Down]"));
				break;
			}
		}
		else
		{
			str.AppendChar((WORD)input.dwKey[i]);
		}
	}
	return str;
}

void CProSpyDlg::OnContextAddremark()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	int index = m_opList.GetNextSelectedItem(pos);
	if(index<0)
		return;
	OpItem * pItem = (OpItem*)m_opList.GetItemData(index); 
	CRemarkDlg dlg;
	dlg.m_strRemark = pItem->szNote;
	INT_PTR nRet = dlg.DoModal();
	if (nRet == IDOK)
	{
		wcscpy_s(pItem->szNote,64,dlg.m_strRemark.GetBuffer());
	}
	UpdateItem(index,pItem);
}


void CProSpyDlg::OnContextCopy()
{
	// TODO:  在此添加命令处理程序代码
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int firstIndex = m_opList.GetNextSelectedItem(pos); 
	m_oProj.Copy(firstIndex, m_opList.GetSelectedCount());
}

void CProSpyDlg::OnContextPaste()
{
	// TODO:  在此添加命令处理程序代码
	int index = GetLastSelectedIndex();

	int nSelect = (index < 0) ? m_opList.GetItemCount() : index+1;
	int nCount = m_oProj.Paste(index);
	if (nCount <= 0)
	{
		return;
	}
	ShowProject();
	for (int i = 0; i < nCount; i++)
	{
		m_opList.SetItemState(nSelect + i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CProSpyDlg::UpdateTitle()
{ 
	CString	strTitle;
	strTitle.Format(_T("ProcSpy----%s"), m_oProj.GetFilePath());
	SetWindowText(strTitle);
}

int CProSpyDlg::GetLastSelectedIndex()
{
	POSITION pos = m_opList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return -1;
	}
	int index = m_opList.GetNextSelectedItem(pos); 
	return index+m_opList.GetSelectedCount()-1;
}

void CProSpyDlg::AddItem(OpItem * pItem)
{
	int index = GetLastSelectedIndex();
	m_oProj.InsertItem(pItem, index);
	ShowProject();
	int nSelect = index < 0 ? m_opList.GetItemCount() - 1 : index + 1;
	m_opList.SetItemState(nSelect, LVIS_SELECTED, LVIS_SELECTED);
}