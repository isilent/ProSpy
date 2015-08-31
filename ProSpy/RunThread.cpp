#include "StdAfx.h"
#include <tlhelp32.h>
#include "RunThread.h"

CRunThread::CRunThread(const HWND &hwnd, const OpItemList &itemList)
{ 
	m_bRunning = false;
	m_ItemList = itemList;  
	m_hMainWnd = hwnd;
}

CRunThread::~CRunThread(void)
{
	if (m_pThread != nullptr)
	{
		m_pThread->join();
		m_pThread = nullptr;
	}
}

void CRunThread::Start(int nRunCount)
{
	m_nMaxCount = nRunCount;
	m_bRunning = true; 
	for (auto ptr : m_ItemList)
	{
		if (ptr->type == OP_RECORD && (ptr->detail.record.dwMask & (RECORD_CPU_USAGE)))
		{
			m_calc.AddProcessID(ptr->detail.record.dwProcessID);
		}
	}
	m_calc.Start();

	m_pThread = make_shared<thread>(bind(&CRunThread::Run, this));
}

void CRunThread::Stop()
{
	m_bRunning = false;
}

void CRunThread::Run()
{ 
	int nCount = 0;
	while (m_bRunning && (m_nMaxCount <= 0 ||nCount < m_nMaxCount))
	{
		for (auto &pItem:m_ItemList)
		{
			if (!m_bRunning)
			{
				break;
			}
			switch (pItem->type)
			{
			case OP_RECORD:
				Record(pItem->detail.record);
				break;
			case OP_LCLICK:
			case OP_RCLICK:
			case OP_DBCICK:
				MouseClick(pItem);
				break;
			case OP_KEY_INPUT:
				KeyInput(pItem->detail.keyinput);
				break;
			default:
				break;
			}
			if (!TrySleep(pItem->dwTimeSpan))
				break;
		}
		nCount++;
	}
	m_calc.Stop();
	PostMessage(m_hMainWnd, WM_THREAD_STOP, (WPARAM)nCount, 0); 
}

void CRunThread::Record( const OpRecord &op )
{
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, op.dwProcessID );
	if (NULL == hProcess )
	{
		CString str;
		str.Format(_T("[ProSpy] OpenProcess error PID:%d"),op.dwProcessID);
		OutputDebugString(str);
		return;
	}
	CStdioFile  file;
	if (!file.Open(op.szFilePath, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate | CFile::typeText))
	{
		OutputDebugString(_T("[ProSpy] Open record file error"));
		return;
	}

	if (file.GetLength() == 0)
	{
		CString strHeader = _T("Process,Time,");
		for (int i=0;i<sizeof(c_recordHeaders)/sizeof(c_recordHeaders[0]); i++)
		{
			if ((op.dwMask & c_recordHeaders[i].dwMask)!=0)
			{
				strHeader.AppendFormat(_T("%s,"),c_recordHeaders[i].szDesc);
			}
		}
		strHeader.Append(_T("\n"));
		file.WriteString(strHeader);
	}
	else
	{
		file.SeekToEnd();
	} 

	CString strContent;
	strContent.AppendFormat(_T("%s,%s,"),op.szProcessName,Utility::GetLocalTime().c_str());
	if (op.dwMask & RECORD_PID)
	{
		strContent.AppendFormat(_T("%u,"),op.dwProcessID);
	} 
	if (op.dwMask & (RECORD_CPU_USAGE | RECORD_KERNEL_CPU))
	{
		float kernel, total;
		m_calc.GetCPURatio(op.dwProcessID, kernel, total);
		if (op.dwMask & (RECORD_CPU_USAGE))
			strContent.AppendFormat(_T("%.2f,"), total);
		if (op.dwMask & (RECORD_KERNEL_CPU)) 
			strContent.AppendFormat(_T("%.2f,"), kernel);
	} 

	PROCESS_MEMORY_COUNTERS memInfo;
	GetProcessMemoryInfo(hProcess,&memInfo,sizeof(PROCESS_MEMORY_COUNTERS));

	if (op.dwMask & RECORD_WORK_SET)
	{
		strContent.AppendFormat(_T("%u,"),memInfo.WorkingSetSize/1024);
	}

	if (op.dwMask & RECORD_VIRTUAL_MEM)
	{
		strContent.AppendFormat(_T("%u,"),memInfo.PagefileUsage/1024);
	}

	if (op.dwMask & RECORD_PAGE_FAULT)
	{
		strContent.AppendFormat(_T("%u,"),memInfo.PageFaultCount);
	}

	if (op.dwMask & RECORD_HANDLE_COUNT)
	{
		DWORD dwCount;
		GetProcessHandleCount(hProcess,&dwCount);
		strContent.AppendFormat(_T("%u,"),dwCount);
	}
	if (op.dwMask & RECORD_THREAD_COUNT)
	{
		int nCnt = 0;
		do 
		{	
			HANDLE hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0); 
			if( hThreadSnap == INVALID_HANDLE_VALUE ) 
				break;

			PROCESSENTRY32  pe32;  
			pe32.dwSize = sizeof(PROCESSENTRY32);  
			if(!Process32First( hThreadSnap, &pe32 )) 
			{ 
				CloseHandle( hThreadSnap );     
				break;
			} 
			do 
			{ 
				if (pe32.th32ProcessID == op.dwProcessID)
				{
					nCnt = pe32.cntThreads;
				}
			} while( Process32Next(hThreadSnap, &pe32));  
			CloseHandle(hThreadSnap); 
		} while (false);
		strContent.AppendFormat(_T("%u,"),nCnt);
	}
	if (op.dwMask & RECORD_USER_OBJECT)
	{
		DWORD dwCnt = GetGuiResources(hProcess,GR_USEROBJECTS);
		strContent.AppendFormat(_T("%u,"),dwCnt);
	}
	if (op.dwMask & RECORD_GDI_COUNT)
	{
		DWORD dwCnt = GetGuiResources(hProcess,GR_GDIOBJECTS);
		strContent.AppendFormat(_T("%u,"),dwCnt);
	}
	strContent.Append(_T("\n"));
	file.WriteString(strContent);
	file.Close();
	CloseHandle( hProcess );
}
 
void CRunThread::MouseClick(LPITEM pItem)
{
	SetCursorPos(pItem->detail.pos.x,pItem->detail.pos.y);
	DWORD keydown = 0,keyup = 0; 
	 
	if(pItem->type == OP_LCLICK || pItem->type == OP_DBCICK )
	{
		keydown = MOUSEEVENTF_LEFTDOWN;
		keyup = MOUSEEVENTF_LEFTUP; 
	} 
	else if(pItem->type == OP_RCLICK)
	{
		keydown = MOUSEEVENTF_RIGHTDOWN;
		keyup = MOUSEEVENTF_RIGHTUP;
	}
	mouse_event(keydown,0,0,0,0);
	mouse_event(keyup,0,0,0,0);
	if (pItem->type == OP_DBCICK)
	{
		mouse_event(keydown,0,0,0,0);
		mouse_event(keyup,0,0,0,0);
	}
}

void CRunThread::KeyInput(const OpKeyInput &key)
{
	vector<INPUT> inputAry;
	vector<INPUT> releaseAry;
	for (int i=0;i<KEYINPUT_MAX;i++)
	{
		DWORD dwKey = key.dwKey[i];
		if (dwKey == 0)
		{
			break;
		}
		INPUT keydown;
		memset(&keydown,0,sizeof(INPUT));
		keydown.type = INPUT_KEYBOARD;

		INPUT keyup;
		memset(&keyup,0,sizeof(INPUT));
		keyup.type = INPUT_KEYBOARD;
		keyup.ki.dwFlags = KEYEVENTF_KEYUP;

		WORD mask = HIWORD(dwKey);
		if (mask == CONTROL_KEY_MASK ) //控制键
		{ 
			keydown.ki.wVk = LOWORD(dwKey); 
			keyup.ki.wVk= keydown.ki.wVk;
			inputAry.push_back(keydown);
			releaseAry.push_back(keyup);
		}  
		else if (mask == VIRTUAL_KEY_MASK) //特殊键
		{
			keydown.ki.wVk = LOWORD(dwKey); 
			keyup.ki.wVk = keydown.ki.wVk;
			inputAry.push_back(keydown);
			inputAry.push_back(keyup);
		}
		else
		{
			keydown.ki.wVk = VkKeyScan(LOWORD(dwKey)); 
			keyup.ki.wVk = keydown.ki.wVk;
			inputAry.push_back(keydown);
			inputAry.push_back(keyup);
		}
	}
	if (!inputAry.empty())
	{
		SendInput((UINT)inputAry.size(),&inputAry[0],sizeof(INPUT));
	} 
	if (!releaseAry.empty())
	{
		SendInput((UINT)releaseAry.size(),&releaseAry[0],sizeof(INPUT));
	} 
}

bool CRunThread::TrySleep(DWORD dwTimeSpan)
{
	//长时间的sleep分成多次，加快终止操作时的响应速度
	int count = dwTimeSpan / 200;
	int remainder = dwTimeSpan % 200;
	Sleep(remainder);
	while(m_bRunning && count-->0)
	{
		Sleep(200); 
	}
	return m_bRunning;
}
