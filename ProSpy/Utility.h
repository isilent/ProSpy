#pragma once
 
#include <Windows.h>
#include <string>
using namespace std; 

#ifdef _UNICODE
#define TString std::wstring
#else
#define TString std::string
#endif

 /************************************************************************
 * @description ������     
 * @date 2014/7/21
 ************************************************************************/
class Utility
{
public:
	static TString GetLocalTime();

	/*����ģ������·����szModuleΪNULLʱ����exe����·��*/
	static TString GetModuleDir(LPCTSTR szModule);
}; 
