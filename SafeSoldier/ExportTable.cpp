// ExportTable.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "ExportTable.h"
#include "afxdialogex.h"


// ExportTable 对话框

IMPLEMENT_DYNAMIC(ExportTable, CDialogEx)

ExportTable::ExportTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXPORT_DIALOG, pParent)
{

}

ExportTable::~ExportTable()
{
}

void ExportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OFFSET_EDIT, m_Offset);
	DDX_Control(pDX, IDC_TRAIT_EDIT, m_Trait);
	DDX_Control(pDX, IDC_IMAGEBASE_EDI, m_ImageBase);
	DDX_Control(pDX, IDC_NAMEADD_EDIT, m_Name);
	DDX_Control(pDX, IDC_NAMESTR_EDIT, m_NameStr);
	DDX_Control(pDX, IDC_NUMOFFUN_EDIT, m_NumOfFun);
	DDX_Control(pDX, IDC_NUMOFNAMEEDIT, m_NumOfFunName);
	DDX_Control(pDX, IDC_EDIT8, m_FunAddress);
	DDX_Control(pDX, IDC_NAEMADDS_EDIT, m_FunNameAdd);
	DDX_Control(pDX, IDC_NAMEINDEXADD_EDIT, m_FunNameIndexAdd);
	DDX_Control(pDX, IDC_EXPORT_LIST, m_ExportList);
}


BEGIN_MESSAGE_MAP(ExportTable, CDialogEx)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// ExportTable 消息处理程序

BOOL ExportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	WCHAR* wcBuffer[] = {
		L"Ordinal",
		L"RVA",
		L"Offset",
		L"Funcation Name"
	};

	DWORD dwIndex = 0;
	for (auto P : wcBuffer)
	{
		m_ExportList.InsertColumn(dwIndex++, P, LVCFMT_CENTER, 100);
	}


	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



BOOL ExportTable::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MyPe = (MyPe*)pCopyDataStruct->cbData;
	//填充信息
	GetExportTableInfo();
	ShowWindow(SW_SHOWNORMAL);

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void ExportTable::GetExportTableInfo()
{
	char  cBuffer[0x10] = { 0 };
	m_MyPe->pExportTable;
	// 输出表偏移
	DWORD FOA = m_MyPe->RVA2FOA(m_MyPe->pOptionalHeader->DataDirectory[0].VirtualAddress);
	_itoa_s(FOA, cBuffer, 16);
	CA2W FOABuffer(cBuffer);
	m_Offset.SetWindowTextW(FOABuffer.m_szBuffer);
	// 特征值
	m_Trait.SetWindowTextW(L"0");
	// 序号基数
	DWORD IndexBase = m_MyPe->pExportTable->Base;
	_itoa_s(IndexBase, cBuffer, 16);
	CA2W IndexBaseBuff(cBuffer);
	m_ImageBase.SetWindowTextW(IndexBaseBuff.m_szBuffer);
	// 名称
	DWORD NameAdd = m_MyPe->pExportTable->Name;
	_itoa_s(NameAdd, cBuffer, 16);
	CA2W NameAddBuff(cBuffer);
	m_Name.SetWindowTextW(NameAddBuff.m_szBuffer);
	// 名称字符串	
	char* pName = (char*)(m_MyPe->RVA2FOA(NameAdd) + (DWORD)m_MyPe->pDosHeader);
	CA2W NameBuff(pName);
	m_NameStr.SetWindowTextW(NameBuff.m_szBuffer);
	// 函数数量
	DWORD TmpNum = m_MyPe->pExportTable->NumberOfFunctions;
	_itoa_s(TmpNum, cBuffer, 16);
	CA2W FunNum(cBuffer);
	m_NumOfFun.SetWindowTextW(FunNum.m_szBuffer);
	// 函数名数量
	TmpNum = m_MyPe->pExportTable->NumberOfNames;
	_itoa_s(TmpNum, cBuffer, 16);
	CA2W FunNameNum(cBuffer);
	m_NumOfFunName.SetWindowTextW(FunNameNum.m_szBuffer);
	// 函数地址表
	TmpNum = m_MyPe->pExportTable->AddressOfFunctions;
	_itoa_s(TmpNum, cBuffer, 16);
	CA2W FunAddress(cBuffer);
	m_FunAddress.SetWindowTextW(FunAddress.m_szBuffer);
	// 函数名称表
	TmpNum = m_MyPe->pExportTable->AddressOfNames;
	_itoa_s(TmpNum, cBuffer, 16);
	CA2W FunNameAdd(cBuffer);
	m_FunNameAdd.SetWindowTextW(FunNameAdd.m_szBuffer);
	// 函数序号表
	TmpNum = m_MyPe->pExportTable->AddressOfNameOrdinals;
	_itoa_s(TmpNum, cBuffer, 16);
	CA2W FunNameIndexAdd(cBuffer);
	m_FunNameIndexAdd.SetWindowTextW(FunNameIndexAdd.m_szBuffer);

	//LIST
	m_ExportList;
	
	DWORD dwIndex = 0;
	PIMAGE_EXPORT_DIRECTORY tmpExportTable = m_MyPe->pExportTable;
	//函数地址表
	DWORD* dwAddRva = (DWORD*)(m_MyPe->RVA2FOA(tmpExportTable->AddressOfFunctions) +
		(DWORD)m_MyPe->pDosHeader);
	//函数序号表
	WORD* dwOrdianlRva = (WORD*)(m_MyPe->RVA2FOA(tmpExportTable->AddressOfNameOrdinals) +
		(DWORD)m_MyPe->pDosHeader);
	//函数名称表
	DWORD* dwNameRva = (DWORD*)(m_MyPe->RVA2FOA(tmpExportTable->AddressOfNames) +
		(DWORD)m_MyPe->pDosHeader);
	for(size_t i = 0; i < tmpExportTable->NumberOfFunctions; i++, dwIndex++)
	{
		if (dwAddRva[i] == 0)
		{
			continue;
		}
		//输出函数地址 m_MyPe->pExportTable->AddressOfFunctions
		_itoa_s(dwAddRva[i], cBuffer, 16);
		CA2W AddRva(cBuffer);
		m_ExportList.InsertItem(dwIndex, L"");
		//RVA
		m_ExportList.SetItemText(dwIndex, 1, AddRva.m_szBuffer);
		//FOA
		DWORD FOA = m_MyPe->RVA2FOA(dwAddRva[i]) ;
		_itoa_s(FOA, cBuffer, 16);
		CA2W AddFOA(cBuffer);
		m_ExportList.SetItemText(dwIndex, 2, AddFOA.m_szBuffer);
		//根据地址下标找序号 如果有则输出 没有则PASS
		for (size_t j = 0; j < tmpExportTable->NumberOfNames; j++)
		{
			//如果有序号 再根据序号下标找函数名 并输出
			if (dwOrdianlRva[j] == i)
			{
				//Ordianl
				_itoa_s(dwOrdianlRva[j]+ tmpExportTable->Base, cBuffer, 16);
				CA2W OrdianlRva(cBuffer);
				m_ExportList.SetItemText(dwIndex, 0, OrdianlRva.m_szBuffer);
				//Name
				char* tmpName = (char*)(m_MyPe->RVA2FOA(dwNameRva[j])+
					(DWORD)m_MyPe->pDosHeader);
				CA2W dwNameRva(tmpName);
				m_ExportList.SetItemText(dwIndex, 3, dwNameRva.m_szBuffer);
				break;
			}
		}
	}
	   
	
}


