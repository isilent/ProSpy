#include "stdafx.h"
#include "RegEdit.h"


#pragma comment(lib,"Advapi32.lib")

CRegEdit::CRegEdit()
{
}


CRegEdit::~CRegEdit()
{
}

bool CRegEdit::DeleteKey(HKEY hKey, LPCTSTR szSubKey,bool bWin32)
{ 
	int nRet = RegDeleteKeyEx(hKey, szSubKey,(bWin32)?KEY_WOW64_32KEY:KEY_WOW64_64KEY, 0);
	return ERROR_FILE_NOT_FOUND == nRet || nRet == ERROR_SUCCESS;
}

bool CRegEdit::IsKeyExist(HKEY hKey, LPCTSTR szSubKey)
{
	HKEY hSubKey; 
	int nRet = RegOpenKeyEx(hKey, szSubKey, 0, KEY_QUERY_VALUE, &hSubKey);
	if (nRet != ERROR_SUCCESS)
		return false;
	RegCloseKey(hSubKey);
	return true;
}

bool CRegEdit::ReadValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, CString &strValue)
{
	HKEY hSubKey;
	int nRet = RegOpenKeyEx(hKey, szSubKey, 0, KEY_QUERY_VALUE, &hSubKey);
	if (nRet != ERROR_SUCCESS)
		return false;
	TCHAR buff[1024] = { 0 };
	DWORD dwSize = sizeof(TCHAR)*(1024-1);
	DWORD dwType = REG_SZ;
	nRet = RegQueryValueEx(hSubKey, szValueName, 0, &dwType,(LPBYTE)buff,&dwSize);
	RegCloseKey(hSubKey);
	if (nRet != ERROR_SUCCESS || dwType != REG_SZ)
		return false;
	strValue = buff;
	return true;
}

bool CRegEdit::ReadValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, DWORD &dwValue)
{ 
	HKEY hSubKey;
	int nRet = RegOpenKeyEx(hKey, szSubKey, 0, KEY_QUERY_VALUE, &hSubKey);
	if (nRet != ERROR_SUCCESS)
		return false;
	DWORD val = 0;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwType = REG_DWORD;
	nRet = RegQueryValueEx(hSubKey, szValueName, 0, &dwType, (LPBYTE)&val, &dwSize);
	RegCloseKey(hSubKey);
	if (nRet != ERROR_SUCCESS || dwType != REG_DWORD)
		return false;
	dwValue = val;
	return true;
}

bool CRegEdit::CreateKey(HKEY hKey, LPCTSTR szSubKey, HKEY &newkey)
{  
	int nRet = RegCreateKeyEx(hKey, szSubKey, NULL, NULL, 0, KEY_WRITE, NULL, &newkey, NULL);
	return (nRet == ERROR_SUCCESS);
}

bool CRegEdit::WriteValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, const DWORD &dwValue)
{
	HKEY hSubKey;
	if (!CreateKey(hKey,szSubKey,hSubKey))
		return false; 
	int nRet = RegSetValueEx(hSubKey, szValueName, 0, REG_DWORD, (const LPBYTE)&dwValue, sizeof(DWORD));
	RegCloseKey(hSubKey);
	if (nRet != ERROR_SUCCESS)
		return false; 
	return true;
}

bool CRegEdit::WriteValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, CString &strValue)
{
	HKEY hSubKey;
	if (!CreateKey(hKey, szSubKey, hSubKey))
		return false;
	DWORD nLen = (strValue.GetLength()+1)*sizeof(TCHAR);
	LPBYTE pByte = new BYTE[nLen];
	if (pByte == nullptr)
		return false;
	memset(pByte, 0, nLen);
	memcpy_s(pByte, nLen, (LPBYTE)strValue.GetBuffer(), nLen - sizeof(TCHAR));
	int nRet = RegSetValueEx(hSubKey, szValueName, 0, REG_SZ, (const LPBYTE)strValue.GetBuffer(), nLen);
	RegCloseKey(hSubKey);
	delete[] pByte;
	if (nRet != ERROR_SUCCESS)
		return false;
	return true; 
}
