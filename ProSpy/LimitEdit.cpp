// LimitEdit.cpp : 实现文件
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



// CLimitEdit 消息处理程序




void CLimitEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (((nChar >= 'A')&&(nChar <= 'Z')) 
		||((nChar >= 'a')&&(nChar <= 'z'))
		||((nChar >= '0')&&(nChar <= '9'))
		||(nChar == 8)  //8为退格键，BackSpace
		||(nChar == ' ' || nChar == '-' || nChar == '=')
		||(nChar == '[' || nChar == ']')
		||(nChar == ';' || nChar == '.' || nChar == '\'' )
		||(nChar == '/' || nChar == '\\')
		)    
	{ 
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
