#pragma once
#include <thread>
#include <memory>
#include "Define.h"
#include "CPUCalculator.h"

using namespace std;

class CRunThread
{
public:
	CRunThread(const HWND &hwnd, const OpItemList &itemList);
	~CRunThread(void);

	void Start(int nRunCount);

	void Stop();

private:
	void Run();
	void Record(const OpRecord &op);
	void MouseClick(LPITEM pItem);
	void KeyInput(const OpKeyInput &key);
	bool TrySleep(DWORD dwTimeSpan);

	CPUCalculator m_calc;
	OpItemList m_ItemList;
	volatile bool m_bRunning =false;
	int  m_nMaxCount;
	HWND m_hMainWnd;
	shared_ptr<thread> m_pThread = nullptr;
};
