#pragma once 

class CRegEdit
{
public:
	CRegEdit();
	~CRegEdit();

	static bool DeleteKey(HKEY hKey, LPCTSTR szSubKey, bool bWin32);

	static bool IsKeyExist(HKEY hKey, LPCTSTR szSubKey);

	static bool ReadValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, CString &strValue);

	static bool ReadValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, DWORD &dwValue);
	  
	static bool CreateKey(HKEY hKey, LPCTSTR szSubKey, HKEY &newkey);

	static bool WriteValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, const DWORD &dwValue);

	static bool WriteValue(HKEY hKey, LPCTSTR szSubKey, LPCTSTR szValueName, CString &strValue);
};

