#include <functional>
#include "debugProtect.h"

debugProtect::debugProtect()
{
}

debugProtect::~debugProtect()
{
}

LONG debugProtect::UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)const
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)pExcepPointers->ContextRecord->Eax);
	pExcepPointers->ContextRecord->Eip += 2;

	__asm {xor eax, eax}
	__asm {div eax}

	return EXCEPTION_CONTINUE_EXECUTION;
}

inline void debugProtect::DebugSelf()const
{
	HANDLE hProcess = NULL;
	DEBUG_EVENT de;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&de, sizeof(DEBUG_EVENT));

	GetStartupInfo(&si);

	CreateProcess(NULL, GetCommandLine(), NULL, NULL, FALSE, DEBUG_PROCESS, NULL, NULL, &si, &pi);
	ContinueDebugEvent(pi.dwProcessId, pi.dwThreadId, DBG_CONTINUE);
	WaitForDebugEvent(&de, INFINITE);
}

inline bool debugProtect::HideThread(HANDLE hThread)const
{
	typedef NTSTATUS(NTAPI *pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);
	NTSTATUS Status;

	pNtSetInformationThread NtSIT = (pNtSetInformationThread)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "NtSetInformationThread");
	if (NtSIT == NULL)
		return false;
	if (hThread == NULL)
		Status = NtSIT(GetCurrentThread(),0x11,0, 0);
	else
		Status = NtSIT(hThread, 0x11, 0, 0);
	if (Status != 0x00000000)
		return false;
	else
		return true;
}

void debugProtect::ProtectMe(void)const
{
	this->DebugSelf();
	this->HideThread(NULL);
}