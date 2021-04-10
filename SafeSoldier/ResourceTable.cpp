// ResourceTable.cpp: 实现文件
//

#include "stdafx.h"
#include "SafeSoldier.h"
#include "ResourceTable.h"
#include "afxdialogex.h"


// ResourceTable 对话框

IMPLEMENT_DYNAMIC(ResourceTable, CDialogEx)

ResourceTable::ResourceTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESOURCE_DIALOG, pParent)
{

}

ResourceTable::~ResourceTable()
{
}

void ResourceTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESOURCE_TREE, m_ResouceTree);
	DDX_Control(pDX, IDC_ROOTNAME_EDIT, m_RootName);
	DDX_Control(pDX, IDC_ROOTID_EDIT, m_RootID);
	DDX_Control(pDX, IDC_CHOICENAEM_EDIT, m_ChoiceName);
	DDX_Control(pDX, IDC_CHOICEID_EDIT, m_ChoiceID);
	DDX_Control(pDX, IDC_CHOICERVA_EDIT, m_ChoiceRVA);
	DDX_Control(pDX, IDC_CHOICEOFFSET_EDIT, m_ChoiceOffset);
	DDX_Control(pDX, IDC_CHOICESIZE_EDIT, m_ChoiceSize);
}


BEGIN_MESSAGE_MAP(ResourceTable, CDialogEx)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// ResourceTable 消息处理程序



BOOL ResourceTable::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MyPe = (MyPe*)pCopyDataStruct->cbData;
	//填充信息
	DWORD dwResourceRVA = m_MyPe->pOptionalHeader->DataDirectory[2].VirtualAddress;
	GetResourceInfo();
	ShowWindow(SW_SHOWNORMAL);


	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

void ResourceTable::GetResourceInfo(DWORD dwResourceTableRva, HTREEITEM treeRoot)
{
	DWORD FOA = m_MyPe->RVA2FOA(dwResourceTableRva);

	PIMAGE_RESOURCE_DIRECTORY TmpResourceTables = (PIMAGE_RESOURCE_DIRECTORY)(FOA + (DWORD)m_MyPe->pDosHeader);
	size_t szNumOfResource =
		TmpResourceTables->NumberOfNamedEntries +
		TmpResourceTables->NumberOfIdEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntryTable =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)((BYTE*)TmpResourceTables +
			sizeof(IMAGE_RESOURCE_DIRECTORY));
	char cBuffer[0x10] = { 0 };
	HTREEITEM treeChildRoot;
	for (size_t i = 0; i < szNumOfResource; i++)
	{
		if (pResourceEntryTable[i].NameIsString == 0)
		{
			if (pResourceEntryTable[i].Id < 0x10)
			{
				//添加根目录
				resourceType[pResourceEntryTable[i].Id - 1];
				CA2W TreeBuffer(resourceType[pResourceEntryTable[i].Id - 1]);
				treeChildRoot = m_ResouceTree.InsertItem(TreeBuffer.m_szBuffer, treeRoot);//4292901760U
				//printf(" %s ", resourceType[pResourceEntryTable[i].Id - 1]);
			}
			else 
			{
				//添加根目录
				_itoa_s(pResourceEntryTable[i].Id, cBuffer, 16);
				CA2W TreeBuffer(cBuffer);
				treeChildRoot = m_ResouceTree.InsertItem(TreeBuffer.m_szBuffer,treeRoot);
				//printf(" %x ", pResourceEntryTable[i].Id);
			}
		}
		else
		{
			//添加根目录
			PIMAGE_RESOURCE_DIR_STRING_U pResourceDirTable =
				(PIMAGE_RESOURCE_DIR_STRING_U)(pResourceEntryTable[i].NameOffset +
				(DWORD)TmpResourceTables);
			//size_t lenth = pResourceDirTable->Length + 1;
			//wchar_t* sourceName = new wchar_t[lenth];
			//wmemset(sourceName, 0, lenth);
			//wmemcpy(sourceName, pResourceDirTable->NameString, pResourceDirTable->Length);
			//treeChildRoot = m_ResouceTree.InsertItem(sourceName, treeRoot);
			WCHAR sourceName[100] = { 0 };
			wcsncpy_s(sourceName, pResourceDirTable->NameString, pResourceDirTable->Length);
			treeChildRoot = m_ResouceTree.InsertItem(sourceName, treeRoot);
			//printf("名称:%s", CW2A(sourceName).m_szBuffer);
		}
		if (pResourceEntryTable[i].DataIsDirectory == 1)
		{
			GetResourceInfo(pResourceEntryTable[i].OffsetToDirectory +
				(DWORD)m_MyPe->pOptionalHeader->DataDirectory[2].VirtualAddress, treeChildRoot);
		}
		else
		{
			PIMAGE_RESOURCE_DATA_ENTRY pResourceDataTable =
				(PIMAGE_RESOURCE_DATA_ENTRY)(pResourceEntryTable[i].OffsetToData +
				(DWORD)m_MyPe->pResourceTables);
			//printf("RVA:%x ", pResourceDataTable->OffsetToData);
			//printf("大小:%x \n", pResourceDataTable->Size);
		}
	}
}

void ResourceTable::GetResourceInfo()
{
	// 第一层：资源的类型是什么（图标，位图，菜单....）
	// 获取资源目录
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = m_MyPe->pResourceTables;

	// 获取资源目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);

	//资源目录个数
	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;
	

	/*m_RootName.SetWindowTextW(L"0");
	
	WCHAR* idBuffer = DwToWchar(dwOneCount,16);
	m_RootID.SetWindowTextW(idBuffer);
*/


	HTREEITEM treeRoot;
	HTREEITEM childenRoot;
	//遍历第一层资源信息
	for (int i = 0; i < dwOneCount; i++)
	{
		// 判断资源是什么类型
		// 资源是字符串作为标识
		if (pResOneEntry[i].NameIsString)
		{
			// 如果NameIsString == 1 那么 NameOffset有效
			PIMAGE_RESOURCE_DIR_STRING_U pName =
				(PIMAGE_RESOURCE_DIR_STRING_U)(pResOneEntry[i].NameOffset + (DWORD)pResDirOne);
			// 输出资源类型名
			WCHAR wResName[100];
			wcsncpy_s(wResName, pName->NameString, pName->Length);

			HTREEITEM treeRoot = m_ResouceTree.InsertItem(wResName);//4292901760U
			//wprintf(L"%s\n", wResName);
		}
		// 资源是ID作为标识
		else {
			// 系统提供的资源类型
			if (pResOneEntry[i].Id < 0x0d)
			{
				CA2W cBuffer(resourceType[pResOneEntry[i].Id-1]);
				treeRoot = m_ResouceTree.InsertItem(cBuffer.m_szBuffer);//4292901760U
				//wprintf(L"%s\n", resourceType[pResOneEntry[i].Id]);
			}
			else if (pResOneEntry[i].Id == 0xe)
			{
				treeRoot = m_ResouceTree.InsertItem(L"图标组");
			}
			else if (pResOneEntry[i].Id == 0x10)
			{
				treeRoot = m_ResouceTree.InsertItem(L"版本信息");
			}
			else {
				char IdBuffer[0x10] = { 0 };
				_itoa_s(pResOneEntry[i].Id, IdBuffer, 16);
				CA2W cBuffer(IdBuffer);
				treeRoot = m_ResouceTree.InsertItem(cBuffer.m_szBuffer);//4292901760U
				//wprintf(L"%08x\n", pResOneEntry[i].Id);
			}

		}
		// 第二次：资源的叫什么名字 （1.png, 2.png）
		// 是否有第二层目录
		if (pResOneEntry[i].DataIsDirectory) {

			// 找到第二层目录表
			PIMAGE_RESOURCE_DIRECTORY pResDirTwo =
				(PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);

			// 找到第二层目录项
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoEntry =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);

			// 获取这种资源有多少个
			DWORD ResTowCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
			for (int j = 0; j < ResTowCount; j++)
			{
				// 资源是字符串作为标识
				if (pResTwoEntry[j].NameIsString)
				{
					// 如果NameIsString == 1 那么 NameOffset有效
					PIMAGE_RESOURCE_DIR_STRING_U pName =
						(PIMAGE_RESOURCE_DIR_STRING_U)(pResTwoEntry[j].NameOffset + (DWORD)pResDirOne);
					// 输出资源类型名
					WCHAR wResName[100];
					wcsncpy_s(wResName, pName->NameString, pName->Length);
					childenRoot = m_ResouceTree.InsertItem(wResName, treeRoot);//4292901760U
				}
				else {
					char IdBuffer[0x10] = { 0 };
					_itoa_s(pResTwoEntry[j].Id, IdBuffer, 16);
					CA2W cBuffer(IdBuffer);
					childenRoot = m_ResouceTree.InsertItem(cBuffer.m_szBuffer, treeRoot);//4292901760U
				}

				// 第三层：资源的语言，资源的信息（大小，文件中位置）
				if (pResTwoEntry[j].DataIsDirectory)
				{
					// 获取第三层目录表
					PIMAGE_RESOURCE_DIRECTORY pResDirThree =
						(PIMAGE_RESOURCE_DIRECTORY)(pResTwoEntry[j].OffsetToDirectory + (DWORD)pResDirOne);
					// 获取第三层目录项
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThreeEntry =
						(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirThree + 1);

					// 获取资源真正信息
					PIMAGE_RESOURCE_DATA_ENTRY pResData =
						(PIMAGE_RESOURCE_DATA_ENTRY)(pResThreeEntry->OffsetToData + (DWORD)pResDirOne);

					char IdBuffer[0x10] = { 0 };
					_itoa_s(pResData->OffsetToData, IdBuffer, 16);
					CA2W cBuffer(IdBuffer);
					WCHAR RVABuffer[0x20] = L"RVA:";
					wcscat(RVABuffer, cBuffer.m_szBuffer);
					m_ResouceTree.InsertItem(RVABuffer, childenRoot);//4292901760U

					_itoa_s(pResData->Size, IdBuffer, 16);
					CA2W caBuffer(IdBuffer);
					WCHAR SizeBuffer[0x20] = L"SIZE:";
					wcscat(SizeBuffer, caBuffer.m_szBuffer);
					m_ResouceTree.InsertItem(SizeBuffer, childenRoot);
				}
			}
		}
	}
}

//WCHAR * DwToWchar(DWORD dwNum,int radix)
//{
//	char cBuffer[0x10] = { 0 };
//	_itoa_s(dwNum, cBuffer, radix);
//	CA2W cwBuffer(cBuffer);
//	return cwBuffer.m_szBuffer;
//}
