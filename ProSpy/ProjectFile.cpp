#include "StdAfx.h"
#include "ProjectFile.h"

ProjectFile::ProjectFile( )
{
}

ProjectFile::~ProjectFile(void)
{
	DeleteItems();
}

bool ProjectFile::Open( LPCTSTR filename )
{	 
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

	for (list<OpItem*>::iterator iT = m_ItemList.begin();iT != m_ItemList.end();++iT)
	{
		file.Write(*iT,sizeof(OpItem));
	} 
	file.Close();
	return true;
}

void ProjectFile::DeleteItems()
{
	for(list<OpItem*>::iterator iT = m_ItemList.begin(); iT != m_ItemList.end();iT++)
	{
		if (*iT != NULL)
		{
			delete (*iT);
		}
	}
	m_ItemList.clear();
}

void ProjectFile::DeleteItem( OpItem *pItem )
{
	for(list<OpItem*>::iterator iT = m_ItemList.begin(); iT != m_ItemList.end();++iT)
	{
		if (*iT == pItem)
		{
			delete (*iT);
			m_ItemList.erase(iT);
			break;
		}
	}
}

void ProjectFile::MoveUp( OpItem *pItem )
{
	for(list<OpItem*>::iterator iT = m_ItemList.begin(); iT != m_ItemList.end();iT++)
	{
		if (*iT == pItem)
		{ 
			list<OpItem*>::iterator itPre = iT;
			--itPre;
			m_ItemList.erase(iT);
			m_ItemList.insert(itPre,pItem);
			break;
		}
	}
}

void ProjectFile::MoveDown( OpItem *pItem )
{
	for(list<OpItem*>::iterator iT = m_ItemList.begin(); iT != m_ItemList.end();++iT)
	{
		if (*iT == pItem)
		{
			list<OpItem*>::iterator itAfter = m_ItemList.erase(iT);
			++itAfter;
			m_ItemList.insert(itAfter,pItem);
			break;
		}
	}
}
