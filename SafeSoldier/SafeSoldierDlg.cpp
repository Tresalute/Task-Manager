
// SafeSoldierDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "SafeSoldierDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _WIN32_WINNT  0x0400

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnProtect();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

	ON_COMMAND(ID_PROTECT, &CAboutDlg::OnProtect)
END_MESSAGE_MAP()


// CSafeSoldierDlg 对话框



CSafeSoldierDlg::CSafeSoldierDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAFESOLDIER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSafeSoldierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTable);
}

BEGIN_MESSAGE_MAP(CSafeSoldierDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSafeSoldierDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_HOTKEY()
	ON_COMMAND(ID_SHUTDOWN, &CSafeSoldierDlg::OnShutdown)
	ON_COMMAND(ID_RESTAR, &CSafeSoldierDlg::OnRestar)
	ON_COMMAND(ID_DORMANY, &CSafeSoldierDlg::OnDormany)
	ON_COMMAND(ID_Protecte, &CSafeSoldierDlg::OnProtecte)
END_MESSAGE_MAP()


// CSafeSoldierDlg 消息处理程序

BOOL CSafeSoldierDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//AdminReStart();

	//设置菜单
	m_Menu.LoadMenu(IDR_MENU4);
	SetMenu(&m_Menu);

	//窗口名称
	const WCHAR* szBuff[] = {
					 L"PE信息",
					 L"进程信息",
					 L"服务信息",
					 L"杀毒卫士",
					 L"垃圾清理"
	};

	int index = 0;
	for (auto & P : szBuff)
	{
		m_MyTable.InsertItem(index++, P);
	}
	//绑定窗口
	TabWnd[0] = new	PEWND;
	TabWnd[0]->Create(IDD_PE_DIALOG, &m_MyTable);
	TabWnd[1] = new	PROCESSWND;
	TabWnd[1]->Create(IDD_PRS_DIALOG, &m_MyTable);
	/*TabWnd[2] = new	MEMWND;
	TabWnd[2]->Create(IDD_MEM_DIALOG, &m_MyTable);*/
	TabWnd[2] = new	SERVERWND;
	TabWnd[2]->Create(IDD_SERVER_DIALOG, &m_MyTable);
	TabWnd[3] = new	SODRWND;
	TabWnd[3]->Create(IDD_SODR_DIALOG, &m_MyTable);
	TabWnd[4] = new	CLEARWND;
	TabWnd[4]->Create(IDD_CLEAR_DIALOG, &m_MyTable);


	//修改子窗口尺寸
	CRect Rect = { 0 };
	GetClientRect(&Rect);
	//m_MyTable.MoveWindow(Rect.left + 10, Rect.top + 10, Rect.right - 10, Rect.bottom - 10);
	m_MyTable.GetClientRect(&Rect);
	Rect.DeflateRect(1, 26, 3, 2);
	for (int i = 0; i < WNDNUM; ++i)
		TabWnd[i]->MoveWindow(&Rect);
	GetClientRect(&Rect);
	
	//设置状态栏
	m_StatusBar.Create(this);
	SetStatusBar();
	m_StatusBar.SetPaneText(2,L"老板键：Atl+1");

	//显示窗口
	ShowTabWnd(0);
	//更新状态栏
	SetTimer(0x1000, 1000, NULL);

	//初始化注册热键
	::RegisterHotKey(GetSafeHwnd(), KeyID, MOD_ALT, 0x31);
	//Null可以为Zlt,Ctrl,Shift;0x24为Home键的16进制
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSafeSoldierDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CSafeSoldierDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//显示。
HCURSOR CSafeSoldierDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//选择显示Table窗口
void CSafeSoldierDlg::ShowTabWnd(size_t index)
{
	for (size_t i = 0; i < WNDNUM; i++)
	{
		TabWnd[i]->ShowWindow(i == index ? SW_SHOWNORMAL : SW_HIDE);
	}
}

//Table控件选项
void CSafeSoldierDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	ShowTabWnd(m_MyTable.GetCurSel());
}

//改变大小
void CSafeSoldierDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	//if (nType == 1) return;					//最小化则什么都不做 
	//CWnd *pWnd;
	//pWnd = GetDlgItem(IDD_PE_DIALOG);		 //获取控件句柄
	//ChangeSize(TabWnd[0], cx, cy);			//调用changesize()函数

	//pWnd = GetDlgItem(IDD_PRS_DIALOG);			//获取控件句柄
	//ChangeSize(TabWnd[1], cx, cy);			//调用changesize()函数

	////ChangeSize(pWnd,cx,cy)是一个自定义的函数，需要在类的protect属性中进行添加声明afx_msg void ChangeSize(CWnd * pWnd, int cx, int cy); 
	//pWnd = GetDlgItem(IDD_SERVER_DIALOG);
	//ChangeSize(TabWnd[2], cx, cy);

	//pWnd = GetDlgItem(IDD_SODR_DIALOG);
	//ChangeSize(TabWnd[3], cx, cy);

	//pWnd = GetDlgItem(IDD_CLEAR_DIALOG);
	//ChangeSize(TabWnd[4], cx, cy);
	//static int i = 1;
	//if (i)
	//{
	//	m_StatusBar.Create(this);
	//	--i;
	//}

	//以下为各种控件尺寸的变更

	//状态栏
	//CStatusBar *m_pwndStatusBar = (CStatusBar*)&m_StatusBar;
	GetClientRect(&m_Rect);

	// 	CRect rectBar;
	// 
	// 	m_pwndStatusBar->GetClientRect(&rectBar);
	// 	m_pwndStatusBar->MoveWindow(0, cy - rectBar.Height(), m_Rect.Width(), rectBar.Height());

	CRect	m_Rect1;
	//m_pwndStatusBar->GetClientRect(&m_Rect1);
	//m_pwndStatusBar->MoveWindow(0, m_Rect.Height() - m_Rect1.Height(), cx, m_Rect1.Height());

	//pWnd = GetDlgItem(IDC_TAB1);
	//ChangeSize(pWnd, cx, cy);
	//TAB
	//if (m_MyTable.m_hWnd)
	//{
	//	GetClientRect(&m_Rect);
	//	m_MyTable.GetClientRect(&m_Rect1);
	//	//m_MyTable.MoveWindow(14, 20, cx - 20, cy - 70);
	//	m_MyTable.MoveWindow(14, 52, cx - 20, cy - 70);
	//	
	//}
	////窗口
	//if (TabWnd[0] != NULL)
	//{
	//	m_MyTable.GetClientRect(&m_Rect);

	//	for (size_t i = 0; i < WNDNUM; i++)
	//	{
	//		TabWnd[i]->MoveWindow(m_Rect.left + 1, 
	//			m_Rect.top + 26, 
	//			m_Rect.right - 3, 
	//			m_Rect.bottom - 2);
	//	}
	//}
}

//改变大小
void CSafeSoldierDlg::ChangeSize(CWnd * pWnd, int cx, int cy)
{
	if (pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);							//获取控件变化前的大小
		ScreenToClient(&rect);								//将控件大小转换为在对话框中的区域坐标 
		rect.left = rect.left*cx / m_Rect.Width();			//调整控件大小 ，cx/m_rect.Width()为对话框在横向的变化比例
		rect.right = rect.right*cx / m_Rect.Width();		//cx存储的是变化后的宽度，cy存储的是变化后的高度
		rect.top = rect.top*cy / m_Rect.Height();			//m_rect.height()表示的是变化前主窗体的高度
		rect.bottom = rect.bottom*cy / m_Rect.Height();
		pWnd->MoveWindow(rect);								//设置控件大小
	}
}

//以管理员权限启动程序
void CSafeSoldierDlg::AdminReStart()
{
	//检测是否是以管理员权限运行的本程序 
	//是则跳过 不是则重启
	//HANDLE hToken = NULL;
	//BOOL res = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
	//if (!res)
	//{
	//	return;
	//}
	//TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	//BOOL bIsAdmin = false;
	//DWORD dwSize = 0;
	//if (GetTokenInformation(hToken,TokenElevationType,&ElevationType,
	//	sizeof(TOKEN_ELEVATION_TYPE),&dwSize))
	//{
	//	BYTE adminSID[SECURITY_MAX_SID_SIZE];
	//	dwSize = sizeof(adminSID);
	//	CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);
	//	if (ElevationType == TokenElevationTypeLimited)
	//	{
	//		HANDLE hUnfilteredToken = NULL;
	//		GetTokenInformation(hToken, TokenLinkedToken, (PVOID)&hUnfilteredToken, sizeof(HANDLE), &dwSize);
	//		if (!CheckTokenMembership(hUnfilteredToken,&adminSID,&bIsAdmin))
	//		{
	//			return;
	//		}
	//	}
	//	else
	//	{
	//		bIsAdmin = IsUserAnAdmin();
	//	}
	//	CloseHandle(hToken);
	//}
	//BOOL bFullToken = false;
	//switch (ElevationType) 
	//{
	//case TokenElevationTypeDefault:
	//	if (IsUserAnAdmin()) bFullToken = true;
	//	else bFullToken = false;
	//	break;
	//case TokenElevationTypeFull:
	//	if (IsUserAnAdmin()) bFullToken = true;
	//	else bFullToken = false;
	//	break;
	//case TokenElevationTypeLimited:
	//	if (bIsAdmin) bFullToken = false;
	//	else bFullToken = false;
	//	break;
	//}

	//HANDLE hToken = NULL;
	//BOOL res = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
	//if (!res)
	//{
	//	return;
	//}
	//TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;

	//BOOL bIsAdmin = false;
	//DWORD dwSize = 0;

	//TOKEN_ELEVATION tokenEle;
	//DWORD dwRetLen = 0;

	//if (GetTokenInformation(hToken, TokenElevationType, &tokenEle,
	//	sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
	//{
	//	if (dwRetLen == sizeof(ElevationType))
	//	{
	//		bIsAdmin = tokenEle.TokenIsElevated;
	//	}
	//}

	if (!IsUserAnAdmin())
	{
		ShowWindow(SW_SHOWNORMAL);
		return;
	}

	this->ShowWindow(SW_HIDE);
	//获取当前程序路径
	WCHAR wcApplication[MAX_PATH] = { 0 };
	DWORD dwLength = _countof(wcApplication);
	QueryFullProcessImageName(GetCurrentProcess(), 0,
		wcApplication, &dwLength);
	//以管理员权限重新打开
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = L"runas";
	sei.lpFile = wcApplication;
	sei.lpParameters = NULL;
	sei.nShow = SW_SHOWNORMAL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.cbSize = sizeof(sei);
	if (ShellExecuteEx(&sei))
	{

	}
	else ShowWindow(SW_SHOWNORMAL);
}

//设置状态栏 内存信息
void CSafeSoldierDlg::SetStatusBar()
{
	//状态栏
	CString Info;
	UINT IdArray2[] = { 0x201,0x202,0x203,0x204 };
	m_StatusBar.SetIndicators(IdArray2, 3);

	// 设置每一栏的样式
	m_StatusBar.SetPaneInfo(0, 0x201, SBPS_STRETCH, 100);
	m_StatusBar.SetPaneInfo(1, 0x202, SBPS_STRETCH, 100);
	m_StatusBar.SetPaneInfo(2, 0x203, SBPS_STRETCH, 100);

	// 设置状态栏的文字
	m_StatusBar.SetPaneText(0, L"");
	m_StatusBar.SetPaneText(1, L"");
	m_StatusBar.SetPaneText(2, L"");

	MEMORYSTATUS status = { 0 };

	GlobalMemoryStatus(&status);
	Info.Format(L"当前内存占用：%d%%", status.dwMemoryLoad);
	m_StatusBar.SetPaneText(0, Info);

	int cpuUsage = GetCpuUsage();
	Info.Format(L"当前CPU占用：%d%%", cpuUsage);
	m_StatusBar.SetPaneText(1, Info);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

//获取CUP信息
int CSafeSoldierDlg::GetCpuUsage()
{
	static _FILETIME idleTime, kernelTime, userTime;
	//GetSystemTimes(&idleTime, &kernelTime, &userTime);


	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	double dOldIdleTime = FileTime2Double(idleTime);
	double dNewidleTime = FileTime2Double(newIdleTime);
	double dOldKernelTime = FileTime2Double(kernelTime);
	double dOldNewKernelTime = FileTime2Double(newKernelTime);
	double dOldUserTime = FileTime2Double(userTime);
	double dOldNewUserTime = FileTime2Double(newUserTime);

	int usAge = int(100.0 - (dNewidleTime - dOldIdleTime) /
		(dOldNewKernelTime - dOldKernelTime + dOldNewUserTime - dOldUserTime) * 100);

	idleTime = newIdleTime;
	kernelTime = newKernelTime;
	userTime = newUserTime;
	return usAge;

}

//时间转换
double CSafeSoldierDlg::FileTime2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9)
		+ double(fileTime.dwLowDateTime);
}


void CSafeSoldierDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0x1000)
	{
		CString Info;
		int cpuUsage = GetCpuUsage();
		Info.Format(L"当前CPU占用：%d%%", cpuUsage);
		m_StatusBar.SetPaneText(1, Info);


		MEMORYSTATUS status = { 0 };
		GlobalMemoryStatus(&status);
		Info.Format(L"当前内存占用：%d%%", status.dwMemoryLoad);
		m_StatusBar.SetPaneText(0, Info);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//保护进程ID 
void CAboutDlg::OnProtect()
{
	// TODO: 在此添加命令处理程序代码



}

//热键
void CSafeSoldierDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	BOOL bVisible = IsWindowVisible();//查看窗口是显示还是隐藏
	int nCmdShow = bVisible ? SW_HIDE : SW_SHOW;//?自动判断
	ShowWindow(nCmdShow);

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

//关机
void CSafeSoldierDlg::OnShutdown()
{
	// TODO: 在此添加命令处理程序代码
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE,NULL);

}

//重启
void CSafeSoldierDlg::OnRestar()
{
	// TODO: 在此添加命令处理程序代码
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, NULL);
}

//注销
void CSafeSoldierDlg::OnDormany()
{
	// TODO: 在此添加命令处理程序代码
	//SetSuspendState(FALSE, FALSE, FALSE);
	//SetSystemPowerState(TRUE, NULL);
	//注销
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, NULL);
}

//保护此进程不被销毁
void CSafeSoldierDlg::OnProtecte()
{
	// TODO: 在此添加命令处理程序代码
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
