// WNDEUM.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "WNDEUM.h"
#include "afxdialogex.h"


// WNDEUM 对话框

IMPLEMENT_DYNAMIC(WNDEUM, CDialogEx)

WNDEUM::WNDEUM(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WNDEMU_DIALOG, pParent)
{

}

WNDEUM::~WNDEUM()
{
}

void WNDEUM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WNDEUM_LIST, m_WndList);
}


BEGIN_MESSAGE_MAP(WNDEUM, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// WNDEUM 消息处理程序


BOOL WNDEUM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	const WCHAR* szBuff[] = {
		L"当前窗口"
	};
	int index = 0;
	for (auto & P : szBuff)
	{
		m_WndList.InsertColumn(index, szBuff[index++], LVCFMT_CENTER, 100);
	}

	OutPutWndList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam)
{
	WCHAR szBuff[256] = { 0 };
	CListCtrl *m_WndList = (CListCtrl*)lParam;
	GetWindowText(hWnd, szBuff, 256);
	if (IsWindowVisible(hWnd) == TRUE && wcslen(szBuff) != 0)
	{
		DWORD nIndex = m_WndList->GetItemCount();
		DWORD nPid = 0;
		m_WndList->InsertItem(nIndex, szBuff);
		GetWindowThreadProcessId(hWnd, &nPid);
		m_WndList->SetItemData(nIndex, nPid);
	}
	return TRUE;
}


void GetProcessFilePath(HANDLE hProcess, CString& sFilePath)
{
	sFilePath = _T("");
	TCHAR tsFileDosPath[MAX_PATH + 1];
	ZeroMemory(tsFileDosPath, sizeof(TCHAR)*(MAX_PATH + 1));
	if (0 == GetProcessImageFileName(hProcess, tsFileDosPath, MAX_PATH + 1))
	{
		return;
	}

	// 获取Logic Drive String长度

	SIZE_T uiLen = GetLogicalDriveStrings(0, NULL);
	if (0 == uiLen)
	{
		return;
	}

	PTSTR pLogicDriveString = new TCHAR[uiLen + 1];
	ZeroMemory(pLogicDriveString, uiLen + 1);
	uiLen = GetLogicalDriveStrings((DWORD)uiLen, pLogicDriveString);
	if (0 == uiLen)
	{
		delete[]pLogicDriveString;
		return;
	}

	TCHAR szDrive[3] = TEXT(" :");
	PTSTR pDosDriveName = new TCHAR[MAX_PATH];
	PTSTR pLogicIndex = pLogicDriveString;

	do
	{
		szDrive[0] = *pLogicIndex;
		uiLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
		if (0 == uiLen)
		{
			if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
			{
				break;
			}

			delete[]pDosDriveName;
			pDosDriveName = new TCHAR[uiLen + 1];
			uiLen = QueryDosDevice(szDrive, pDosDriveName, (DWORD)uiLen + 1);
			if (0 == uiLen)
			{
				break;
			}
		}

		uiLen = _tcslen(pDosDriveName);
		if (0 == _tcsnicmp(tsFileDosPath, pDosDriveName, uiLen))
		{
			sFilePath.Format(_T("%s%s"), szDrive, tsFileDosPath + uiLen);
			break;
		}

		while (*pLogicIndex++);
	} while (*pLogicIndex);

	delete[]pLogicDriveString;
	delete[]pDosDriveName;
}

void WNDEUM::OutPutWndList()
{
	m_WndList.DeleteAllItems();
	EnumWindows(WndEnumProc, (LPARAM)&m_WndList);

	static HIMAGELIST hImage = { 0 };
	static SHFILEINFO fileInfo = { 0 };


	bool button = true;
	if (button)
	{
		button = false;
		hImage = ImageList_Create(24, 24, ILC_COLOR32, 0, 0);
		ImageList_SetBkColor(hImage, m_WndList.GetBkColor());
		m_WndList.SendMessage(LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)&hImage);
	}

	int listNum = m_WndList.GetItemCount();
	ImageList_SetImageCount(hImage, listNum);

	for (int i = 0; i < listNum; ++i)
	{
		CString fileRoute;
		DWORD_PTR pid;
		HANDLE hProess;
		fileRoute = m_WndList.GetItemText(i, 0);
		pid = m_WndList.GetItemData(i);
		hProess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		GetProcessFilePath(hProess, fileRoute);
		CloseHandle(hProess);

		if (SHGetFileInfo(fileRoute, SHGFI_SYSICONINDEX | SHGFI_SMALLICON, &fileInfo, sizeof(SHFILEINFO), SHGFI_ICON))
		{
			SHGetFileInfo(fileRoute, SHGFI_SYSICONINDEX | SHGFI_SMALLICON, &fileInfo, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		}
		ImageList_ReplaceIcon(hImage, i, fileInfo.hIcon);
		DestroyIcon(fileInfo.hIcon);
		m_WndList.SetItem(i, 0, 2, TEXT(""), i, 0, 0, 0);
	}
}


void WNDEUM::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	OutPutWndList();
}
