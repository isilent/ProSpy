#pragma once

#include "Define.h"

#define DEFAULT_FILENAME _T("default.ps")
const DWORD CURRENT_FILE_VERSION = 1;

//实现工程文件的读写
class ProjectFile
{
public:
	ProjectFile();
	~ProjectFile(void);

	void InsertItem(LPITEM pItem,int index);

	void DeleteItem(LPITEM pItem);

	bool MoveUp(int nFirstIndex, int nCount);

	bool MoveDown(int nFirstIndex, int nCount); 

	void Copy(int nFirstIndex, int nCount);
	
	int Paste(int nIndex);

	OpItemList &GetItemList(){ return m_ItemList; }

	bool Open(LPCTSTR filename = NULL);

	bool Save(LPCTSTR filename = NULL);

	bool IsEmpty(){return m_ItemList.empty();}

	void Destroy();

	bool IsModified(){return m_bModified;}
	void SetModified(){ m_bModified = true; }
	CString GetFilePath(){ return m_strFileName; } 
private:
	void DeleteItems();
	void DestroyCopyList();
	bool    m_bModified;
	CString m_strFileName;
	OpItemList m_ItemList;
	OpItemList m_copyList;
};
