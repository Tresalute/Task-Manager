// DataDir.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "DataDir.h"
#include "afxdialogex.h"


// DataDir 对话框

IMPLEMENT_DYNAMIC(DataDir, CDialogEx)

DataDir::DataDir(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATADIR_DIALOG, pParent)
{

}

DataDir::~DataDir()
{
}

void DataDir::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//EXPORT
	DDX_Control(pDX, IDC_EXPORTRVA_EDIT, m_ExportRva);
	DDX_Control(pDX, IDC_EXPORTSIZE_EDIT, m_ExportSize);
	//IMPORT
	DDX_Control(pDX, IDC_IMPORTRVA_EDIT, m_ImportRva);
	DDX_Control(pDX, IDC_IMPORTSIZE_EDIT, m_ImportSize);
	//RESOURCE
	DDX_Control(pDX, IDC_RESOURCERVA_EDIT, m_ResourceRva);
	DDX_Control(pDX, IDC_RESOURCESIZE_EDIT, m_ResourceSize);
	//EXCEPTION
	DDX_Control(pDX, IDC_EXCEPTIONRVA_EDIT, m_ExcepRva);
	DDX_Control(pDX, IDC_EXCEPTIONSIZE_EDIT, m_ExcepSize);
	//SECURITY
	DDX_Control(pDX, IDC_SECURITYRVA_EDIT, m_SecurityRva);
	DDX_Control(pDX, IDC_SECURITYSIZE_EDIT, m_SecuritySize);
	//BASERELOC
	DDX_Control(pDX, IDC_BASERELOCRVA_EDIT, m_BaseReLocRva);
	DDX_Control(pDX, IDC_BASERELOCSIZE_EDIT, m_BaseRelocSize);
	//DEBUG
	DDX_Control(pDX, IDC_DEBUGRVA_EDIT, m_DebugRva);
	DDX_Control(pDX, IDC_DEBUGSIZE_EDIT, m_DebugSize);
	//ARCHITECTURE
	DDX_Control(pDX, IDC_ARCHITECTURERVA_EDIT, m_ArchiRva);
	DDX_Control(pDX, IDC_ARCHITECTURESIZE_EDIT, m_ArchiSize);
	//CLOBALPTR
	DDX_Control(pDX, IDC_CLOBALPTRRVA_EDIT, m_GlobalPtrRva);
	DDX_Control(pDX, IDC_CLOBALPTRSIZE_EDIT, m_GlobalPtrSize);
	//TLS
	DDX_Control(pDX, IDC_TLSRVA_EDIT, m_TLSRva);
	DDX_Control(pDX, IDC_TLSSIZE_EDIT, m_TLSSize);
	//LOAD
	DDX_Control(pDX, IDC_LOADRVA_EDIT, m_LoadRva);
	DDX_Control(pDX, IDC_LOADSIZE_EDIT, m_LoadSize);
	//BOUND
	DDX_Control(pDX, IDC_BOUNDRVA_EDIT, m_BoundRva);
	DDX_Control(pDX, IDC_BOUNDSIZE_EDIT, m_BoundSize);
	//IAT
	DDX_Control(pDX, IDC_IATRVA_EDIT, m_IATRva);
	DDX_Control(pDX, IDC_IATSIZE_EDIT, m_IATSize);
	//DELAU
	DDX_Control(pDX, IDC_DELAURVA_EDIT, m_DelauRva);
	DDX_Control(pDX, IDC_DELAYSIZE_EDIT, m_DelauSize);
	//COM
	DDX_Control(pDX, IDC_COMRVA_EDIT, m_ComRva);
	DDX_Control(pDX, IDC_COMSIZE_EDIT, m_ComSize);
	//OTHER
	DDX_Control(pDX, IDC_OTHERRVA_EDIT, m_OtherRva);
	DDX_Control(pDX, IDC_OTHERSIZE_EDIT, m_OtherSize);
}




BEGIN_MESSAGE_MAP(DataDir, CDialogEx)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_EXPORT_BUTTON, &DataDir::OnBnClickedExportButton)
	ON_BN_CLICKED(IDC_IMPORT_BUTTON2, &DataDir::OnBnClickedImportButton2)
	ON_BN_CLICKED(IDC_RESOURCE_BUTTON, &DataDir::OnBnClickedResourceButton)
	ON_BN_CLICKED(IDC_RELOC_BUTTON, &DataDir::OnBnClickedRelocButton)
	ON_BN_CLICKED(IDC_TLS_BUTTON, &DataDir::OnBnClickedTlsButton)
	ON_BN_CLICKED(IDC_DELAY_BUTTON, &DataDir::OnBnClickedDelayButton)
END_MESSAGE_MAP()


// DataDir 消息处理程序


BOOL DataDir::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	m_MyPe = (MyPe*)pCopyDataStruct->cbData;
	//填充m_SectionList
	GetDataInfo();
	ShowWindow(SW_SHOWNORMAL);

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void DataDir::GetDataInfo()
{
	char  cBuffer[0x10] = { 0 };
	IMAGE_DATA_DIRECTORY* DataDirectory = m_MyPe->pOptionalHeader->DataDirectory;
	//EXPORT
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W ExportRva(cBuffer);
	m_ExportRva.SetWindowTextW(ExportRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W ExportSize(cBuffer);
	m_ExportSize.SetWindowTextW(ExportSize.m_szBuffer);
	//IMPORT
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W ImportRva(cBuffer);
	m_ImportRva.SetWindowTextW(ImportRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W ImportSize(cBuffer);
	m_ImportSize.SetWindowTextW(ImportSize.m_szBuffer);
	//RESOURCE
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W Resource(cBuffer);
	m_ResourceRva.SetWindowTextW(Resource.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W ResourceSize(cBuffer);
	m_ResourceSize.SetWindowTextW(ResourceSize.m_szBuffer);
	//EXCEPTION
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W ExcepRva(cBuffer);
	m_ExcepRva.SetWindowTextW(ExcepRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W ExcepSize(cBuffer);
	m_ExcepSize.SetWindowTextW(ExcepSize.m_szBuffer);
	//SECURITY
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W SecurityRva(cBuffer);
	m_SecurityRva.SetWindowTextW(SecurityRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W SecuritySize(cBuffer);
	m_SecuritySize.SetWindowTextW(SecuritySize.m_szBuffer);
	//BASERELOC
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W BaseReLocRva(cBuffer);
	m_BaseReLocRva.SetWindowTextW(BaseReLocRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W BaseRelocSize(cBuffer);
	m_BaseRelocSize.SetWindowTextW(BaseRelocSize.m_szBuffer);
	//DEBUG
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W DebugRva(cBuffer);
	m_DebugRva.SetWindowTextW(DebugRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W DebugSize(cBuffer);
	m_DebugSize.SetWindowTextW(DebugSize.m_szBuffer);
	//ARCHITECTURE
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W ArchiRva(cBuffer);
	m_ArchiRva.SetWindowTextW(ArchiRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W ArchiSize(cBuffer);
	m_ArchiSize.SetWindowTextW(ArchiSize.m_szBuffer);
	//CLOBALPTR
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W GlobalPtrRva(cBuffer);
	m_GlobalPtrRva.SetWindowTextW(GlobalPtrRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W GlobalPtrSize(cBuffer);
	m_GlobalPtrSize.SetWindowTextW(GlobalPtrSize.m_szBuffer);
	//TLS
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W TLSRva(cBuffer);
	m_TLSRva.SetWindowTextW(TLSRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W TLSSize(cBuffer);
	m_TLSSize.SetWindowTextW(TLSSize.m_szBuffer);
	//LOAD
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W LoadRva(cBuffer);
	m_LoadRva.SetWindowTextW(LoadRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W LoadSize(cBuffer);
	m_LoadSize.SetWindowTextW(LoadSize.m_szBuffer);
	//BOUND
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W BoundRva(cBuffer);
	m_BoundRva.SetWindowTextW(BoundRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W BoundSize(cBuffer);
	m_BoundSize.SetWindowTextW(BoundSize.m_szBuffer);
	//IAT
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W IATRva(cBuffer);
	m_IATRva.SetWindowTextW(IATRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W IATSize(cBuffer);
	m_IATSize.SetWindowTextW(IATSize.m_szBuffer);
	//DELAU
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W DelauRva(cBuffer);
	m_DelauRva.SetWindowTextW(DelauRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W DelauSize(cBuffer);
	m_DelauSize.SetWindowTextW(DelauSize.m_szBuffer);
	//COM
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W ComRva(cBuffer);
	m_ComRva.SetWindowTextW(ComRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W ComSize(cBuffer);
	m_ComSize.SetWindowTextW(ComSize.m_szBuffer);
	//OTHER
	DataDirectory++;
	_itoa_s(DataDirectory->VirtualAddress, cBuffer, 16);
	CA2W OtherRva(cBuffer);
	m_OtherRva.SetWindowTextW(OtherRva.m_szBuffer);

	_itoa_s(DataDirectory->Size, cBuffer, 16);
	CA2W OtherSize(cBuffer);
	m_OtherSize.SetWindowTextW(OtherSize.m_szBuffer);
}

//导出表窗口
void DataDir::OnBnClickedExportButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_MyPe->pOptionalHeader->DataDirectory[0].VirtualAddress == NULL)
	{
		MessageBox(L"无导出表");
		return;
	}
	m_MyExportTable = new ExportTable;
	m_MyExportTable->Create(IDD_EXPORT_DIALOG);

	COPYDATASTRUCT Copydata = { 0 };
	Copydata.cbData = (DWORD)m_MyPe;
	Copydata.dwData = sizeof(DWORD);
	::SendMessage(m_MyExportTable->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&Copydata);

}

//导入表窗口
void DataDir::OnBnClickedImportButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MyImportTable = new ImportTable;
	m_MyImportTable->Create(IDD_IMPORT_DIALOG);

	COPYDATASTRUCT Copydata = { 0 };
	Copydata.cbData = (DWORD)m_MyPe;
	Copydata.dwData = sizeof(DWORD);
	::SendMessage(m_MyImportTable->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&Copydata);

}


void DataDir::OnBnClickedResourceButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MyResourceTable = new ResourceTable;
	m_MyResourceTable->Create(IDD_RESOURCE_DIALOG);

	COPYDATASTRUCT Copydata = { 0 };
	Copydata.cbData = (DWORD)m_MyPe;
	Copydata.dwData = sizeof(DWORD);
	::SendMessage(m_MyResourceTable->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&Copydata);

}


void DataDir::OnBnClickedRelocButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MyReLoc = new ReLoc;
	m_MyReLoc->Create(IDD_RELOC_DIALOG);

	COPYDATASTRUCT Copydata = { 0 };
	Copydata.cbData = (DWORD)m_MyPe;
	Copydata.dwData = sizeof(DWORD);
	::SendMessage(m_MyReLoc->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&Copydata);
}


void DataDir::OnBnClickedTlsButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MyTLS = new TLSTable;
	m_MyTLS->Create(IDD_TLS_DIALOG);

	COPYDATASTRUCT Copydata = { 0 };
	Copydata.cbData = (DWORD)m_MyPe;
	Copydata.dwData = sizeof(DWORD);
	::SendMessage(m_MyTLS->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&Copydata);
}


void DataDir::OnBnClickedDelayButton()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"无延迟输入表");
}
