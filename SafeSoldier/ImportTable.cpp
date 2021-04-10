// ImportTable.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "ImportTable.h"
#include "afxdialogex.h"

#include <shlwapi.h> 
#pragma comment(lib, "shlwapi.lib") 

// ImportTable 对话框

IMPLEMENT_DYNAMIC(ImportTable, CDialogEx)

ImportTable::ImportTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPORT_DIALOG, pParent)
{

}

ImportTable::~ImportTable()
{
}

void ImportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMDLLINFO_LIST, m_ImDllLidt);
	DDX_Control(pDX, IDC_IMFUNINFO_LIST, m_ImFunInfo);
}


BEGIN_MESSAGE_MAP(ImportTable, CDialogEx)
	ON_WM_COPYDATA()
	ON_NOTIFY(NM_DBLCLK, IDC_IMDLLINFO_LIST, &ImportTable::OnNMDblclkImdllinfoList)
END_MESSAGE_MAP()


// ImportTable 消息处理程序


BOOL ImportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ImDllLidt.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_VSCROLL);
	m_ImDllLidt.EnsureVisible(m_ImDllLidt.GetItemCount() - 1, FALSE);

	m_ImFunInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_VSCROLL);
	m_ImFunInfo.EnsureVisible(m_ImDllLidt.GetItemCount() - 1, FALSE);


	WCHAR* wcDllBuffer[] = {
		L"DllName",
		L"OriginalFirstThunk",
		L"TimeDateStamp",
		L"ForWarDerChain",
		L"名称RVA",
		L"FirstThunk"
	};
	DWORD Index = 0;
	for (auto P : wcDllBuffer)
	{
		m_ImDllLidt.InsertColumn(Index++, P, LVCFMT_CENTER,120);
	}

	WCHAR* wcFunBuffer[] = {
	L"ThunkRVA",
	L"ThunkOffset",
	L"ThunkValue",
	L"Hint",
	L"名称ApiName"
	};
	Index = 0;
	for (auto P : wcFunBuffer)
	{
		m_ImFunInfo.InsertColumn(Index++, P, LVCFMT_CENTER,120);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL ImportTable::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_MyPe = (MyPe*)pCopyDataStruct->cbData;
	//填充信息
	GetImPortDllInfo();
	ShowWindow(SW_SHOWNORMAL);

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
//获取DLL信息
void ImportTable::GetImPortDllInfo()
{
	PIMAGE_IMPORT_DESCRIPTOR TmpImportTable = m_MyPe->pImportTable;
	DWORD dwIndex = 0;
	while (TmpImportTable->FirstThunk != NULL)
	{
		char cBuffer[0x10] = { 0 };
		m_ImDllLidt.InsertItem(dwIndex, L"");
		//Dll名字
		char* cName = (char*)(m_MyPe->RVA2FOA(TmpImportTable->Name) +
			(DWORD)m_MyPe->pDosHeader);
		CA2W DllName(cName);
		m_ImDllLidt.SetItemText(dwIndex, 0, DllName.m_szBuffer);
		//Original
		DWORD OriginalFirstThunk = TmpImportTable->OriginalFirstThunk;
		_itoa_s(OriginalFirstThunk, cBuffer, 16);
		CA2W OriginalBuffer(cBuffer);
		m_ImDllLidt.SetItemText(dwIndex, 1, OriginalBuffer.m_szBuffer);
		//TimeDateStamp
		DWORD TimeDateStamp = TmpImportTable->TimeDateStamp;
		_itoa_s(TimeDateStamp, cBuffer, 16);
		CA2W TimeBuffer(cBuffer);
		m_ImDllLidt.SetItemText(dwIndex, 2, TimeBuffer.m_szBuffer);
		//ForwarderChain
		DWORD ForwarderChain = TmpImportTable->ForwarderChain;
		_itoa_s(ForwarderChain, cBuffer, 16);
		CA2W ChainBuffer(cBuffer);
		m_ImDllLidt.SetItemText(dwIndex, 3, ChainBuffer.m_szBuffer);
		//名称RVA
		DWORD Name = TmpImportTable->Name;
		_itoa_s(Name, cBuffer, 16);
		CA2W NameBuffer(cBuffer);
		m_ImDllLidt.SetItemText(dwIndex, 4, NameBuffer.m_szBuffer);
		//FirstThunk
		DWORD FirstThunk = TmpImportTable->FirstThunk;
		_itoa_s(FirstThunk, cBuffer, 16);
		CA2W FirstBuffer(cBuffer);
		m_ImDllLidt.SetItemText(dwIndex, 5, FirstBuffer.m_szBuffer);

		dwIndex++;
		TmpImportTable++;
	}

}

void ImportTable::OnNMDblclkImdllinfoList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int index = (int)m_ImDllLidt.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	m_ImFunInfo.DeleteAllItems();
	CString csFirstThunk = m_ImDllLidt.GetItemText(index, 5);
	CW2A cFirstThunk(csFirstThunk);
	DWORD dwFirstThunk = strtol(cFirstThunk.m_szBuffer, NULL, 16);
	//填充函数窗口信息
	GetImPortFunInfo(dwFirstThunk);
	return;
}

//获取FUN信息
void ImportTable::GetImPortFunInfo(DWORD Thunk)
{
	DWORD dwOriginal = m_MyPe->pImportTable->OriginalFirstThunk;

	DWORD FirstThunk_FOA = m_MyPe->RVA2FOA(Thunk);

	PIMAGE_THUNK_DATA thunk = 
		(PIMAGE_THUNK_DATA)(FirstThunk_FOA + (DWORD)m_MyPe->pDosHeader);
	DWORD dwIndex = 0;
	while (thunk->u1.Ordinal)
	{
		if (!IMAGE_SNAP_BY_ORDINAL(thunk->u1.Ordinal))
		{
			PIMAGE_IMPORT_BY_NAME functionInfo =
				(PIMAGE_IMPORT_BY_NAME)(m_MyPe->RVA2FOA(thunk->u1.AddressOfData) +
				(DWORD)m_MyPe->pDosHeader);
			m_ImFunInfo.InsertItem(dwIndex,L"");
			char cBuffer[0x10] = { 0 };
			//RVA
			dwOriginal;
			_itoa_s(dwOriginal, cBuffer, 16);
			CA2W DataRva(cBuffer);
			m_ImFunInfo.SetItemText(dwIndex, 0, DataRva.m_szBuffer);
			//OFFSET
			DWORD FOA = m_MyPe->RVA2FOA(thunk->u1.AddressOfData);
			_itoa_s(FOA, cBuffer, 16);
			CA2W offset(cBuffer);
			m_ImFunInfo.SetItemText(dwIndex, 1, offset.m_szBuffer);
			//value
			functionInfo;
			_itoa_s(thunk->u1.Ordinal, cBuffer, 16);
			CA2W Ordinal(cBuffer);
			m_ImFunInfo.SetItemText(dwIndex, 2, Ordinal.m_szBuffer);
			//hit
			functionInfo->Hint;
			_itoa_s(functionInfo->Hint, cBuffer, 16);
			CA2W Hint(cBuffer);
			m_ImFunInfo.SetItemText(dwIndex, 3, Hint.m_szBuffer);
			//name 
			functionInfo->Name;
			CA2W NameBuffer(functionInfo->Name);
			m_ImFunInfo.SetItemText(dwIndex, 4, NameBuffer.m_szBuffer);
		}
		dwOriginal += 4;
		dwIndex++;
		thunk++;
	}
}