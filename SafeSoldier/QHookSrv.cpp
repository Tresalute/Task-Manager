#include "stdafx.h"
#include "QHookSrv.h"


QHookSrv::QHookSrv()
{
}


QHookSrv::~QHookSrv()
{
}

PVOID QHookSrv::sm_pvMaxAppAddr = NULL;
const BYTE cPushOpCode = 0x68;   // The PUSH opcode on x86 platforms
QHookSrv* QHookSrv::sm_pHead = NULL;
QHookSrv::QHookSrv(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook, BOOL fExcludeAPIHookMod)
{
	if (sm_pvMaxAppAddr == NULL)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		sm_pvMaxAppAddr = si.lpMaximumApplicationAddress;
	}
	m_pNext = sm_pHead;    // The next node was at the head
	sm_pHead = this;        // This node is now at the head
	// Save information about this hooked function
	m_pszCalleeModName = pszCalleeModName;
	m_pszFuncName = pszFuncName;
	m_pfnHook = pfnHook;
	m_fExcludeAPIHookMod = fExcludeAPIHookMod;
	m_pfnOrig = GetProcAddress(GetModuleHandleA(pszCalleeModName), m_pszFuncName);
	if (m_pfnOrig > sm_pvMaxAppAddr) {
		// The address is in a shared DLL; the address needs fixing up 
		PBYTE pb = (PBYTE)m_pfnOrig;
		if (pb[0] == cPushOpCode) {
			// Skip over the PUSH op code and grab the real address
			PVOID pv = *(PVOID*)&pb[1];
			m_pfnOrig = (PROC)pv;
		}
	}
	// Hook this function in all currently loaded modules
	ReplaceIATEntryInAllMods(m_pszCalleeModName, m_pfnOrig, m_pfnHook,
		m_fExcludeAPIHookMod);
}
QHookSrv::~QHookSrv()
{
	// Unhook this function from all modules
	ReplaceIATEntryInAllMods(m_pszCalleeModName, m_pfnHook, m_pfnOrig, m_fExcludeAPIHookMod);
	// Remove this object from the linked list
	QHookSrv* p = sm_pHead;
	if (p == this) {     // Removing the head node
		sm_pHead = p->m_pNext;
	}
	else {
		BOOL fFound = FALSE;
		// Walk list from head and fix pointers
		for (; !fFound && (p->m_pNext != NULL); p = p->m_pNext) {
			if (p->m_pNext == this) {
				// Make the node that points to us point to the our next node
				p->m_pNext = p->m_pNext->m_pNext;
				break;
			}
		}
	}
}
static HMODULE ModuleFromAddress(PVOID pv)
{
	MEMORY_BASIC_INFORMATION mbi;
	return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
		? (HMODULE)mbi.AllocationBase : NULL);
}
void QHookSrv::ReplaceIATEntryInAllMods(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, BOOL fExcludeAPIHookMod)
{
	HMODULE hmodThisMod = fExcludeAPIHookMod ? ModuleFromAddress(ReplaceIATEntryInAllMods) : NULL;
	HANDLE m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (m_hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me = { sizeof(me) };
		for (BOOL fOk = Module32First(m_hSnapshot, &me); fOk; fOk = Module32Next(m_hSnapshot, &me))
		{
			// NOTE: We don't hook functions in our own module
			if (me.hModule != hmodThisMod)
			{
				// Hook this function in this module
				ReplaceIATEntryInOneMod(pszCalleeModName, pfnCurrent, pfnNew, me.hModule);
			}
		}
		CloseHandle(m_hSnapshot);
	}
}
void QHookSrv::ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
	PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller) {
	// Get the address of the module's import section
	ULONG ulSize;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
	if (pImportDesc == NULL)
		return;  // This module has no import section

	 // Find the import descriptor containing references to callee's functions
	for (; pImportDesc->Name; pImportDesc++) {
		PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);
		if (lstrcmpiA(pszModName, pszCalleeModName) == 0)
			break;   // Found
	}
	if (pImportDesc->Name == 0)
		return;  // This module doesn't import any functions from this callee
	 // Get caller's import address table (IAT) for the callee's functions
	PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
		((PBYTE)hmodCaller + pImportDesc->FirstThunk);
	// Replace current function address with new function address
	for (; pThunk->u1.Function; pThunk++) {
		// Get the address of the function address
		PROC* ppfn = (PROC*)&pThunk->u1.Function;
		// Is this the function we're looking for?
		BOOL fFound = (*ppfn == pfnCurrent);
		if (!fFound && (*ppfn > sm_pvMaxAppAddr)) {
			// If this is not the function and the address is in a shared DLL, 
			// then maybe we're running under a debugger on Windows 98. In this 
			// case, this address points to an instruction that may have the 
			// correct address.
			PBYTE pbInFunc = (PBYTE)*ppfn;
			if (pbInFunc[0] == cPushOpCode) {
				// We see the PUSH instruction, the real function address follows
				ppfn = (PROC*)&pbInFunc[1];
				// Is this the function we're looking for?
				fFound = (*ppfn == pfnCurrent);
			}
		}
		if (fFound) {
			// The addresses match, change the import section address
			WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew,
				sizeof(pfnNew), NULL);
			return;  // We did it, get out
		}
	}
}