#include "stdafx.h"

#include <algorithm>
#include <stdlib.h>

#include "Utility.h"
 
#ifdef _UNICODE
#define tsprintf swprintf_s
#else
#define tsprintf sprintf_s
#endif


TString Utility::GetLocalTime()
{
	SYSTEMTIME  st;
	::GetLocalTime(&st);
	TCHAR buff[128] = {0};
	tsprintf(buff,128,_T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	return TString(buff);
}

TString Utility::GetModuleDir( LPCTSTR szModule )
{
	HMODULE hMod= GetModuleHandle(szModule);
	if (hMod == NULL)
	{
		return TString();
	}
	TCHAR filepath[MAX_PATH] = {0};
	GetModuleFileName(hMod,filepath,MAX_PATH);
	TString str(filepath);
	size_t index = str.find_last_of(_T('\\'));
	if (index != TString::npos)
	{
		str = str.substr(0,index);
	}
	return str;
}