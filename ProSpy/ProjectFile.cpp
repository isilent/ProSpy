#include "StdAfx.h"
#include "ProjectFile.h"
#include "RegEdit.h"

ProjectFile::ProjectFile( )
{
	m_bModified = false;
	m_ItemList.reserve(20);
}

ProjectFile::~ProjectFile(void)
{
	Destroy();
}

bool ProjectFile::Open( LPCTSTR filename )
{
	Destroy();
	if (filename != NULL)
	{
		m_strFileName = filename;
	}
	else if(m_strFileName.IsEmpty())
	{
		m_strFileName.Format(_T("%s\\%s"),Utility::GetModuleDir(NULL).c_str(),DEFAULT_FILENAME);
	}
	
	CFile file;
	if(!file.Open(m_strFileName,CFile::modeRead|CFile::typeBinary))
		return false;
	
	DWORD dwVer;
	if (file.Read(&dwVer,sizeof(DWORD)) != sizeof(DWORD) || dwVer != CURRENT_FILE_VERSION)
	{
		OutputDebugString(_T("[ProSpy] read project file error"));
		return false;
	}
	OpItem item;
	int nSize = sizeof(OpItem);
	while(file.Read(&item,nSize) == nSize)
	{
		OpItem *pItem = new OpItem;
		memcpy_s(pItem,nSize,&item,nSize);
		m_ItemList.push_back(pItem);
	}
	file.Close();
	CRegEdit::WriteValue(HKEY_LOCAL_MACHINE, REG_PATH_APP, REG_ITEM_RUNFILE, m_strFileName);
	return true;
}

bool ProjectFile::Save( LPCTSTR filename /*= NULL*/ )
{
	if (filename != NULL)
	{
		m_strFileName = filename;
	}
	
	if(m_strFileName.IsEmpty())
	{
		m_strFileName.Format(_T("%s\\%s"),Utility::GetModuleDir(NULL).c_str(),DEFAULT_FILENAME);
	}


	CFile file;
	if(!file.Open(m_strFileName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary))
		return false;
	
	file.Write(&CURRENT_FILE_VERSION,sizeof(DWORD));

	for (auto iT = m_ItemList.begin();iT != m_ItemList.end();++iT)
	{
		file.Write(*iT,sizeof(OpItem));
	} 
	file.Close();
	m_bModified = false;
	CRegEdit::WriteValue(HKEY_LOCAL_MACHINE, REG_PATH_APP, REG_ITEM_RUNFILE, m_strFileName);
	return true;
}

void ProjectFile::DeleteItems()
{
	for(auto item : m_ItemList)
	{
		if (item != NULL)
		{
			delete (item);
		}
	} 
	m_ItemList.clear();
}

void ProjectFile::DeleteItem(LPITEM pItem )
{ 
	for(auto iT = m_ItemList.begin(); iT != m_ItemList.end();++iT)
	{
		if (*iT == pItem)
		{
			delete (*iT);
			m_ItemList.erase(iT);
			break;
		}
	}
	m_bModified =true;
}

bool ProjectFile::MoveUp(int nFirstIndex, int nCount)
{ 
	if (((int)m_ItemList.size() <(nFirstIndex+nCount)) || nFirstIndex<=0)
	{
		return false;
	}
	LPITEM pItem = m_ItemList[nFirstIndex-1];
	for (int i = nFirstIndex; i < nFirstIndex + nCount;i++)
	{
		m_ItemList[i-1] = m_ItemList[i];
	}
	m_ItemList[nFirstIndex + nCount-1] = pItem;   

	m_bModified =true;
	return true;
}
 

bool ProjectFile::MoveDown(int nFirstIndex, int nCount)
{
	if (((int)m_ItemList.size() <= (nFirstIndex + nCount)) || nFirstIndex<0)
	{
		return false;
	}
	LPITEM pItem = m_ItemList[nFirstIndex+nCount];
	for (int i = nFirstIndex + nCount; i > nFirstIndex ; i--)
	{
		m_ItemList[i] = m_ItemList[i-1];
	}
	m_ItemList[nFirstIndex] = pItem;

	m_bModified = true;
	return true;
}

void ProjectFile::Destroy()
{
	DestroyCopyList();
	DeleteItems();
	m_strFileName=L"";
	m_bModified = false;
}

void ProjectFile::InsertItem(LPITEM pItem,int index)
{
	if (index<0||index >= (int)m_ItemList.size())
	{
		m_ItemList.push_back(pItem);
	}
	else
	{
		m_ItemList.insert(m_ItemList.begin() + index+1, pItem);
	}
	m_bModified = true;
}

void ProjectFile::Copy(int nFirstIndex, int nCount)
{ 
	DestroyCopyList();
	if (nFirstIndex<0 || (nFirstIndex + nCount)>(int)m_ItemList.size())
		return;
	for (int i = nFirstIndex; i < nFirstIndex + nCount;i++)
	{
		LPITEM pItem = new OpItem;
		*pItem = *m_ItemList[i];
		m_copyList.push_back(pItem);
	}
}

void ProjectFile::DestroyCopyList()
{ 
	for (auto item : m_copyList)
	{
		if (item != NULL)
		{
			delete (item);
		}
	}
	m_copyList.clear();
}

int ProjectFile::Paste(int nIndex)
{
	if (m_copyList.empty())
	{
		return 0;
	}
	OpItemList tmpList; 	
	for (auto &item : m_copyList)
	{
		LPITEM pItem = new OpItem;
		*pItem = *item;
		tmpList.push_back(pItem);
	}
	if (nIndex <0 || nIndex >= (int)m_ItemList.size())
	{ 
		m_ItemList.insert(m_ItemList.end(),tmpList.begin(),tmpList.end()); 
	}
	else
	{ 
		m_ItemList.insert(m_ItemList.begin()+nIndex+1,tmpList.begin(),tmpList.end()); 
	}
	m_bModified = true; 
	return (int)tmpList.size();
}
