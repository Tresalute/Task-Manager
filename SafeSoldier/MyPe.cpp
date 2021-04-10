#include "stdafx.h"
#include "MyPe.h"


//������PE �ӿڵ�ʵ��

MyPe::MyPe(IN LPCWSTR lpFileName)
{
	readPefile(lpFileName);
	//DOS
	GetDosHeader();
	//NT
	GetNtHeader();
	//FILE
	GetFileHeader();
	//OPTIONAL
	GetOptionalHeader();
	//SECTION
	GetSectionHeader();
	//EXPORT
	GetExportTable();
	//IMPORT
	GetImportTable();
	//RESOURCE
	DWORD dwResourceRVA = pOptionalHeader->DataDirectory[2].VirtualAddress;
	GetResourceTable(dwResourceRVA);
	//RELOCAL
	GetReLocalTAble();
	//TLS
	GetTlsTable();
	//DELAY
	GetDelayLoadTable();

}

MyPe::~MyPe()
{
}

DWORD MyPe::readPefile(IN LPCWSTR lpFileName)
{
	DWORD readSize = 0;
	//���ļ�
	HANDLE hFile = CreateFileW(lpFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�ļ���ʧ��\n");
		return false;
	}
	//��ȡ�ļ���С
	m_dwFileSize = GetFileSize(hFile, NULL);
	printf("�ļ���СΪ:%d KB\n", m_dwFileSize / 1024);

	m_buf = new char[m_dwFileSize];

	//��ȡ�ļ� 
	ReadFile(hFile, m_buf, m_dwFileSize, &readSize, nullptr);
	CloseHandle(hFile);
	return m_dwFileSize;
}

PIMAGE_DOS_HEADER MyPe::GetDosHeader()
{
	pDosHeader = (PIMAGE_DOS_HEADER)m_buf;
	return pDosHeader;
}

PIMAGE_NT_HEADERS MyPe::GetNtHeader()
{
	pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader->e_lfanew + (DWORD)m_buf);
	return pNtHeader;
}

PIMAGE_FILE_HEADER MyPe::GetFileHeader()
{
	pFileHeader = (PIMAGE_FILE_HEADER)(&pNtHeader->FileHeader);
	return pFileHeader;
}

PIMAGE_OPTIONAL_HEADER MyPe::GetOptionalHeader()
{
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)(&pNtHeader->OptionalHeader);
	return pOptionalHeader;
}

PIMAGE_SECTION_HEADER MyPe::GetSectionHeader()
{
	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	return pSectionHeader;
}

PIMAGE_EXPORT_DIRECTORY MyPe::GetExportTable()
{
	DWORD FOA = RVA2FOA(pOptionalHeader->DataDirectory[0].VirtualAddress);
	pExportTable = (PIMAGE_EXPORT_DIRECTORY)(FOA + (DWORD)pDosHeader);

	//if (FOA == NULL)
	//{
	//	printf("�޵�����\n");
	//	return pExportTable;
	//}
	//printf("Base:%d\n", pExportTable->Base);
	//printf("NumberOfFunctions:%d\n", pExportTable->NumberOfFunctions);
	//printf("NumberOfNames:%d\n", pExportTable->NumberOfNames);
	//printf("��ַ��			");
	//DWORD FOA2 = RVA2FOA(pExportTable->AddressOfFunctions);
	//DWORD* addTable = (DWORD*)(FOA2 + (DWORD)pDosHeader);
	////for (size_t i = 0; i < pExportTable->NumberOfFunctions; i++)
	////{
	////	printf("%x\n", addTable[i]);
	////}
	//printf("��ű�			");
	//DWORD FOA3 = RVA2FOA(pExportTable->AddressOfNameOrdinals);
	//WORD* indexTable = (WORD*)(FOA3 + (DWORD)pDosHeader);
	////for (size_t i = 0; i < pExportTable->NumberOfNames; i++)
	////{
	////	printf("%x\n", indexTable[i]+pExportTable->Base);
	////}
	//printf("������\n");
	//DWORD FOA4 = RVA2FOA(pExportTable->AddressOfNames);
	//DWORD* NameTable = (DWORD*)(FOA4 + (DWORD)pDosHeader);
	////for (size_t i = 0; i < pExportTable->NumberOfNames; i++)
	////{
	////	//������
	////	DWORD tmpFOA = RVA2FOA(NameTable[i]);
	////	char* Name = (char*)(tmpFOA + (DWORD)pDosHeader);
	////	printf("%s ", Name);
	////	//��ű�
	////	printf("%14x ", indexTable[i] + pExportTable->Base);
	////	//��ַ
	////	printf("%26x \n", addTable[indexTable[i]]);
	////}
	//for (size_t i = 0; i < pExportTable->NumberOfFunctions; i++)
	//{
	//	if (addTable[i] == 0)
	//	{
	//		continue;
	//	}
	//	//������ַ
	//	printf("%x ", addTable[i]);
	//	//�������
	//	//������
	//	BOOL bButton = TRUE;
	//	for (size_t j = 0; j < pExportTable->NumberOfNames; j++)
	//	{
	//		if (indexTable[j] == i)
	//		{
	//			printf("%x ", indexTable[i] + pExportTable->Base);
	//			DWORD tmpFOA = RVA2FOA(NameTable[i]);
	//			char* Name = (char*)(tmpFOA + (DWORD)pDosHeader);
	//			printf("%s \n", Name);
	//			bButton = FALSE;
	//			break;
	//		}
	//	}
	//	if (bButton)
	//	{
	//		printf("null ");
	//		printf("null \n");
	//	}
	//}
	return pExportTable;
}

PIMAGE_IMPORT_DESCRIPTOR MyPe::GetImportTable()
{
	DWORD FOA =
		RVA2FOA(pOptionalHeader->DataDirectory[1].VirtualAddress);
	pImportTable =
		(PIMAGE_IMPORT_DESCRIPTOR)(FOA + (DWORD)pDosHeader);
	PIMAGE_IMPORT_DESCRIPTOR tmpPImportTable = pImportTable;
	////size_t numOfImportTable = pOptionalHeader->DataDirectory[1].Size / sizeof(IMAGE_IMPORT_DESCRIPTOR);
	//printf("�����");
	////for (size_t i = 0; i < numOfImportTable; i++);
	//while (pImportTable->Name != NULL)
	//{
	//	printf("�����PE�ļ����ƣ�");
	//	char * dwName = (char *)(RVA2FOA(pImportTable->Name) + (DWORD)pDosHeader);
	//	printf("%s\n", dwName);
	//	printf("����ĺ���\n");
	//	//DWORD FirstThunk_FOA = RVA2FOA(pImportTable->OriginalFirstThunk);//INT
	//	DWORD FirstThunk_FOA = RVA2FOA(pImportTable->FirstThunk);//IAT
	//	PIMAGE_THUNK_DATA thunk =
	//		(PIMAGE_THUNK_DATA)(FirstThunk_FOA + (DWORD)pDosHeader);
	//	printf("Hint  Name \n");
	//	while (thunk->u1.Ordinal)
	//	{
	//		if (!IMAGE_SNAP_BY_ORDINAL(thunk->u1.Ordinal))
	//		{
	//			PIMAGE_IMPORT_BY_NAME functionInfo =
	//				(PIMAGE_IMPORT_BY_NAME)(RVA2FOA(thunk->u1.AddressOfData) + (DWORD)pDosHeader);
	//			printf("%04x %s\n", functionInfo->Hint, functionInfo->Name);
	//		}
	//		else
	//		{
	//			printf("%04x %s\n", thunk->u1.Ordinal & 0xffff, "(NULL)");
	//		}
	//		thunk++;
	//	}
	//	pImportTable++;
	//}
	return pImportTable;
}

PIMAGE_RESOURCE_DIRECTORY MyPe::GetResourceTable(DWORD dwResourceTableRva)
{

	DWORD FOA = RVA2FOA(dwResourceTableRva);
	pResourceTables = (PIMAGE_RESOURCE_DIRECTORY)(FOA + (DWORD)pDosHeader);

	/*size_t szNumOfResource =
		pResourceTables->NumberOfNamedEntries +
		pResourceTables->NumberOfIdEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntryTable =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)((BYTE*)pResourceTables +
			sizeof(IMAGE_RESOURCE_DIRECTORY));
	printf("\nReLocal\n");
	for (size_t i = 0; i < szNumOfResource; i++)
	{
		if (pResourceEntryTable[i].NameIsString == 0)
		{
			if (pResourceEntryTable[i].Id < 0x10)
			{
				printf(" %s ", resourceType[pResourceEntryTable[i].Id - 1]);
			}
			else printf(" %x ", pResourceEntryTable[i].Id);
		}
		else
		{
			PIMAGE_RESOURCE_DIR_STRING_U pResourceDirTable =
				(PIMAGE_RESOURCE_DIR_STRING_U)(pResourceEntryTable[i].NameOffset +
				(DWORD)pResourceTables);
			size_t lenth = pResourceDirTable->Length + 1;
			wchar_t* sourceName = new wchar_t[lenth];
			wmemset(sourceName, 0, lenth);
			wmemcpy(sourceName, pResourceDirTable->NameString, pResourceDirTable->Length);
			printf("����:%s", CW2A(sourceName).m_szBuffer);
		}
		if (pResourceEntryTable[i].DataIsDirectory == 1)
		{
			GetResourceTable(pResourceEntryTable[i].OffsetToDirectory +
				(DWORD)pOptionalHeader->DataDirectory[2].VirtualAddress);
		}
		else
		{
			PIMAGE_RESOURCE_DATA_ENTRY pResourceDataTable =
				(PIMAGE_RESOURCE_DATA_ENTRY)(pResourceEntryTable[i].OffsetToData +
				(DWORD)pResourceTables);
			printf("RVA:%x ", pResourceDataTable->OffsetToData);
			printf("��С:%x \n", pResourceDataTable->Size);
		}
	}*/
	return pResourceTables;
}

PIMAGE_BASE_RELOCATION MyPe::GetReLocalTAble()
{
	typedef struct MyStruct
	{
		WORD offset : 12;
		WORD type : 4;
	}*pMyStruct;

	pReLocalTables =
		(PIMAGE_BASE_RELOCATION)(RVA2FOA(pOptionalHeader->DataDirectory[5].VirtualAddress) +
		(DWORD)pDosHeader);
	/*PIMAGE_BASE_RELOCATION pReLocalTable = pReLocalTables;

	while (pReLocalTable->VirtualAddress != NULL)
	{
		printf("VirtualAddress:%x\n", pReLocalTable->VirtualAddress);
		size_t szNumOfTable = (pReLocalTable->SizeOfBlock - 8) / 2;
		pMyStruct ReLocal = (pMyStruct)(pReLocalTable + 1);
		for (size_t i = 0; i < szNumOfTable; i++, ReLocal++)
		{
			if (ReLocal->type == 0x3)
			{
				printf("offset:0x%x \n", ReLocal->offset);
				printf("RVA:0x%x \n", ReLocal->offset + pReLocalTable->VirtualAddress);
				printf("FOA:0x%x \n", RVA2FOA(ReLocal->offset + pReLocalTable->VirtualAddress));
			}
		}
		pReLocalTable = (PIMAGE_BASE_RELOCATION)((BYTE*)pReLocalTable + pReLocalTable->SizeOfBlock);
	}*/

	return pReLocalTables;
}

PIMAGE_TLS_DIRECTORY MyPe::GetTlsTable()
{
	pTLSTables =
		(PIMAGE_TLS_DIRECTORY)(RVA2FOA(pOptionalHeader->DataDirectory[9].VirtualAddress) +
		(DWORD)pDosHeader);
	/*pTLSTables->StartAddressOfRawData;
	printf("StartAddressOfRawData:%x\n", pTLSTables->StartAddressOfRawData);
	DWORD StartAddressOfRawData =
		RVA2FOA(pTLSTables->StartAddressOfRawData - pOptionalHeader->ImageBase) + (DWORD)pDosHeader;
	printf("EndAddressOfRawData:%x\n", pTLSTables->EndAddressOfRawData);
	DWORD EndAddressOfRawData =
		RVA2FOA(pTLSTables->EndAddressOfRawData - pOptionalHeader->ImageBase) + (DWORD)pDosHeader;

	DWORD* pAddressOfCallBacks =
		(DWORD*)(RVA2FOA(pTLSTables->AddressOfCallBacks - pOptionalHeader->ImageBase) + (DWORD)pDosHeader);


	while (*pAddressOfCallBacks != NULL)
	{
		printf("C:%x\n", *(pAddressOfCallBacks++));
	}*/

	return PIMAGE_TLS_DIRECTORY();
}

PIMAGE_DELAYLOAD_DESCRIPTOR MyPe::GetDelayLoadTable()
{
	pDelayLoad =
		(PIMAGE_DELAYLOAD_DESCRIPTOR)(RVA2FOA(pOptionalHeader->DataDirectory[13].VirtualAddress) +
		(DWORD)pDosHeader);
	return PIMAGE_DELAYLOAD_DESCRIPTOR();
}

CHAR * MyPe::FileBufferToImageBuffer()
{
	//���ٴ洢�ռ�
	char* imageBuffer = new char[pOptionalHeader->SizeOfImage];
	memset(imageBuffer, 0, pOptionalHeader->SizeOfImage);
	//����ͷ
	memcpy(imageBuffer, m_buf, pOptionalHeader->SizeOfHeaders);
	//������
	auto pTmpSecHeader = pSectionHeader;
	for (size_t i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		char* pSectionRaw = nullptr;
		pSectionRaw = (char*)(pTmpSecHeader->PointerToRawData + (DWORD)pDosHeader);
		char* pMemSection = nullptr;
		pMemSection = (char*)(pTmpSecHeader->VirtualAddress + (DWORD)imageBuffer);
		memcpy(pMemSection, pSectionRaw, pTmpSecHeader->SizeOfRawData);
		pTmpSecHeader++;
	}
	return imageBuffer;
}

BOOL MyPe::ChangeImportTable()
{
	//1.�鿴�ĸ��ֶ��г���Ŀռ�洢���е����ṹ������

	//2.�ƶ���������õ�ַ
	//3.���ӵ����ṹ
	return 0;
}

DWORD MyPe::filebuffertoimagebuffer()
{
	LPVOID pTempImageBuffer = nullptr;
	if (m_buf == nullptr)
	{
		printf("������ָ����Ч");
		return 0;
	}
	//�ж��Ƿ�Ϊ��Ч��MZ ���
	if (*(PDWORD)m_buf != IMAGE_DOS_SIGNATURE)
	{
		printf("������Ч��MZ��־");
		return 0;
	}
	pDosHeader = (PIMAGE_DOS_HEADER)m_buf;
	//�ж��Ƿ�Ϊ��Ч��PE��־
	if (*((PDWORD)((DWORD)m_buf + pDosHeader->e_lfanew)) != IMAGE_NT_SIGNATURE)
	{
		printf("������Ч��pe��־");
	}
	pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)m_buf + pDosHeader->e_lfanew);
	pFileHeader = (PIMAGE_FILE_HEADER)(((DWORD)pNtHeader) + 4);
	//��ѡPE ͷ
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	//��һ����Ŀ¼ָ��
	//pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

}

DWORD MyPe::RVA2FOA(DWORD RVA)
{
	//1.�ж����Ǹ�����
	IMAGE_SECTION_HEADER pTmpSectionHeader = { 0 };
	for (size_t i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (pSectionHeader[i].VirtualAddress <= RVA &&
			RVA < pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData)
		{
			pTmpSectionHeader = pSectionHeader[i];
			break;
		}
	}
	//2.RVA >> FOA
	DWORD FOA = RVA - pTmpSectionHeader.VirtualAddress + pTmpSectionHeader.PointerToRawData;
	return FOA;
}

WCHAR * DwToWchar(DWORD dwNum, int radix)
{
	char cBuffer[0x10] = { 0 };
	_itoa_s(dwNum, cBuffer, radix);
	CA2W cwBuffer(cBuffer);
	WCHAR* wcBuffer = new WCHAR[0x10];
	cwBuffer.m_szBuffer;
	wmemcpy(wcBuffer, cwBuffer.m_szBuffer, 0x10);
	return wcBuffer;
}