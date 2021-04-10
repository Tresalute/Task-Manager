// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TlHelp32.h>
#include <stdlib.h>


#define DESPATH L"SafeSoldier.exe"


unsigned char code[12];

unsigned char oldcode[12];

FARPROC addr;

DWORD pid;
//获取PID
DWORD GetPID(const WCHAR* name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	int flag = Process32First(hSnapshot, &pe);

	while (flag != 0)
	{
		if (wcscmp(pe.szExeFile, name) == 0)
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
		flag = Process32NextW(hSnapshot, &pe);
	}
	CloseHandle(hSnapshot);
	return 0;
}

//通过打开文件映射的方式获取PID
DWORD FromFileGetPID()
{
	HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"File_Protecte_Mapping");
	if (!hMap) return 0;
	PVOID pBuf = MapViewOfFile(hMap, FILE_ALL_ACCESS, 0, 0, 16);
	if (!pBuf) return 0;
	DWORD PID = 0;
	PID = atoi((const char*)pBuf);
	//修改数据
	*((PBYTE)pBuf) = 0;

	UnmapViewOfFile(pBuf);
	CloseHandle(hMap);
	return PID;
}

//构建自己的 OpenProcess
HANDLE WINAPI MyOpenProcess(DWORD dwDesiredAccess,
	BOOL bInheritHandle, DWORD dwProcessID)
{
	HANDLE handle;
	// 静态获取PID
	// 被关闭的PID 与 被保护的PID 相同则提示错误
	if (GetPID(DESPATH) == dwProcessID)
	{
		SetLastError(5);
		return NULL;
	}


	// 被关闭的PID 与 被保护的PID 相同则提示错误
	DWORD old;
	if (VirtualProtectEx(GetCurrentProcess(), addr, 12,
		PAGE_EXECUTE_READWRITE, &old))
	{
		WriteProcessMemory(GetCurrentProcess(), addr, oldcode, 12, NULL);
		VirtualProtectEx(GetCurrentProcess(), addr, 12, old, &old);
	}
	handle = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessID);
	if (VirtualProtectEx(GetCurrentProcess(), addr, 12,
		PAGE_EXECUTE_READWRITE, &old))
	{
		WriteProcessMemory(GetCurrentProcess(), addr, code, 12, NULL);
		VirtualProtectEx(GetCurrentProcess(), addr, 12, old, &old);
	}
	return handle;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		addr = 0;
		HMODULE hdll;
		//加载Kernel32 通过获取到的句柄 获得OpenProcess的在内存中地址
 		hdll = LoadLibrary(TEXT("Kernel32.dll"));
		addr = GetProcAddress(hdll, "OpenProcess");
		if (addr)
		{
			code[0] = 0x48;
			code[1] = 0xB8;		// jmp
			code[10] = 0x50;
			code[11] = 0xC3;	
			long long a = (long long)MyOpenProcess;
			RtlMoveMemory(code + 2, &a, 8);// 64位地址 所以是8个字节

			DWORD old;
			if (VirtualProtectEx(GetCurrentProcess(), addr, 12, PAGE_EXECUTE_READWRITE, &old)) 
			{
				RtlMoveMemory(oldcode, addr, 12);
				WriteProcessMemory(GetCurrentProcess(), addr, code, 12, NULL);
				VirtualProtectEx(GetCurrentProcess(), addr, 12, old, &old);
			}
		}
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

