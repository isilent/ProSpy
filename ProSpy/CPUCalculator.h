#pragma once
#include <unordered_map>
#include <mutex>
#include <memory>

using namespace std;

struct CPURatio
{
	ULONGLONG lastCPUTime;
	float fRatio;
};

class CCPUCalculator
{
public:
	CCPUCalculator();
	~CCPUCalculator();
	void AddProcessID(DWORD pid);
	void Start();
	void Update();
	void Stop();
	float GetCPURatio(DWORD pid);
private:
	ULONGLONG filetime_2_ull(const FILETIME &ftime);
	HANDLE m_hTimerQueue;
	HANDLE m_hTimer;
	mutex m_mutex;
	DWORD m_dwProcessorNum = 1;
	unordered_map<DWORD, shared_ptr<CPURatio>> m_mapRatio;
};

