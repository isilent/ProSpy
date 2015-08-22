#include "stdafx.h"
#include "CPUCalculator.h" 

static DWORD dwLastTick = 0;

VOID CALLBACK TimerCallback(PVOID pParam, BOOLEAN TimerOrWaitFired)
{
	CCPUCalculator*pCalc = (CCPUCalculator*)pParam;

	pCalc->Update();
}

CCPUCalculator::CCPUCalculator()
{
}


CCPUCalculator::~CCPUCalculator()
{
}

void CCPUCalculator::AddProcessID(DWORD pid)
{
	auto ptr =make_shared<CPURatio>();
	ptr->fRatio = 0;
	ptr->lastCPUTime = 0;
	m_mapRatio[pid] = ptr;
}

void CCPUCalculator::Start()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_dwProcessorNum = si.dwNumberOfProcessors;
	m_hTimerQueue = CreateTimerQueue(); 
	dwLastTick = GetTickCount();
	CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, TimerCallback, this, 1000, 1000,0);
}

ULONGLONG CCPUCalculator::filetime_2_ull(const FILETIME &ftime)
{
	LARGE_INTEGER li;
	li.LowPart = ftime.dwLowDateTime;
	li.HighPart = ftime.dwHighDateTime;
	return li.QuadPart;
}

void CCPUCalculator::Update()
{ 
	lock_guard<mutex> lock(m_mutex);
	for (auto item:m_mapRatio)
	{ 
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,
		FALSE, item.first);
		if (NULL == hProcess)
		{ 
			OutputDebugString(_T("[ProcSpy] OpenProcess error PID:%d"));
			continue;;
		}
		FILETIME ft1, ft2, ft3, ft4;
		GetProcessTimes(hProcess, &ft1, &ft2, &ft3, &ft4);
		ULONGLONG total = filetime_2_ull(ft3)+filetime_2_ull(ft4);
		DWORD dw = GetTickCount();
		if (item.second->lastCPUTime !=0)
		{
			DWORD interval = dw - dwLastTick;
			DWORD msTime = (total - item.second->lastCPUTime) / 10000;
			item.second->fRatio = msTime*100.0f/ (interval *m_dwProcessorNum);
			CString str;
			str.Format(_T("%d, %d, %f"), msTime,interval,item.second->fRatio);
			OutputDebugString(str);
		}
		dwLastTick = dw;
		item.second->lastCPUTime = total;
	}
}

void CCPUCalculator::Stop()
{
	DeleteTimerQueue(m_hTimerQueue);
}

float CCPUCalculator::GetCPURatio(DWORD pid)
{
	lock_guard<mutex> lock(m_mutex);
	if (m_mapRatio.find(pid)!= m_mapRatio.end())
	{
		return m_mapRatio[pid]->fRatio;
	}
	return 0;
}
