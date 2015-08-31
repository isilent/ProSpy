#pragma once
#include <unordered_map>
#include <mutex>
#include <memory>

using namespace std;

#define UPDATE_INTERVAL 1000

struct CPURatio
{
	ULONGLONG lastTotalTime;
	ULONGLONG lastKernelTime;
	float totalRatio;
	float kernelRatio;
	CPURatio()
	{
		lastKernelTime = lastTotalTime = 0;
		totalRatio = kernelRatio = 0;
	}
};

class CPUCalculator
{
public:
	CPUCalculator();
	~CPUCalculator();
	void AddProcessID(DWORD pid);
	void Start();
	void Update();
	void Stop();
	bool GetCPURatio(DWORD pid, float &kernel, float &total);
private:
	ULONGLONG filetime_2_ull(const FILETIME &ftime);
	HANDLE m_hTimerQueue;
	HANDLE m_hTimer;
	mutex m_mutex;
	DWORD m_dwCycleTime = UPDATE_INTERVAL;
	unordered_map<DWORD, shared_ptr<CPURatio>> m_mapRatio;
};

