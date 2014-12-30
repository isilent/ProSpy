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
 * @description 工具类     
 * @date 2014/7/21
 ************************************************************************/
class Utility
{
public:
	static TString GetLocalTime();

	/*返回模块所在路径，szModule为NULL时返回exe所在路径*/
	static TString GetModuleDir(LPCTSTR szModule);
}; 
