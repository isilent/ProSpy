#pragma once
#include "Define.h"

class CRunThread
{
	friend unsigned int __stdcall Run(PVOID pParam);
public:
	CRunThread(const HWND &hwnd, const list<OpItem*> &itemList);
	~CRunThread(void);

	void Start(int nRunCount);

	void Stop();
 
protected:
	void Record(const OpRecord &op);
	void MouseClick(const OpItem* pItem);
	void KeyInput(const OpItem* pItem);
	list<OpItem*> m_ItemList;
	volatile bool m_bRunning;
	int  m_nCount;
	int  m_nMaxCount;
	HWND m_hMainWnd;
private:
	static ULONGLONG filetime_2_ull(const FILETIME* ftime);
 
	HANDLE m_hThread;
	CStdioFile m_file;
};
