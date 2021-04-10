// SECTION.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "SECTION.h"
#include "afxdialogex.h"


// SECTION 对话框

IMPLEMENT_DYNAMIC(SECTION, CDialogEx)

SECTION::SECTION(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SECTION_DIALOG, pParent)
{

}

SECTION::~SECTION()
{
}

void SECTION::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SECTION_LIST, m_SectionList);
}


BEGIN_MESSAGE_MAP(SECTION, CDialogEx)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// SECTION 消息处理程序


BOOL SECTION::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	const WCHAR* wcTitle[] = {
		L"Name",
		L"VSize",
		L"VOffset",
		L"RSize",
		L"ROffset",
		L"标志"
	};
	DWORD dwIndex = 0;
	for (auto P: wcTitle)
	{
		m_SectionList.InsertColumn(dwIndex++, P, LVCFMT_CENTER, 100);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SECTION::GetSectionInfo()
{
	DWORD dwNumOfSection = m_MyPe->pFileHeader->NumberOfSections;
	PIMAGE_SECTION_HEADER pSectionHeader = m_MyPe->pSectionHeader;
	for (size_t i = 0; i < dwNumOfSection; i++, pSectionHeader++)
	{
		char  cBuffer[0x10] = { 0 };
		m_SectionList.InsertItem(i, L"");
		//区段名
		pSectionHeader->Name;
		char* Name = new char[9];
		memset(Name, 0, 9);
		memcpy(Name, pSectionHeader->Name, 8);
		CA2W cSName(Name);
		m_SectionList.SetItemText(i, 0, cSName.m_szBuffer);
		//区段大小 对齐前
		pSectionHeader->Misc.VirtualSize;
		_itoa_s(pSectionHeader->Misc.VirtualSize, cBuffer, 16);
		CA2W cSVirtualSize(cBuffer);
		m_SectionList.SetItemText(i, 1, cSVirtualSize.m_szBuffer);
		//区段相对虚拟地址
		pSectionHeader->VirtualAddress;
		_itoa_s(pSectionHeader->VirtualAddress, cBuffer, 16);
		CA2W cSVirtualAddress(cBuffer);
		m_SectionList.SetItemText(i, 2, cSVirtualAddress.m_szBuffer);
		//区段在文件中的大小
		pSectionHeader->SizeOfRawData;
		_itoa_s(pSectionHeader->SizeOfRawData, cBuffer, 16);
		CA2W cSSizeOfRawData(cBuffer);
		m_SectionList.SetItemText(i, 3, cSSizeOfRawData.m_szBuffer);
		//区段在文件中的偏移
		pSectionHeader->PointerToRawData;
		_itoa_s(pSectionHeader->PointerToRawData, cBuffer, 16);
		CA2W cSPointerToRawData(cBuffer);
		m_SectionList.SetItemText(i, 4, cSPointerToRawData.m_szBuffer);
		//区段标志
		pSectionHeader->Characteristics;
		_itoa_s(pSectionHeader->Characteristics, cBuffer, 16);
		CA2W cSCharacteristics(cBuffer);
		m_SectionList.SetItemText(i, 5, cSCharacteristics.m_szBuffer);
	}
}


BOOL SECTION::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MyPe = (MyPe*)pCopyDataStruct->cbData;
	//填充m_SectionList
	GetSectionInfo();
	ShowWindow(SW_SHOWNORMAL);
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
