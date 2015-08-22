/***************************************************************/
/* 系统相关定义                                                  */
/***************************************************************/
#pragma once

#define PROCESS_NAME_LEN 64
#define KEYINPUT_MAX     32

enum OPERATOR_TYPE
{
	OP_UNKNOWN,
	OP_RECORD,      //资源记录
	OP_RCLICK,      //鼠标右键
	OP_LCLICK,      //鼠标左键
	OP_DBCICK,      //鼠标双击
	OP_KEY_INPUT    //键盘输入
};


#define RECORD_PID				0x0001 
#define RECORD_CPU_TIME_KERNEL	0x0002
#define RECORD_CPU_TIME_USER	0x0004

#define RECORD_WORK_SET			0x0010 
#define RECORD_VIRTUAL_MEM		0x0020
#define RECORD_PAGE_FAULT		0x0040

#define RECORD_HANDLE_COUNT		0x0100
#define RECORD_THREAD_COUNT		0x0200
#define RECORD_USER_OBJECT		0x0400
#define RECORD_GDI_COUNT		0x0800

struct RECORD_COLUMN
{
	DWORD dwMask;
	TCHAR szDesc[32];
};

const RECORD_COLUMN c_recordHeaders[] = {
	{RECORD_PID,_T("PID")}, 
	{RECORD_CPU_TIME_KERNEL,_T("CPU Kernel Time(ms)")},
	{RECORD_CPU_TIME_USER,_T("CPU User Time(ms)")},
	{RECORD_WORK_SET,_T("Physical Memory(KB)")}, 
	{RECORD_VIRTUAL_MEM,_T("Virtual Memory(KB)")},
	{RECORD_PAGE_FAULT,_T("Page Fault")},
	{RECORD_HANDLE_COUNT,_T("Handle")},
	{RECORD_THREAD_COUNT,_T("Thread")},
	{RECORD_USER_OBJECT,_T("User Object")},
	{RECORD_GDI_COUNT,_T("GDI Object")}
};

struct OpRecord
{
	DWORD  dwProcessID;
	DWORD  dwMask;
	TCHAR  szFilePath[MAX_PATH];
	TCHAR  szProcessName[PROCESS_NAME_LEN];
};

#define VISIBLE_CHAR_MASK 0x000000   //不按Shift时能打出的可见字符，如‘%’不在此列，
#define CONTROL_KEY_MASK  0x010000   //表示ctrl、Alt、Shift，这些键需要一直保持按下状态直到其他键释放
#define VIRTUAL_KEY_MASK  0X020000   //不可见字符，得到的直接是虚拟键值
struct OpKeyInput
{
	DWORD dwKey[KEYINPUT_MAX];  //高位字表示类型，低位字表示虚拟键值
	void Clear()
	{
		memset(dwKey,0,sizeof(DWORD)*KEYINPUT_MAX);
	}
};


struct OpItem
{
	OPERATOR_TYPE type; 
	union OpDetail{
		OpRecord record;
		POINT pos;
		OpKeyInput keyinput;
	} detail;
	DWORD dwTimeSpan;
	TCHAR szNote[64];
	OpItem()
	{
		memset(this,0,sizeof(OpItem));
	}
};

#define WM_THREAD_STOP (WM_USER + 0x0010)