// TLSTable.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "TLSTable.h"
#include "afxdialogex.h"


// TLSTable 对话框

IMPLEMENT_DYNAMIC(TLSTable, CDialogEx)

TLSTable::TLSTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TLS_DIALOG, pParent)
{

}

TLSTable::~TLSTable()
{
}

void TLSTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STARTVA_EDIT, m_StartVA);
	DDX_Control(pDX, IDC_ENDVA_EDIT, m_EndVA);
	DDX_Control(pDX, IDC_INDEXVA_EDIT, m_IndexVA);
	DDX_Control(pDX, IDC_CALLBACK_EDIT, m_CallBack);
	DDX_Control(pDX, IDC_ZERO_EDIT, m_Zero);
	DDX_Control(pDX, IDC_TRAIT_EDIT, m_Trait);
}


BEGIN_MESSAGE_MAP(TLSTable, CDialogEx)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// TLSTable 消息处理程序


BOOL TLSTable::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MyPe = (MyPe*)pCopyDataStruct->cbData;
	//填充信息
	GetTLSInfo();
	ShowWindow(SW_SHOWNORMAL);


	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void TLSTable::GetTLSInfo()
{
	PIMAGE_TLS_DIRECTORY TLS = m_MyPe->pTLSTables;

	UpdateData(TRUE);

	WCHAR* wcBuffer = nullptr;

	
	wcBuffer = DwToWchar(TLS->StartAddressOfRawData, 16);
	m_StartVA.SetWindowTextW(wcBuffer);

	wcBuffer = DwToWchar(TLS->EndAddressOfRawData, 16);
	m_EndVA.SetWindowTextW(wcBuffer);

	wcBuffer = DwToWchar(TLS->AddressOfIndex, 16);
	m_IndexVA.SetWindowTextW(wcBuffer);

	wcBuffer = DwToWchar(TLS->AddressOfCallBacks, 16);
	m_CallBack.SetWindowTextW(wcBuffer);

	wcBuffer = DwToWchar(TLS->SizeOfZeroFill, 16);
	m_Zero.SetWindowTextW(wcBuffer);

	wcBuffer = DwToWchar(TLS->Characteristics, 16);
	m_Trait.SetWindowTextW(wcBuffer);

	UpdateData(FALSE);
}
