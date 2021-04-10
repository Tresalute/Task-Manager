// ReLoc.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "ReLoc.h"
#include "afxdialogex.h"


// ReLoc 对话框

IMPLEMENT_DYNAMIC(ReLoc, CDialogEx)

ReLoc::ReLoc(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RELOC_DIALOG, pParent)
{

}

ReLoc::~ReLoc()
{
}

void ReLoc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BLOCK_LIST, m_BlockList);
	DDX_Control(pDX, IDC_ITEM_LIST, m_ItemList);
}


BEGIN_MESSAGE_MAP(ReLoc, CDialogEx)
	ON_WM_COPYDATA()
	ON_NOTIFY(NM_CLICK, IDC_BLOCK_LIST, &ReLoc::OnNMClickBlockList)
END_MESSAGE_MAP()


// ReLoc 消息处理程序

BOOL ReLoc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_BlockList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_VSCROLL);
	m_BlockList.EnsureVisible(m_BlockList.GetItemCount() - 1, FALSE);

	m_ItemList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_VSCROLL);
	m_ItemList.EnsureVisible(m_BlockList.GetItemCount() - 1, FALSE);



	const WCHAR* cBlock[] = {
		L"Index",
		L"Section",
		L"RVA"
	};
	DWORD dwIndex = 0;
	for (auto P : cBlock)
	{
		m_BlockList.InsertColumn(dwIndex++, P, LVCFMT_CENTER, 100);
	}

	const WCHAR* cItem[] = {
		L"Index",
		L"RVA",
		L"Offset",
		L"Type"
	};
	dwIndex = 0;
	for (auto P : cItem)
	{
		m_ItemList.InsertColumn(dwIndex++, P, LVCFMT_CENTER, 100);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL ReLoc::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MyPe = (MyPe*)pCopyDataStruct->cbData;
	//填充信息
	GetReLocInfo();
	ShowWindow(SW_SHOWNORMAL);


	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void ReLoc::GetReLocInfo()
{


	PIMAGE_BASE_RELOCATION pReLocalTables = m_MyPe->pReLocalTables;

	DWORD dwIndex = 0;
	char cBuffer[0x10] = { 0 };
	WCHAR* wcBuffer = nullptr;
	while (pReLocalTables->VirtualAddress != NULL)
	{
		m_BlockList.InsertItem(dwIndex, L"");
		m_BlockList.UpdateWindow();

		wcBuffer = DwToWchar(dwIndex+1, 16);
		m_BlockList.SetItemText(dwIndex, 0, wcBuffer);

		//计算所在区段 并输出区段名

		DWORD dwNumOfSection = m_MyPe->pFileHeader->NumberOfSections;
		PIMAGE_SECTION_HEADER pSectionHeader = m_MyPe->pSectionHeader;
		for (size_t i = 0; i < dwNumOfSection; i++, pSectionHeader++)
		{
			if (pReLocalTables->VirtualAddress >= pSectionHeader->VirtualAddress&&
				pReLocalTables->VirtualAddress < pSectionHeader->Misc.VirtualSize +
				pSectionHeader->VirtualAddress)
			{
				char* Name = new char[9];
				memset(Name, 0, 9);
				memcpy(Name, pSectionHeader->Name, 8);
				CA2W cSName(Name);
				m_BlockList.SetItemText(dwIndex, 1, cSName.m_szBuffer);
				break;
			}
		}
		
		//输出RVA
		wcBuffer = DwToWchar(pReLocalTables->VirtualAddress, 16);
		m_BlockList.SetItemText(dwIndex, 2, wcBuffer);

		dwIndex++;
		pReLocalTables = (PIMAGE_BASE_RELOCATION)((BYTE*)pReLocalTables + pReLocalTables->SizeOfBlock);
	}


}



void ReLoc::OnNMClickBlockList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int index = (int)m_BlockList.GetFirstSelectedItemPosition();
	if (index == 0) return;
	--index;
	m_ItemList.DeleteAllItems();

	//填充函数窗口信息
	GetItemInfo(index);
	
	return;
}


void ReLoc::GetItemInfo(DWORD dwIndex)
{
	typedef struct MyStruct
	{
		WORD offset : 12;
		WORD type : 4;
	}*pMyStruct;

	PIMAGE_BASE_RELOCATION pReLocalTables = m_MyPe->pReLocalTables;
	for (size_t i = 0; i < dwIndex; i++)
	{
		pReLocalTables = (PIMAGE_BASE_RELOCATION)((BYTE*)pReLocalTables + pReLocalTables->SizeOfBlock);
	}

	char cBuffer[0x10] = { 0 };
	WCHAR* wcBuffer = nullptr;
	size_t szNumOfTable = (pReLocalTables->SizeOfBlock - 8) / 2;
	pMyStruct ReLocal = (pMyStruct)(pReLocalTables + 1);

	for (size_t i = 0; i <= szNumOfTable; i++, ReLocal++)
	{
		if (ReLocal->type == 0x3)
		{
			m_ItemList.InsertItem(i, L"");

			wcBuffer = DwToWchar(i+1, 16);
			m_ItemList.SetItemText(i, 0, wcBuffer);

			wcBuffer = DwToWchar(ReLocal->offset + pReLocalTables->VirtualAddress,16);
			m_ItemList.SetItemText(i, 1, wcBuffer);

			wcBuffer = DwToWchar(m_MyPe->RVA2FOA(ReLocal->offset + pReLocalTables->VirtualAddress), 16);
			m_ItemList.SetItemText(i, 2, wcBuffer);

			wcBuffer = DwToWchar(ReLocal->type, 16);
			m_ItemList.SetItemText(i, 3, wcBuffer);


			//printf("offset:0x%x \n", ReLocal->offset);
			//printf("RVA:0x%x \n", ReLocal->offset + pReLocalTables->VirtualAddress);
			//printf("FOA:0x%x \n", m_MyPe->RVA2FOA(ReLocal->offset + pReLocalTables->VirtualAddress));
		}
	}

}
