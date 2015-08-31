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
		ULONGLONG kernel = filetime_2_ull(ft3);
		ULONGLONG total = kernel + filetime_2_ull(ft4);

		if (item.second->lastTotalTime !=0)
		{ 
			DWORD microTime = DWORD((total - item.second->lastTotalTime) / 10);
			item.second->totalRatio = microTime*100.0f / (m_dwCycleTime*1000);
		}
		if (item.second->lastKernelTime != 0)
		{
			DWORD microTime = DWORD((kernel - item.second->lastKernelTime) / 10);
			item.second->kernelRatio = microTime*100.0f / (m_dwCycleTime*1000);
		}
		item.second->lastTotalTime = total;
		item.second->lastKernelTime = kernel;
	}
}

void CPUCalculator::Stop()
{
	DeleteTimerQueue(m_hTimerQueue);
}

bool CPUCalculator::GetCPURatio(DWORD pid, float &kernel, float &total)
{ 
	lock_guard<mutex> lock(m_mutex);
	auto iter = m_mapRatio.find(pid);
	if (iter == m_mapRatio.end())
	{
		return false;
	}
	kernel = iter->second->kernelRatio;
	total = iter->second->totalRatio;
	return true;
}
