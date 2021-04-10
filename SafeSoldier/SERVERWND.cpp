// SERVERWND.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "SERVERWND.h"
#include "afxdialogex.h"
#include <Winsvc.h>


//#pragma comment(lib,"Advapi32.lib")
//_declspec(dllimport) SC_HANDLE WINAPI OpenSCManager(
//	_In_opt_  LPCTSTR lpMachineName,
//	_In_opt_  LPCTSTR lpDatabaseName,
//	_In_      DWORD dwDesiredAccess
//);




// SERVERWND 对话框

IMPLEMENT_DYNAMIC(SERVERWND, CDialogEx)

SERVERWND::SERVERWND(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{

}

SERVERWND::~SERVERWND()
{
}

void SERVERWND::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_LIST, m_SerVerList);
}


BEGIN_MESSAGE_MAP(SERVERWND, CDialogEx)
	ON_WM_TIMER()
	ON_COMMAND(ID_START_SERVER, &SERVERWND::OnStartServer)
	ON_COMMAND(ID_SUSPENDSERVER, &SERVERWND::OnSuspendserver)
	ON_COMMAND(ID_STOPSERVER, &SERVERWND::OnStopserver)
	ON_NOTIFY(NM_RCLICK, IDC_SERVER_LIST, &SERVERWND::OnNMRClickServerList)
	ON_COMMAND(ID_REFRESHSERVER, &SERVERWND::OnRefreshserver)
END_MESSAGE_MAP()


// SERVERWND 消息处理程序


BOOL SERVERWND::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//加载菜单
	m_Menu.LoadMenuW(IDR_MENU2);
	m_SerVerList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_VSCROLL);
	m_SerVerList.EnsureVisible(m_SerVerList.GetItemCount() - 1, FALSE);

	const WCHAR* cTitle[] = {
		L"名称",
		L"描述",
		L"状态",
		L"类型",
		L"服务名称"
	};

	m_SerVerList;
	int index = 0;
	for (auto P : cTitle)
	{
		m_SerVerList.InsertColumn(index++, P, LVCFMT_CENTER,100);
	}
	
	SerVerControl();

	SetTimer(0x110, 1000, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SERVERWND::SerVerControl( )
{
	const WCHAR* state[] = { 
		L" ",L"已停止",L"正在启动",L"正在停止",
		L"运行中",L"挂起",L"正在暂停",L"已暂停" };
	const WCHAR* startType[] = {
		L"由系统加载程序启动的设备驱动程序",
		L"由System功能启动的设备驱动程序",
		L"服务控制管理器在系统启动期间自动启动的服务",
		L"当进程启动StartService时，服务控制管理器启动的服务",
		L"无法启动的服务"
	};
	enum type{ 文件系统驱动 = 1, 内核设备驱动 , 
		WIN32OWN进程 = 0x10, WIN32共享进程 = 0x20};
	//1.打开远程服务控制器 
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,//所有服务状态
		NULL,				//缓冲区
		0,					//缓冲区大小
		&dwSize,			//需要的大小
		&dwServiceNum,		//缓冲区中服务个数
		NULL, NULL);
	//3.申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice =
		(LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//4.第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,//所有服务状态
		(PBYTE)pEnumSerice,
		dwSize,
		&dwSize,
		&dwServiceNum,
		NULL, NULL);
	//5.遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//获取基础信息
		//服务名
		pEnumSerice[i].lpDisplayName;
		m_SerVerList.InsertItem(i, L"");
		m_SerVerList.SetItemText(i, 0, pEnumSerice[i].lpDisplayName);
		m_SerVerList.SetItemText(i, 4, pEnumSerice[i].lpServiceName);
		//服务状态 有 已停止 正在运行 正在暂停
		//根据得到的值手动输出字符串，具体值含义可以在结构体上按F1查MSDN
		DWORD dwState = pEnumSerice[i].ServiceStatusProcess.dwCurrentState;
		if (dwState == 0x04)
		{
			m_SerVerList.SetItemText(i, 2, state[dwState]);
		}		
		//服务类型
		//有 文件系统驱动服务，驱动服务，独立进程服务
		//更多请 查MSDN
		DWORD dwType = pEnumSerice[i].ServiceStatusProcess.dwServiceType;
		const WCHAR* type[] = { L"文件系统驱动",L"内核设备驱动",
	L"WIN32OWN进程", L"WIN32共享进程" };
		switch (dwType)
		{
		case 0x01:
			m_SerVerList.SetItemText(i, 3, L"文件系统驱动");
			break;
		case 0x02:
			m_SerVerList.SetItemText(i, 3, L"内核设备驱动");
			break;
		case 0x10:
			m_SerVerList.SetItemText(i, 3, L"WIN32OWN进程");
			break;
		case 0x20:
			m_SerVerList.SetItemText(i, 3, L"WIN32共享进程");
			break;
		default:
			break;
		}

		//6.获取服务详细信息
		//打开服务
		SC_HANDLE hService = OpenService(hSCM,//服务控制管理器的句柄
			pEnumSerice[i].lpServiceName,   //服务名
			SERVICE_QUERY_CONFIG			//打开权限
		);
		//第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//第二次调用，获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);

		//通过上面获取到的结构体信息具体得想要的值
		//获取启动类型
		DWORD dwStartType = pServiceConfig->dwStartType;
		startType;
		m_SerVerList.SetItemText(i, 1, startType[dwStartType]);
		//类型 自启动 手动启动 已禁用
		//获取路径信息
		pServiceConfig->lpBinaryPathName;
	}
	//return &pEnumSerice;
}


void SERVERWND::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	//更新服务列表
	//SerVerControl();



	CDialogEx::OnTimer(nIDEvent);
}
 
//菜单响应消息
void SERVERWND::OnNMRClickServerList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu* pSubMenu = m_Menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}

//开启服务
void SERVERWND::OnStartServer()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_SerVerList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString strName = m_SerVerList.GetItemText(index, 4);

	SERVICE_STATUS status;
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		TRACE("open SCManager error");
		return;
	}
	// 打开www服务。
	SC_HANDLE hSvc = ::OpenService(hSC, strName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open  erron。");
		::CloseServiceHandle(hSC);
		return;
	}
	//开启服务
	if (::StartService(hSvc, NULL, NULL) == FALSE)
	{
		TRACE("start service error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	// 等待服务启动
	while (::QueryServiceStatus(hSvc, &status) == TRUE)
	{
		::Sleep(status.dwWaitHint);
		if (status.dwCurrentState == SERVICE_RUNNING)
		{
			MessageBox(L"start success。");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			m_SerVerList.SetItemText(index, 2, L"运行中");
			return;
		}
	}
}

//暂停服务
void SERVERWND::OnSuspendserver()
{
	return;
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_SerVerList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString strName = m_SerVerList.GetItemText(index, 0);
	MessageBox(strName);

	SERVICE_STATUS status;
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		TRACE("open SCManager error");
		return;
	}
	// 打开www服务。
	SC_HANDLE hSvc = ::OpenService(hSC, strName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open  erron。");
		::CloseServiceHandle(hSC);
		return;
	}

}

//停止服务
void SERVERWND::OnStopserver()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_SerVerList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString strName = m_SerVerList.GetItemText(index, 4);
	MessageBox(strName);

	SERVICE_STATUS status;
	// 打开服务管理对象
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		TRACE("open SCManager error");
		return;
	}
	// 打开www服务。
	SC_HANDLE hSvc = ::OpenService(hSC, strName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		TRACE("Open  erron。");
		::CloseServiceHandle(hSC);
		return;
	}
	// 停止服务
	if (::ControlService(hSvc,
		SERVICE_CONTROL_STOP, &status) == FALSE)
	{
		TRACE("stop service error。");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return;
	}
	// 等待服务停止
	while (::QueryServiceStatus(hSvc, &status) == TRUE)
	{
		::Sleep(status.dwWaitHint);
		if (status.dwCurrentState == SERVICE_STOPPED)
		{
			MessageBox(L"stop success。");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			m_SerVerList.SetItemText(index, 2, L"");
			return;
		}
		
	}
}



void SERVERWND::OnRefreshserver()
{
	// TODO: 在此添加命令处理程序代码
	SerVerControl();
}
