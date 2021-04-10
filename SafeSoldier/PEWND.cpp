// PEWND.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "PEWND.h"
#include "afxdialogex.h"


// PEWND 对话框

IMPLEMENT_DYNAMIC(PEWND, CDialogEx)

PEWND::PEWND(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PE_DIALOG, pParent)
{

}

PEWND::~PEWND()
{
}

void PEWND::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LFANEW_EDIT, m_lianew);
	DDX_Control(pDX, IDC_OEP_EDIT, m_OEP);
	DDX_Control(pDX, IDC_IMAGEBASE_EDIT, m_ImageBase);
	DDX_Control(pDX, IDC_SECALIG_EDIT, m_SectionAlig);
	DDX_Control(pDX, IDC_FILEALIG_EDIT, m_FileAlig);
	DDX_Control(pDX, IDC_SIZEOFIMAGE_EDIT, m_SizeOfImage);
	DDX_Control(pDX, IDC_SIZEOFHEADER_EDIT, m_SizeOfHeader);
	DDX_Control(pDX, IDC_NUMOFSECTION_EDIT, m_NumOfSection);
	DDX_Control(pDX, IDC_SIZEOFOP_EDIT, m_SizeofOpHerader);
	DDX_Control(pDX, IDC_CHARACTER_EDIT, m_Character);
}


BEGIN_MESSAGE_MAP(PEWND, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_SECTION_BUTTON, &PEWND::OnBnClickedSectionButton)
	ON_BN_CLICKED(IDC_DIRDATA_BUTTON, &PEWND::OnBnClickedDirdataButton)
END_MESSAGE_MAP()


// PEWND 消息处理程序


BOOL PEWND::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void PEWND::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取文件路径
	if (m_MyPe != nullptr)
	{
		delete m_MyPe;
	}
	::DragQueryFile(hDropInfo, 0, m_FPath, _MAX_PATH);

	{
		DWORD readSize = 0;
		//打开文件
		HANDLE hFile = CreateFileW(m_FPath,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(L"不是标准PE文件");
			return ;
		}
		//获取文件大小
		DWORD dwFileSize = GetFileSize(hFile, NULL);

		char *buf = new char[dwFileSize];

		//读取文件 
		ReadFile(hFile, buf, dwFileSize, &readSize, nullptr);
		PIMAGE_DOS_HEADER pDOS = (PIMAGE_DOS_HEADER)buf;
		if (pDOS->e_magic != 0x5A4D)
		{
			MessageBox(L"不是标准PE文件");
			CloseHandle(hFile);
			return;
		}
		CloseHandle(hFile);
	}


	m_MyPe = new MyPe(m_FPath);
	UpdateData(TRUE);

	char* m_buf = nullptr;
	char cBuffer[0x10] = { 0 };
	DWORD FOA = 0;
	PIMAGE_DOS_HEADER pDosHeader = m_MyPe->pDosHeader;
	PIMAGE_NT_HEADERS pNtHeader = m_MyPe->pNtHeader;
	PIMAGE_FILE_HEADER pFileHeader = m_MyPe->pFileHeader;
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = m_MyPe->pOptionalHeader;
	PIMAGE_SECTION_HEADER pSectionHeader = m_MyPe->pSectionHeader;
	// 入口点
	//FOA = m_MyPe->RVA2FOA(pDosHeader->e_lfanew);
	//_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	_itoa_s(pDosHeader->e_lfanew, cBuffer, 16);
	CA2W lianew(cBuffer);
	m_lianew.SetWindowTextW(lianew.m_szBuffer);
	// OEP
	 m_OEP;
	 //FOA = m_MyPe->RVA2FOA(pOptionalHeader->AddressOfEntryPoint);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pOptionalHeader->AddressOfEntryPoint, cBuffer, 16);
	 CA2W OEP(cBuffer);
	 m_OEP.SetWindowTextW(OEP.m_szBuffer);
	// ImageBase
	 m_ImageBase;
	 //FOA = m_MyPe->RVA2FOA(pOptionalHeader->ImageBase);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pOptionalHeader->ImageBase, cBuffer, 16);
	 CA2W ImageBase(cBuffer);
	 m_ImageBase.SetWindowTextW(ImageBase.m_szBuffer);
	// SectionAlignment
	 m_SectionAlig;
	 //FOA = m_MyPe->RVA2FOA(pOptionalHeader->SectionAlignment);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pOptionalHeader->SectionAlignment, cBuffer, 16);
	 CA2W SectionAlig(cBuffer);
	 m_SectionAlig.SetWindowTextW(SectionAlig.m_szBuffer);
	// FileAlignment
	 m_FileAlig;
	 //FOA = m_MyPe->RVA2FOA(pOptionalHeader->FileAlignment);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pOptionalHeader->FileAlignment, cBuffer, 16);
	 CA2W FileAlig(cBuffer);
	 m_FileAlig.SetWindowTextW(FileAlig.m_szBuffer);
	// SizeOfImage
	 m_SizeOfImage;
	 //FOA = m_MyPe->RVA2FOA(pOptionalHeader->SizeOfImage);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pOptionalHeader->SizeOfImage, cBuffer, 16);
	 CA2W SizeOfImage(cBuffer);
	 m_SizeOfImage.SetWindowTextW(SizeOfImage.m_szBuffer);
	// SizeOfHeader
	 m_SizeOfHeader;
	 //FOA = m_MyPe->RVA2FOA(pOptionalHeader->SizeOfHeaders);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pOptionalHeader->SizeOfHeaders, cBuffer, 16);
	 CA2W SizeOfHeader(cBuffer);
	 m_SizeOfHeader.SetWindowTextW(SizeOfHeader.m_szBuffer);
	// NumOfSection
	 m_NumOfSection;
	 //FOA = m_MyPe->RVA2FOA(pFileHeader->NumberOfSections);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pFileHeader->NumberOfSections, cBuffer, 16);
	 CA2W NumOfSection(cBuffer);
	 m_NumOfSection.SetWindowTextW(NumOfSection.m_szBuffer);
	// SizeofOptionalHeader
	 m_SizeofOpHerader;
	 //FOA = m_MyPe->RVA2FOA(pFileHeader->SizeOfOptionalHeader);
	 //_itoa_s((FOA + (DWORD)pDosHeader), cBuffer, 16);
	 _itoa_s(pFileHeader->SizeOfOptionalHeader, cBuffer, 16);
	 CA2W SizeofOpHerader(cBuffer);
	 m_SizeofOpHerader.SetWindowTextW(SizeofOpHerader.m_szBuffer);
	// Characteristics
	 m_Character;
	 _itoa_s(pFileHeader->Characteristics, cBuffer, 16);
	 CA2W Character(cBuffer);
	 m_Character.SetWindowTextW(Character.m_szBuffer);

	 UpdateData(FALSE);

	CDialogEx::OnDropFiles(hDropInfo);
}


//创建区段窗口
void PEWND::OnBnClickedSectionButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_MyPe == nullptr)
	{
		MessageBox(L"请先添加文件");
		return;
	}
	m_MySection = new SECTION;
	m_MySection->Create(IDD_SECTION_DIALOG);

	COPYDATASTRUCT Copydata = { 0 };
	Copydata.cbData = (DWORD)m_MyPe;
	Copydata.dwData = sizeof(DWORD);
	::SendMessage(m_MySection->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&Copydata);

}

//创建数据目录窗口
void PEWND::OnBnClickedDirdataButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_MyPe == nullptr)
	{
		MessageBox(L"请先添加文件");
		return;
	}
	m_MyDataDir = new DataDir;
	m_MyDataDir->Create(IDD_DATADIR_DIALOG);

	COPYDATASTRUCT Copydata = { 0 };
	Copydata.cbData = (DWORD)m_MyPe;
	Copydata.dwData = sizeof(DWORD);
	::SendMessage(m_MyDataDir->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&Copydata);


}


