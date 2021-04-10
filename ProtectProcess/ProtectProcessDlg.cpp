
// ProtectProcessDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ProtectProcess.h"
#include "ProtectProcessDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DLLPATH "D:\\paper\\ProtectProcess\\x64\\Debug\\ProtectPrcessDll.dll"
#define DESNAME "Taskmgr.exe"
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
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProtectProcessDlg 对话框



CProtectProcessDlg::CProtectProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROTECTPROCESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProtectProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUT_EDIT, m_Out);
}

BEGIN_MESSAGE_MAP(CProtectProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PROTECT_BUTTON, &CProtectProcessDlg::OnBnClickedProtectButton)
END_MESSAGE_MAP()


// CProtectProcessDlg 消息处理程序

BOOL CProtectProcessDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	//提升为DeBug权限
	EnableDeBugPrivilege();


	OnBnClickedProtectButton();
	exit(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProtectProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProtectProcessDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CProtectProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProtectProcessDlg::OnBnClickedProtectButton()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取需要保护的程序的PID 并写入共享的内存中
	//CString csPID;
	//m_Out.GetWindowTextW(csPID);
	//BOOL res = CreaetMap(csPID);
	//if (res)
	//{
	//	MessageBox(L"文件共享失败");
	//}
	////获取任务管理器的PID 并注入
	//return;

	DWORD PID = 0;
	CString csDesName(DESNAME);
	PID = GetPID(csDesName.GetBuffer());
	if (PID == 0)
	{
		MessageBox(L"管理器未开启");
		return;
	}
	BOOL res = InjectDLL(PID, DLLPATH);
	if (res)
	{
		MessageBox(L"注入成功");
	}
	else
	{
		MessageBox(L"注入失败");
	}
}

//提升为DeBug权限
BOOL CProtectProcessDlg::EnableDeBugPrivilege()
{
	HANDLE hToken;
	LUID Luid;
	TOKEN_PRIVILEGES tp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return FALSE;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid))//这里获取了调试权限的Luid
		return FALSE;

	tp.PrivilegeCount = 1;	//需要提升的权限数量
	tp.Privileges[0].Luid = Luid;//这里将调试权限的Luid 赋予了将要提升的权限
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//启用权限
	if (!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	CloseHandle(hToken);
	return TRUE;
}

//注入
BOOL CProtectProcessDlg::InjectDLL(DWORD dwProcessID,CHAR* dllpath)
{
	FARPROC FuncAddr = NULL;
	//获取Kernel32 的句柄 准备获取其中LoadLibraryA 的地址
	HMODULE hModule = LoadLibrary(TEXT("Kernel32.dll"));
	if (hModule != NULL)
	{
		FuncAddr = GetProcAddress(hModule, "LoadLibraryA");
		if (FuncAddr == NULL)return FALSE;
	}

	//通过传入的PID 获取任务管理器的进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL) return FALSE;
	DWORD dwSize = strlen(dllpath) + 1;
	//在内存中申请一段内存 用以存储待注入DLL的路径 (Size为路径字符串的大小)
	LPVOID RemoteBuf = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	SIZE_T dwRealSize;
	//通过VirtualAllocEx 返回的申请的内存的地址 向内存中写入DLL 路径 以备调用
	if (WriteProcessMemory(hProcess, RemoteBuf, dllpath, dwSize, &dwRealSize))
	{
		DWORD dwThreadID;
		//创建一条远程线程  FuncAddr为 通过GetProcAddress 获取到的LoadLibraryA的地址 
		//通过地址就可以调用LoadLibraryA
		//RemoteBuf为传入LoadLibraryA的参数
		//然后进程加载DLL后  便会触发创建进程的消息 执行DLL中的DllMain函数
		HANDLE hRemoteThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)FuncAddr, RemoteBuf, 0, &dwProcessID);
		if (hRemoteThread == NULL)
		{
			VirtualFreeEx(hProcess, RemoteBuf, dwSize, MEM_COMMIT);
			CloseHandle(hProcess);
			return FALSE;
		}
		//释放资源
		WaitForSingleObject(hRemoteThread, INFINITE);
		CloseHandle(hRemoteThread);
		VirtualFreeEx(hProcess, RemoteBuf, dwSize, MEM_COMMIT);
		CloseHandle(hProcess);
		return TRUE;
	}	
	else
	{
		VirtualFreeEx(hProcess, RemoteBuf, dwSize, MEM_COMMIT);
		CloseHandle(hProcess);
		return FALSE;
	}

	return 0;
}

//获取其PID 
DWORD CProtectProcessDlg::GetPID(WCHAR* name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
	
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	int flag = Process32First(hSnapshot, &pe);

	while (flag != 0)
	{
		if (wcscmp(pe.szExeFile,name) == 0)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
		flag = Process32NextW(hSnapshot, &pe);
	}
	CloseHandle(hSnapshot);
	return 0;
}

//创建内存共享并写入PID 
BOOL CProtectProcessDlg::CreaetMap(CString PID)
{
	HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL, PAGE_READWRITE, 0, 16, L"File_Protecte_Mapping");
	if (hMapFile == NULL || INVALID_HANDLE_VALUE == hMapFile)
		return FALSE;

	PVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 16);
	if (pBuf == NULL) return FALSE;
	int iSize = sizeof(PID);
	wcscpy_s((PWCHAR)pBuf, iSize, PID);
	FlushViewOfFile(pBuf, 10);

	DWORD dwPID = 0;
	CString csDesName(DESNAME);
	dwPID = GetPID(csDesName.GetBuffer());
	if (dwPID == 0)
	{
		MessageBox(L"管理器未开启");
		return FALSE;
	}
	BOOL res = InjectDLL(dwPID, DLLPATH);
	if (res)
	{
		MessageBox(L"注入成功");
	}
	else
	{
		MessageBox(L"注入失败");
	}
	//等待
	while (*(PBYTE)pBuf)
		Sleep(2000);
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
	return TRUE;
}