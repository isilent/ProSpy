#pragma once

#include "Define.h"

#define DEFAULT_FILENAME _T("default.ps")
const DWORD CURRENT_FILE_VERSION = 1;

//实现工程文件的读写
class ProjectFile
{
public:
	ProjectFile();

	void AddItem(OpItem *pItem);

	void DeleteItem(OpItem *pItem);

	void MoveUp(OpItem *pItem);

	void MoveDown(OpItem *pItem);

	list<OpItem*> &GetItemList(){return m_ItemList;}

	bool Open(LPCTSTR filename = NULL);

	bool Save(LPCTSTR filename = NULL);

	bool IsEmpty(){return m_ItemList.empty();}

	void Discard();

	bool IsModified(){return m_bModified;}
public:
	~ProjectFile(void);
private:
	void DeleteItems();
	bool    m_bModified;
	CString m_strFileName;
	list<OpItem*> m_ItemList;
};
