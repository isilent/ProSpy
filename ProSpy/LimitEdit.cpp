// LimitEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProSpy.h"
#include "LimitEdit.h"


// CLimitEdit

IMPLEMENT_DYNAMIC(CLimitEdit, CEdit)

CLimitEdit::CLimitEdit()
{

}

CLimitEdit::~CLimitEdit()
{
}


BEGIN_MESSAGE_MAP(CLimitEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CLimitEdit ��Ϣ�������




void CLimitEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (((nChar >= 'A')&&(nChar <= 'Z')) 
		||((nChar >= 'a')&&(nChar <= 'z'))
		||((nChar >= '0')&&(nChar <= '9'))
		||(nChar == 8)  //8Ϊ�˸����BackSpace
		||(nChar == ' ' || nChar == '-' || nChar == '=')
		||(nChar == '[' || nChar == ']')
		||(nChar == ';' || nChar == '.' || nChar == '\'' )
		||(nChar == '/' || nChar == '\\')
		)    
	{ 
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
