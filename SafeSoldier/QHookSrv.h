#pragma once
#include <TlHelp32.h>
#include <Dbghelp.h>


class QHookSrv
{
public:
	// Hook a function in all modules
	QHookSrv(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook,
		BOOL fExcludeAPIHookMod);

	// Unhook a function from all modules
	QHookSrv();
	~QHookSrv();

	// Returns the original address of the hooked function
	operator PROC() { return(m_pfnOrig); }

private:
	static PVOID sm_pvMaxAppAddr; // Maximum private memory address
	static QHookSrv* sm_pHead;    // Address of first object
	QHookSrv* m_pNext;            // Address of next  object

	PCSTR m_pszCalleeModName;     // Module containing the function (ANSI)
	PCSTR m_pszFuncName;          // Function name in callee (ANSI)
	PROC  m_pfnOrig;              // Original function address in callee
	PROC  m_pfnHook;              // Hook function address
	BOOL  m_fExcludeAPIHookMod;   // Hook module w/CAPIHook implementation?

private:
	// Replaces a symbol's address in a module's import section
	static void WINAPI ReplaceIATEntryInAllMods(PCSTR pszCalleeModName,
		PROC pfnOrig, PROC pfnHook, BOOL fExcludeAPIHookMod);

	// Replaces a symbol's address in all module's import sections
	static void WINAPI ReplaceIATEntryInOneMod(PCSTR pszCalleeModName,
		PROC pfnOrig, PROC pfnHook, HMODULE hmodCaller);
};
