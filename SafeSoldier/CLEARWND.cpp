// CLEARWND.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "CLEARWND.h"
#include "afxdialogex.h"
#include <stdlib.h>
#include <wchar.h>
#include <atlconv.h>
#include <Psapi.h>

// CLEARWND 对话框

IMPLEMENT_DYNAMIC(CLEARWND, CDialogEx)

CLEARWND::CLEARWND(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLEAR_DIALOG, pParent)
	, m_Suffix(_T(""))
{

}

CLEARWND::~CLEARWND()
{
}

void CLEARWND::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELFILEPATH_LIST, m_FPathList);
	DDX_Control(pDX, IDC_FPANTH_EDIT, m_In_FPath);
	DDX_Control(pDX, IDC_FSUF_EDIT, m_FSuf);
	DDX_Control(pDX, IDC_GETFILE_BUTTON, m_GetDelFile);
	DDX_Control(pDX, IDC_DELFILE_BUTTON, m_DelFile);
	DDX_Control(pDX, IDC_DELFILEINFO_LIST, m_DelFileInfo);
	DDX_Text(pDX, IDC_FSUF_EDIT, m_Suffix);
}


BEGIN_MESSAGE_MAP(CLEARWND, CDialogEx)
	ON_BN_CLICKED(IDC_GETFILE_BUTTON, &CLEARWND::OnBnClickedGetfileButton)
	ON_BN_CLICKED(IDC_DELFILE_BUTTON, &CLEARWND::OnBnClickedDelfileButton)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CLEARTRASH_BUTTON, &CLEARWND::OnBnClickedCleartrashButton)
	ON_BN_CLICKED(IDC_ClearMEM_BUTTON, &CLEARWND::OnBnClickedClearmemButton)
END_MESSAGE_MAP()


// CLEARWND 消息处理程序


void CLEARWND::EnumFile(CString FilePath)
{
	//以下遍历文件
	HANDLE hFile;
	WIN32_FIND_DATA fData = { 0 };
	hFile = FindFirstFile(FilePath + _T("\\*"), &fData);
	CString nowFilePath;
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(fData.cFileName, L".") == 0 ||
					wcscmp(fData.cFileName, L"..") == 0)
				{
					continue;
				}

				nowFilePath = FilePath;
				nowFilePath += "\\";
				nowFilePath += fData.cFileName;
				EnumFile(nowFilePath);
			}
			else
			{
				WCHAR* _suffix = fData.cFileName + wcslen(fData.cFileName);
				while (*_suffix != '.')	--_suffix;
				if (NULL != wcsstr(m_Suffix, _suffix))
				{
					nowFilePath = FilePath;
					nowFilePath += "\\";
					nowFilePath += fData.cFileName;
					m_DelFilePath.push_back(nowFilePath);
				}
			}
		} while (FindNextFile(hFile, &fData));
	}
}

BOOL CLEARWND::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_FPathList.SetExtendedStyle(LVS_EX_FULLROWSELECT | WS_VSCROLL);

	m_FPathList.InsertColumn(0, L"文件路径", LVCFMT_CENTER, 200);
	m_DelFileInfo.InsertColumn(0, L"文件路径", LVCFMT_CENTER, 200);

	m_Suffix = L".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\.log\\";
	m_FSuf.SetWindowTextW(m_Suffix);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//获取文件
void CLEARWND::OnBnClickedGetfileButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//更新获取后缀
	m_FSuf.UpdateData(TRUE);
	m_FSuf.GetWindowTextW(m_Suffix);

	CString filePath;
	WCHAR szBuff[0x100] = { 0 };
	int count = m_FPathList.GetItemCount();
	for (int i = 0; i < count; ++i)
	{
		filePath = m_FPathList.GetItemText(i, 0);
		EnumFile(filePath);
	}
	m_DelFileInfo.DeleteAllItems();
	int index = 0;
	for (auto & P : m_DelFilePath)
	{
		m_DelFileInfo.InsertItem(index, P);
		m_DelFileInfo.Scroll(CSize(0, 100000));
	}
	m_DelFileInfo.ShowWindow(SW_SHOWNORMAL);
}

//清理回调函数
DWORD WINAPI ClearProc(LPVOID lpThreadParameter)
{

	CLEARWND* ClearWnd = (CLEARWND*)lpThreadParameter;
	int index = 0;
	ClearWnd->MessageBox(L"清理中");
	for (auto & P : ClearWnd->m_DelFilePath)
	{
		DeleteFile(P);
		ClearWnd->m_FPathList.DeleteItem(index);
	}
	ClearWnd->m_DelFilePath.clear();

	ClearWnd->MessageBox(L"清理完成");
	ClearWnd->m_FPathList.DeleteAllItems();
	ClearWnd->m_DelFileInfo.DeleteAllItems();
	return 0;
}

//清理
void CLEARWND::OnBnClickedDelfileButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 创建线程清理
	CreateThread(NULL, NULL, ClearProc, (LPVOID)this, NULL, NULL);	
}


void CLEARWND::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT i;
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	for (i = 0; i < nFiles; i++)
	{
		TCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
		int count = m_FPathList.GetItemCount();
		for (int i = 0; i < count; ++i)
		{
			CString filePath = m_FPathList.GetItemText(i, 0);
			//过滤重复路径
			if (filePath == szFileName) return;
		}
		m_FPathList.InsertItem(i, szFileName);
	}
	CDialogEx::OnDropFiles(hDropInfo);
}


DWORD WINAPI Cleartrash(LPVOID lpThreadParameter)
{
	CLEARWND* ClearWnd = (CLEARWND*)lpThreadParameter;

	//获取回收站内容
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(SHQUERYRBINFO);
	SHQueryRecycleBin(NULL, &RecycleBinInfo);
	char cNum[0x10] = { 0 };
	char cSize[0x10] = { 0 };
	char cBuffer[0x40] = { 0 };
	_i64toa_s(RecycleBinInfo.i64NumItems, cNum, 0x10, 10);
	_i64toa_s(RecycleBinInfo.i64Size / 1024, cSize, 0x10, 10);
	char cNumBuffer[0x20] = { "回收站文件数量：" };
	char cSizeBuffer[0x20] = { "回收站文件大小：" };
	strcat_s(cNumBuffer, 0x20, cNum);
	strcat_s(cSizeBuffer, 0x20, cSize);

	strcat_s(cBuffer, 0x40, cNumBuffer);
	strcat_s(cBuffer, 0x40, "\n");
	strcat_s(cBuffer, 0x40, cSizeBuffer);
	strcat_s(cBuffer, 0x40, "KB");

	CA2W cWBuffer(cBuffer);
	int iRec = ClearWnd->MessageBox(cWBuffer.m_szBuffer, 0, MB_YESNO);
	if (iRec == IDYES)
	{
		SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION |
			SHERB_NOPROGRESSUI | SHERB_NOSOUND);
		ClearWnd->MessageBox(L"清理完成");
	}
	return 0;

}
//清楚回收站垃圾
void CLEARWND::OnBnClickedCleartrashButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL, NULL, Cleartrash, (LPVOID)this, NULL, NULL);

	//MessageBox("")
	//清理回收站内容
	
}

//清理内存
void CLEARWND::OnBnClickedClearmemButton()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwPIDList[1000] = { 0 };
	DWORD dwBufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, dwBufSize, &dwBufSize);
	for (size_t i = 0; i < dwBufSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	MessageBox(L"清理完成");
}
