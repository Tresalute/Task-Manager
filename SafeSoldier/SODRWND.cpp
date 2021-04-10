// SODRWND.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "SODRWND.h"
#include "afxdialogex.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

#define WHITEBASE L"./WhiteList.txt"
#define SERVERIP "127.0.0.1"
#define SERVERPORT 6999



// SODRWND 对话框

IMPLEMENT_DYNAMIC(SODRWND, CDialogEx)

SODRWND::SODRWND(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SODR_DIALOG, pParent)
{

}

SODRWND::~SODRWND()
{
}

void SODRWND::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILEPATH_EDIT, m_FilePath);
	DDX_Control(pDX, IDC_SODR_LIST, m_SoDrList);
	DDX_Control(pDX, IDC_TRAVERSAL_EDIT, m_Traversal);
}


BEGIN_MESSAGE_MAP(SODRWND, CDialogEx)
	ON_BN_CLICKED(IDC_CHOICEFILE_BUTTON, &SODRWND::OnBnClickedChoicefileButton)
	ON_BN_CLICKED(IDC_MD5_BUTTON, &SODRWND::OnBnClickedMd5Button)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_GOOD_BUTTON, &SODRWND::OnBnClickedGoodButton)
	ON_NOTIFY(NM_RCLICK, IDC_SODR_LIST, &SODRWND::OnNMRClickSodrList)
	ON_COMMAND(ID_WhiteList, &SODRWND::OnWhitelist)
	ON_BN_CLICKED(IDC_Virus_BUTTON, &SODRWND::OnBnClickedVirusButton)
	ON_COMMAND(ID_32788, &SODRWND::OnDelWhiteList)
END_MESSAGE_MAP()


// SODRWND 消息处理程序


BOOL SODRWND::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	// TODO:  在此添加额外的初始化
	//初始化MENU
	m_Menu.LoadMenuW(IDR_MENU3);

	//初始化LIST
	const WCHAR* cBuffer[] = {
		L"路径"
	};
	DWORD dwIndex = 0;
	for (auto P : cBuffer)
	{
		m_SoDrList.InsertColumn(dwIndex++, P, LVCFMT_CENTER, 1000);
	}

	//从数据库下载病毒MD5库
	BOOL result = m_ClientServer.InitClient(SERVERIP, SERVERPORT);
	if (!result)
	{
		MessageBox(L"连接服务器失败");
		return TRUE;
	}
	//发送下载病毒库消息
	m_ClientServer.send(GetVirusMD5Base,"NULL");
	HANDLE hHandel = CreateThread(NULL, NULL, recvProc, (LPVOID)this, NULL, NULL);

	//加载白名单
	CW2A tmpbuff(WHITEBASE);
	FILE* file = fopen(tmpbuff.m_szBuffer, "r");
	if (file == NULL)
	{
		MessageBox(L"白名单为空");
		file = fopen(tmpbuff.m_szBuffer, "w");
		fclose(file);
	}
	else 
	{
		while (!feof(file))
		{
			char tmpcbuffer[MAX_PATH] = { 0 };
			fscanf(file, "%s\r\n", tmpcbuffer);
			CString tmpCsFilePath(tmpcbuffer);
			vecWhite.push_back(tmpCsFilePath);
		}
		fclose(file);
	}

	m_SoDrList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_VSCROLL);
	m_SoDrList.EnsureVisible(m_SoDrList.GetItemCount() - 1, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//消息处理
DWORD CALLBACK recvProc(LPVOID arg)
{
	SODRWND * SoDrWnd = (SODRWND*)arg;
	ServerPort* pSoDrWnd = &SoDrWnd->m_ClientServer;
	DataPackResult* pResult = NULL;
	char name[64];
	while (TRUE)
	{
		pResult = pSoDrWnd->recv();
		if (pResult == NULL)
		{
			Sleep(0);
			continue;
		}
		if (pResult->type == 14)//获取病毒库
		{
			pResult->data;
			CString buff(pResult->data);
			SoDrWnd->m_vecVirusMD5 = SplitCString(buff, ',');
		}
	}
	return 0;
}

//处理数据库数据
std::vector<CString> SplitCString(CString strSource, char ch)
{
	CString strTmp;
	std::vector<CString> vecString;
	int iPos = 0;
	while (AfxExtractSubString(strTmp, strSource, iPos, ch))
	{
		iPos++;
		if (strTmp != "")
		{
			vecString.push_back(strTmp);
		}

	}
	return vecString;
}

//选择杀毒路径
void SODRWND::OnBnClickedChoicefileButton()
{
	// TODO: 在此添加控件通知处理程序代码
	vecString.clear();
	m_vecFilePath.clear();

	TCHAR tcBuffer[MAX_PATH] = { 0 };


	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.lpszTitle = L"请选择文件";
	bi.pszDisplayName = tcBuffer;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;

	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);
	tcBuffer;
	if (pItemIDList)
	{
		if (SHGetPathFromIDList(pItemIDList, tcBuffer))
		{
			CString csPath = tcBuffer;
			UpdateData(TRUE);
			m_FilePath.SetWindowTextW(csPath +"\\");
		}
	}
	return;
}

//遍历文件夹获取文件
void SODRWND::EnumFile(CString FilePath)
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
				if (wcscmp(fData.cFileName, L".") == 0 || wcscmp(fData.cFileName, L"..") == 0)
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
				nowFilePath = FilePath;
				nowFilePath += "\\";
				nowFilePath += fData.cFileName;
				m_vecFilePath.push_back(nowFilePath);
			}
		} while (FindNextFile(hFile, &fData));
	}
}

//查询与病毒MD5相同的文件
void SODRWND::OnBnClickedMd5Button()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filepath;
	GetDlgItemTextW(IDC_MD5_BUTTON, filepath);
	if (filepath == "开始查询")
	{	
		//创建一条新线程用于 查找 
		CreateThread(NULL, NULL, FindVirusProc, (LPVOID)this, NULL, NULL);
		SetDlgItemTextW(IDC_MD5_BUTTON, L"开始查杀");
	}
	else
	{	//创建一条新线程用于查杀
		CreateThread(NULL, NULL, DelVirusProc, (LPVOID)this, NULL, NULL);
		SetDlgItemTextW(IDC_MD5_BUTTON, L"开始查询");
	}
	

	

	
}

//寻找病毒文件
DWORD WINAPI FindVirusProc(LPVOID lpThreadParameter)
{
	SODRWND* SoDr = (SODRWND*)lpThreadParameter;
	//获取查杀文件路径
	CString csFilePath;
	SoDr->m_FilePath.GetWindowTextW(csFilePath);
	//批量获取文件
	SoDr->EnumFile(csFilePath);
	SoDr->m_vecFilePath;
	//转换文件MD5 与 病毒库进行比较
	SoDr->m_vecVirusMD5;
	SoDr->FindVirus();
	SoDr->MessageBox(L"查找完成");
	
	return 0;
}

//病毒文件删除
DWORD WINAPI DelVirusProc(LPVOID lpThreadParameter)
{
	SODRWND* SoDrWnd = (SODRWND*)lpThreadParameter;
	//获取list中的文件路径  与白名单比较 进行删除
	INT listCount = SoDrWnd->m_SoDrList.GetItemCount();
	for (auto virusPath: SoDrWnd->vecString)
	{
		CW2A cwBuffer(virusPath);
		char* MD5Buffer = md5FileValue(cwBuffer.m_psz);
		CString csBuffer(MD5Buffer);

		vector<CString>::iterator iter;
		iter = std::find(SoDrWnd->vecWhite.begin(), SoDrWnd->vecWhite.end(), csBuffer);

		//有相同则跳过删除
		if (iter != SoDrWnd->vecWhite.end())
		//if (MD5Buffer1 == MD5Buffer2)
		{
			continue;
		}
		else//删除文件
		{
			DeleteFile(virusPath);
		}
	}
	SoDrWnd->MessageBox(L"查杀完成");
	SoDrWnd->m_FilePath.SetWindowTextW(L"");
	SoDrWnd->m_SoDrList.DeleteAllItems();
	return TRUE;
}

//比较寻找病毒文件
void SODRWND::FindVirus()
{
	//消除列表框内容
	m_SoDrList.DeleteAllItems();
	DWORD dwIndex = 0;
	//如果m_vecFilePath为空 则为单文件查杀 //否则为问价夹路径查杀
	if (m_vecFilePath.empty())
	{
		//从m_FilePath中获取文件路径
		CString csFilePath;
		m_FilePath.GetWindowTextW(csFilePath);
		m_vecFilePath.push_back(csFilePath);
	}
	//通过获取文件MD5 值 与病毒库中的MD5值进行比较 获取相同的
	for (auto P : m_vecFilePath)
	{
		//遍历框视觉效果
		m_Traversal.SetWindowTextW(P);
		CW2A caBuffer(P);
		//转换为MD5
		char* md5Buffer = md5FileValue(caBuffer.m_psz);
		for (auto V : m_vecVirusMD5)
		{
			CW2A VirusBuffer(V);
			if (!strcmp(md5Buffer, VirusBuffer.m_psz))
			{
				//将获取到的病毒文件加入列表框显示
				m_SoDrList.InsertItem(dwIndex, L"");
				m_SoDrList.SetItemText(dwIndex++, 0, P.GetBuffer());
				//将病毒文件加入病毒文件集合中准备删除
				vecString.push_back(P);
				break;
			}
		}
	}
	m_Traversal.SetWindowTextW(L"");
}

//文件拖拽
void SODRWND::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	vecString.clear();
	m_vecFilePath.clear();
	// 文件名
	TCHAR FPath[_MAX_PATH];
	::DragQueryFile(hDropInfo, 0, FPath, _MAX_PATH);
	UpdateData(TRUE);
	m_FilePath.SetWindowTextW(FPath);


	UpdateData(FALSE);
	CDialogEx::OnDropFiles(hDropInfo);
}

//读取白名单文件 并加载显示至LIST框中
void SODRWND::OnBnClickedGoodButton()
{
	// TODO: 在此添加控件通知处理程序代码

	m_SoDrList.DeleteAllItems();
	DWORD dwIndex = 0;
	for (auto P : vecWhite) 
	{
		m_SoDrList.InsertItem(dwIndex, L"");
		m_SoDrList.SetItemText(dwIndex++,0,P);
	}
}

//通过选中文件 弹出菜单
void SODRWND::OnNMRClickSodrList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu* pSubMenu = m_Menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
}

//添加进白名单
void SODRWND::OnWhitelist()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_SoDrList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString filepath = m_SoDrList.GetItemText(index, 0);

	//去重
	for (auto P: vecWhite)
	{
		if (P == filepath)
		{
			return;
		}
	}
	
	//写入白名单文件
	CW2A tmpbuff(WHITEBASE);
	char path[MAX_PATH] = { 0 };
	FILE * file = fopen(tmpbuff.m_szBuffer, "a+");
	if (file == NULL)
	{
		MessageBox(L"打开白名单文件失败");
		fclose(file);
	}
	
	CW2A cwPath(filepath);
	char* cBuffer = md5FileValue(cwPath.m_psz);
	//memcpy(path, cwPath.m_szBuffer, MAX_PATH);
	//fprintf_s(file, "%s\r\n", path);
	fprintf_s(file, "%s\r\n", cBuffer);
	fclose(file);

	CA2W caBuffer(cBuffer);
	vecWhite.push_back(caBuffer.m_szBuffer);

}

//从白名单删除
void SODRWND::OnDelWhiteList()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_SoDrList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	CString filepath = m_SoDrList.GetItemText(index, 0);

	//查询是否在白名单内 在则删除 vecWhite 并从新写入文件
	std::vector<CString>::iterator Itor;
	Itor = std::find(vecWhite.begin(), vecWhite.end(), filepath);
	//找到了
	if (Itor != vecWhite.end())
	{
		vecWhite.erase(Itor);
		MessageBox(L"删除成功");
		m_SoDrList.DeleteAllItems();
		//重新写入文件
		CW2A tmpBuffer(WHITEBASE);
		FILE* file = fopen(tmpBuffer.m_psz, "w");
		if (file)
		{
			for (auto P : vecWhite)
			{
				fprintf_s(file, "%s\r\n", P);
			}
			fclose(file);
		}
		else
		{
			MessageBox(L"文件创建失败");
		}
	}
	else MessageBox(L"白名单内无此项");
}

//计算文件MD5  获取文件的信息
void SODRWND::OnBnClickedVirusButton()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//从输入框内获取文件信息
	CString filePath;
	m_FilePath.GetWindowTextW(filePath);
	
	CW2A caFilePath(filePath);
	DWORD dwLenth = sizeof(caFilePath.m_psz);

	char* p = caFilePath.m_psz;
	p += dwLenth - 1;
	if (p == "\\")
	{
		return;
	}

	WIN32_FIND_DATA stcFData = { 0 };

	HANDLE hFind = FindFirstFile(filePath.GetString(),&stcFData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	//输出文件信息

	FILETIME time = stcFData.ftCreationTime;

	char fileInfo[0x100] = "大小：";
	CString csFileInfo;

	//大小
	DWORD dwSize = stcFData.nFileSizeLow / 1024;
	char buffer[0x20] = { 0 };
	_itoa_s(dwSize, buffer, 10);
	strcat(fileInfo, buffer);
	strcat(fileInfo, " KB\r\nMD5: ");

	//MD5
	char* md5Buffer = md5FileValue(caFilePath.m_szBuffer);
	strcat(fileInfo, md5Buffer);
	strcat(fileInfo, "\r\n创建时间： ");
	//时间
	SYSTEMTIME SystemTime;
	FileTimeToSystemTime(&stcFData.ftCreationTime, &SystemTime);
	SystemTime.wYear;
	SystemTime.wMonth;
	SystemTime.wDay;
	csFileInfo.Format(_T("%hd.%hd.%hd\r\n"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay);
	CW2A caFileInfo(csFileInfo);
	strcat(fileInfo, caFileInfo.m_psz);


	CA2W cwFileInfo(fileInfo);
	MessageBox(cwFileInfo.m_szBuffer);

}