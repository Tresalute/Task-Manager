// PROCESSWND.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "PROCESSWND.h"
#include "afxdialogex.h"

// PROCESSWND 对话框

IMPLEMENT_DYNAMIC(PROCESSWND, CDialogEx)

PROCESSWND::PROCESSWND(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRS_DIALOG, pParent)
{

}

PROCESSWND::~PROCESSWND()
{
}

void PROCESSWND::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRS_LIST, m_PrsList);
}


BEGIN_MESSAGE_MAP(PROCESSWND, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_PRS_LIST, &PROCESSWND::OnNMRClickPrsList)
	ON_COMMAND(ID_32771, &PROCESSWND::OnProcessKill_1)
	ON_COMMAND(ID_32772, &PROCESSWND::OnSuspendThread_1)
	ON_COMMAND(ID_32773, &PROCESSWND::ResumeThread_1)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_PRS_LIST, &PROCESSWND::OnNMDblclkPrsList)
	ON_WM_TIMER()
	ON_COMMAND(ID_32774, &PROCESSWND::OnWndEum)
	ON_COMMAND(ID_PROTECT, &PROCESSWND::OnProtect)
END_MESSAGE_MAP()


// PROCESSWND 消息处理程序

BOOL PROCESSWND::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Menu.LoadMenuW(IDR_MENU1);

	//LIST TITLE
	m_PrsList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_VSCROLL);
	m_PrsList.EnsureVisible(m_PrsList.GetItemCount() - 1, FALSE);


	const WCHAR* wTtile[] = {
				L"进程名称",
				L"PID",
				L"线程数量",
				L"父进程PID"
	};
	int index = 0;
	for (auto & P : wTtile)
	{
		m_PrsList.InsertColumn(index++, P, LVCFMT_CENTER, 100);
	}

	GetProcessInfo();
	SetTimer(0x101, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//获取进程消息
void PROCESSWND::GetProcessInfo()
{

	//m_MyThreadList.DeleteAllItems();
	//遍历线程vector
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	std::vector<PROCESSENTRY32> pe32;
	PROCESSENTRY32 stcPe32;
	stcPe32.dwSize = sizeof(stcPe32);
	if (Process32First(hProcessSnap, &stcPe32))
	{
		do
		{
			pe32.push_back(stcPe32);
		} while (Process32Next(hProcessSnap, &stcPe32));
	}
	//检测去掉不存在的线程
	if ( nowPe32.size() != 0)
	{
		int index = 0;
		for (auto P = nowPe32.begin(); P != nowPe32.end();)
		{
			if (false == IsJudProcee(pe32, P->th32ProcessID))
			{
				P = nowPe32.erase(P);
				m_PrsList.DeleteItem(index);
				continue;
			}
			++index;
			++P;
		}
	}
	
	//找到新的进程
	int index = m_PrsList.GetItemCount();
	for (auto &P : pe32)
	{
		if ( false == IsJudProcee(nowPe32, P.th32ProcessID))//
		{
			int nNum = 0;
			CString szAmount;
			CString szProID;
			CString szParentID;
			nowPe32.push_back(P);

			m_PrsList.InsertItem(index, L"");
			m_PrsList.UpdateWindow();
			//nNum = PrtThreadInfo(P.th32ProcessID);//线程数量
			nNum = P.cntThreads;
			
			szAmount.Format(_T("%d"), nNum);
			szProID.Format(_T("%d"), P.th32ProcessID);
			szParentID.Format(_T("%d"), P.th32ParentProcessID);
			m_PrsList.SetItemText(index, 0, P.szExeFile);
			m_PrsList.SetItemText(index, 1, szProID);
			m_PrsList.SetItemText(index, 2, szAmount);
			m_PrsList.SetItemText(index, 3, szParentID);
			++index;
		}
	}
}
//判断是否为有效线程
bool IsJudProcee(std::vector<PROCESSENTRY32> pVec32, DWORD pid)
{
	int nNum = pVec32.size();
	for (int i = 0; i < nNum; ++i)
	{
		if (pVec32[i].th32ProcessID == pid)
		{
			return true;
		}
	}
	return false;
}
//线程消息
int PROCESSWND::PrtThreadInfo(DWORD swPid)
{
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	int nNum = 0;
	ThreadInfo.clear();
	THREADENTRY32 stcTe32;
	stcTe32.dwSize = sizeof(stcTe32);
	if (Thread32First(hThreadSnap, &stcTe32))
	{
		do
		{
			if (stcTe32.th32OwnerProcessID == swPid)
			{
				ThreadInfo.push_back(stcTe32);
				++nNum;
			}

		} while (Thread32Next(hThreadSnap, &stcTe32));
	}
	CloseHandle(hThreadSnap);
	return nNum;
}
//弹出菜单
void PROCESSWND::OnNMRClickPrsList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu* pSubMenu = m_Menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}
//结束线程
void PROCESSWND::OnProcessKill_1()
{
	// TODO: 在此添加命令处理程序代码

	int index = (int)m_PrsList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString strPid = m_PrsList.GetItemText(index, 1);
	DWORD dwPid = _wtoi(strPid);

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);

	if (hProcess == 0)
	{
		MessageBox(L"打开进程失败");
		CloseHandle(hProcess);
		return;
	}
	else
	{
		TerminateProcess(hProcess, 0x0002);
		CloseHandle(hProcess);
	}
}
//挂起进程
void PROCESSWND::OnSuspendThread_1()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_PrsList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString strPid = m_PrsList.GetItemText(index, 1);
	DWORD dwPid = _wtoi(strPid);


	HANDLE nHandle = OpenThread(PROCESS_ALL_ACCESS, FALSE, dwPid);
	SuspendThread(nHandle);
	CloseHandle(nHandle);

}
//恢复进程
void PROCESSWND::ResumeThread_1()
{
	// TODO: 在此添加命令处理程序代码
		// TODO: 在此添加命令处理程序代码
	int index = (int)m_PrsList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString strPid = m_PrsList.GetItemText(index, 1);
	DWORD dwPid = _wtoi(strPid);


	HANDLE nHandle = OpenThread(PROCESS_ALL_ACCESS, FALSE, dwPid);
	::ResumeThread(nHandle);
	CloseHandle(nHandle);
}
//改变窗口大小
void PROCESSWND::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&m_Rect);
	static bool button = true;
	if (!button)
	{
		m_PrsList.MoveWindow(14, 14, m_Rect.right-24, m_Rect.bottom-50);
	}
	button = false;
	
}
//模块消息
void PROCESSWND::PrtModuleInfo(DWORD swPid)
{
	ModuleInfo.clear();
	HANDLE ModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, swPid);
	MODULEENTRY32 stcMd32;
	stcMd32.dwSize = sizeof(stcMd32);
	if (Module32First(ModuleSnap, &stcMd32))
	{
		do
		{
			ModuleInfo.push_back(stcMd32);
		} while (Module32Next(ModuleSnap, &stcMd32));
	}
	CloseHandle(ModuleSnap);
}
//双击弹出模块窗口
void PROCESSWND::OnNMDblclkPrsList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (!m_TAndM.m_hWnd)
	{
		m_TAndM.Create(IDD_TANDM_DIALOG, this);
	}
	m_TAndM.ShowWindow(SW_SHOWNORMAL);
	//获取PID 
	int index = (int)m_PrsList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString strPid = m_PrsList.GetItemText(index, 1);
	DWORD dwPid = _wtoi(strPid);
	//更新 线程信息
	PrtThreadInfo(dwPid);
	PrtModuleInfo(dwPid);


	//发送消息
	pProcessInfo ProInfo = new ProcessInfo;
	ProInfo->_ThreadInfo = ThreadInfo;
	ProInfo->_ModuleInfo = ModuleInfo;

	COPYDATASTRUCT data;
	data.cbData = sizeof(ThreadInfo) + sizeof(ModuleInfo);
	data.dwData = 0;
	data.lpData = ProInfo;

	::SendMessage(m_TAndM.m_hWnd, WM_COPYDATA, NULL, (LPARAM)&data);
}
//定时更新进程窗口
void PROCESSWND::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);

	GetProcessInfo();
}
//当前窗口信息
void PROCESSWND::OnWndEum()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_WndEum.m_hWnd)
	{
		m_WndEum.Create(IDD_WNDEMU_DIALOG, this);
	}
	m_WndEum.ShowWindow(SW_SHOWNORMAL);
	m_WndEum.SendMessage(WM_TIMER, NULL, NULL);
}

//保护线程
void PROCESSWND::OnProtect()
{
	// TODO: 在此添加命令处理程序代码
		//获取当前程序路径
	//WCHAR wcApplication[MAX_PATH] = { 0 };
	WCHAR* wcApplication = new WCHAR[MAX_PATH];
	DWORD dwLength = MAX_PATH;
	QueryFullProcessImageName(GetCurrentProcess(), 0,
		wcApplication, &dwLength);
	WCHAR* P = wcApplication;
	P += MAX_PATH;
	while (*P != '\\')
	{
		P--;
	}
	P++;
	wmemset(P, 0, 20);
	wmemcpy(P, L"ProtectProcess.exe", sizeof("ProtectProcess.exe"));
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = L"runas";
	sei.lpFile = wcApplication;
	sei.lpParameters = NULL;
	sei.nShow = SW_SHOWNORMAL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.cbSize = sizeof(sei);
	ShellExecuteEx(&sei);

}
