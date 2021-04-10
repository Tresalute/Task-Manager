// ThreadAndMoudle.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "afxdialogex.h"
#include "ThreadAndMoudle.h"



// ThreadAndMoudle 对话框

IMPLEMENT_DYNAMIC(ThreadAndMoudle, CDialogEx)

ThreadAndMoudle::ThreadAndMoudle(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TANDM_DIALOG, pParent)
{

}

ThreadAndMoudle::~ThreadAndMoudle()
{
}

void ThreadAndMoudle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Thread_LIST, m_ThreadList);
	DDX_Control(pDX, IDC_MODULE_LIST, m_ModuleList);
}


BEGIN_MESSAGE_MAP(ThreadAndMoudle, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// ThreadAndMoudle 消息处理程序


void ThreadAndMoudle::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL ThreadAndMoudle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ThreadList.SetExtendedStyle(LVS_EX_FULLROWSELECT | WS_VSCROLL);
	m_ThreadList.InsertColumn(0, L"线程标识", LVCFMT_CENTER, 200);

	m_ModuleList.InsertColumn(0, L"模块信息", LVCFMT_CENTER, 200);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//线程数据
void ThreadAndMoudle::PrtThreadInfo()
{
	m_ThreadList.DeleteAllItems();
	int index = 0;
	if (ProInfo->_ThreadInfo.size() == 0)
	{
		m_ThreadList.InsertItem(0, L"拒绝访问");
		return;
	}
	for (auto & P : ProInfo->_ThreadInfo)
	{
		WCHAR ThreadInfo[0x20];
		_itow_s(P.th32ThreadID, ThreadInfo, 0x20, 10);
		m_ThreadList.InsertItem(index++, ThreadInfo);
	}
}
//模块数据
void ThreadAndMoudle::PrtModuleInfo()
{
	m_ModuleList.DeleteAllItems();
	int index = 0;
	if (ProInfo->_ModuleInfo.size() == 0)
	{
		m_ModuleList.InsertItem(0, L"拒绝访问");
		return;
	}
	for (auto & P : ProInfo->_ModuleInfo)
	{
		m_ModuleList.InsertItem(index++, P.szModule);
	}
}

BOOL ThreadAndMoudle::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	ProInfo = (pProcessInfo)pCopyDataStruct->lpData;

	PrtThreadInfo();
	PrtModuleInfo();

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
