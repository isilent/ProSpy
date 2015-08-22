#include "stdafx.h"
#include "CPUCalculator.h" 
 

VOID CALLBACK TimerCallback(PVOID pParam, BOOLEAN TimerOrWaitFired)
{
	CPUCalculator*pCalc = (CPUCalculator*)pParam;

	pCalc->Update();
}

CPUCalculator::CPUCalculator()
{
}


CPUCalculator::~CPUCalculator()
{
}

void CPUCalculator::AddProcessID(DWORD pid)
{
	auto ptr =make_shared<CPURatio>();
	ptr->fRatio = 0;
	ptr->lastCPUTime = 0;
	m_mapRatio[pid] = ptr;
}

void CPUCalculator::Start()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	m_dwCycleTime = si.dwNumberOfProcessors * UPDATE_INTERVAL;
	m_hTimerQueue = CreateTimerQueue();  
	CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, TimerCallback, this, UPDATE_INTERVAL, UPDATE_INTERVAL, 0);
}

ULONGLONG CPUCalculator::filetime_2_ull(const FILETIME &ftime)
{
	LARGE_INTEGER li;
	li.LowPart = ftime.dwLowDateTime;
	li.HighPart = ftime.dwHighDateTime;
	return li.QuadPart;
}

void CPUCalculator::Update()
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
			DWORD msTime = DWORD((total - item.second->lastCPUTime) / (UPDATE_INTERVAL*10));
			item.second->fRatio = msTime*100.0f / m_dwCycleTime	; 
		} 
		item.second->lastCPUTime = total;
	}
}

void CPUCalculator::Stop()
{
	DeleteTimerQueue(m_hTimerQueue);
}

float CPUCalculator::GetCPURatio(DWORD pid)
{
	lock_guard<mutex> lock(m_mutex);
	if (m_mapRatio.find(pid) == m_mapRatio.end())
	{
		return 0;
	}
	return m_mapRatio[pid]->fRatio;
}
