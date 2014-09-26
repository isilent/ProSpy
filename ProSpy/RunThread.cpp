#include "StdAfx.h"
#include <tlhelp32.h>
#include "RunThread.h"

CRunThread::CRunThread(const HWND &hwnd, const list<OpItem*> &itemList)
{
	m_hThread = 0;
	m_bRunning = false;
	m_ItemList = itemList; 
	m_nCount = 0;
	m_hMainWnd = hwnd;
}

CRunThread::~CRunThread(void)
{
}

unsigned int __stdcall Run( PVOID pParam )
{
	CRunThread *pThread = (CRunThread*)pParam;
	pThread->m_nCount = 0;
	while(pThread->m_bRunning 
		&& (pThread->m_nMaxCount<=0 ||pThread->m_nCount<pThread->m_nMaxCount))
	{
		for (list<OpItem*>::iterator iT =pThread->m_ItemList.begin();
			iT != pThread->m_ItemList.end() && pThread->m_bRunning; ++iT)
		{
			switch((*iT)->type)
			{
			case OT_RECORD:
				pThread->Record((*iT)->detail.record);
				break;
			case OT_LCLICK: 
			case OT_RCLICK: 
			case OT_DBCICK:
				pThread->MouseClick((*iT));
				break;
			case OT_KEY_INPUT:
				break;
			}
			Sleep((*iT)->dwTimeSpan *10);
		} 
		pThread->m_nCount++;
	}
	PostMessage(pThread->m_hMainWnd,WM_THREAD_STOP,(WPARAM)pThread->m_nCount,0);
	return 0;
}

void CRunThread::Start(int nRunCount)
{
	m_nMaxCount = nRunCount;
	m_bRunning = true;
	m_hThread = (HANDLE)_beginthreadex(NULL,0,Run,this,0,NULL);
}

void CRunThread::Stop()
{
	m_bRunning = false;
	WaitForSingleObject(m_hThread,INFINITE); 
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
	if (!m_file.Open(op.szFilePath,CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate| CFile::typeText))
	{
		OutputDebugString(_T("[ProSpy] Open record file error"));
		return;
	}

	if (m_file.GetLength() == 0)
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
		m_file.WriteString(strHeader); 
	}
	else
	{
		m_file.SeekToEnd();
	} 

	CString strContent;
	strContent.AppendFormat(_T("%s,%s,"),op.szProcessName,Utility::GetLocalTime().c_str());
	if (op.dwMask & RECORD_PID)
	{
		strContent.AppendFormat(_T("%u,"),op.dwProcessID);
	} 
	if (op.dwMask & (RECORD_CPU_TIME_KERNEL|RECORD_CPU_TIME_USER))
	{ 
		FILETIME ft1,ft2,ft3,ft4;
		GetProcessTimes(hProcess,&ft1,&ft2,&ft3,&ft4);
		if (op.dwMask & RECORD_CPU_TIME_KERNEL)
		{
			ULONGLONG kernel = filetime_2_ull(&ft3); 
			strContent.AppendFormat(_T("%I64u,"),kernel/10000); //显示时单位取ms
		}
		if (op.dwMask & RECORD_CPU_TIME_USER)
		{
			ULONGLONG user = filetime_2_ull(&ft4); 
			strContent.AppendFormat(_T("%I64u,"),user/10000);
		} 
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
	m_file.WriteString(strContent);
	m_file.Close();
	CloseHandle( hProcess );
}

ULONGLONG CRunThread::filetime_2_ull( const FILETIME* ftime )
{
	LARGE_INTEGER li;   
	li.LowPart = ftime->dwLowDateTime;   
	li.HighPart = ftime->dwHighDateTime;   
	return li.QuadPart;   
}
 
void CRunThread::MouseClick( const OpItem* pItem)
{
	SetCursorPos(pItem->detail.pos.x,pItem->detail.pos.y);
	DWORD keydow = 0,keyup = 0; 
	 
	if(pItem->type == OT_LCLICK || pItem->type == OT_DBCICK )
	{
		keydow = MOUSEEVENTF_LEFTDOWN;
		keyup = MOUSEEVENTF_LEFTUP; 
	} 
	else if(pItem->type == OT_RCLICK)
	{
		keydow = MOUSEEVENTF_RIGHTDOWN;
		keyup = MOUSEEVENTF_RIGHTUP;
	}
	mouse_event(keydow,0,0,0,0);
	mouse_event(keyup,0,0,0,0);
	if (pItem->type == OT_DBCICK)
	{
		mouse_event(keydow,0,0,0,0);
		mouse_event(keyup,0,0,0,0);
	}
}
