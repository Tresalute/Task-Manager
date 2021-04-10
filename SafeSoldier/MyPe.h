#pragma once
//PE ��
class MyPe
{
public:
	MyPe(IN LPCWSTR lpFileName);
	~MyPe();

	DWORD RVA2FOA(DWORD RVA);

	DWORD readPefile(IN LPCWSTR lpFileName);
	DWORD filebuffertoimagebuffer();
	PIMAGE_DOS_HEADER GetDosHeader();
	PIMAGE_NT_HEADERS GetNtHeader();
	PIMAGE_FILE_HEADER GetFileHeader();
	PIMAGE_OPTIONAL_HEADER GetOptionalHeader();
	PIMAGE_SECTION_HEADER GetSectionHeader();
	PIMAGE_EXPORT_DIRECTORY GetExportTable();
	PIMAGE_IMPORT_DESCRIPTOR GetImportTable();
	PIMAGE_RESOURCE_DIRECTORY GetResourceTable(DWORD dwResourceTableRva);
	PIMAGE_BASE_RELOCATION GetReLocalTAble();
	PIMAGE_TLS_DIRECTORY GetTlsTable();
	PIMAGE_DELAYLOAD_DESCRIPTOR GetDelayLoadTable();
	CHAR* FileBufferToImageBuffer();
	BOOL ChangeImportTable();
	//ͷ
	DWORD m_dwFileSize;
	char* m_buf = nullptr;
	PIMAGE_DOS_HEADER pDosHeader = nullptr;
	PIMAGE_NT_HEADERS pNtHeader = nullptr;
	PIMAGE_FILE_HEADER pFileHeader = nullptr;
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = nullptr;
	PIMAGE_SECTION_HEADER pSectionHeader = nullptr;
	//��
	PIMAGE_EXPORT_DIRECTORY pExportTable = nullptr;
	PIMAGE_IMPORT_DESCRIPTOR pImportTable = nullptr;
	PIMAGE_RESOURCE_DIRECTORY pResourceTables = nullptr;
	PIMAGE_BASE_RELOCATION pReLocalTables = nullptr;
	PIMAGE_TLS_DIRECTORY pTLSTables = nullptr;
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelayLoad = nullptr;

	char resourceType[0x10][0x10] = {
		"�����Դ",
		"λͼ",
		"ͼ��",
		"�˵�",
		"�Ի���",
		"�ַ�Ŀ¼",
		"����",
		"��ݼ�",
		"�Ǹ�ʽ��",
		"��Ϣ�б�",
		"���ָ������",
		"ͼ����",
		"�汾��Ϣ"
	};
};
//����ת16�����ַ�
WCHAR* DwToWchar(DWORD dwNum, int radix);