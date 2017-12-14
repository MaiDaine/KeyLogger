// Uninstaller.cpp : Définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include <Winerror.h>
#include <Shlobj.h>
#include <Shellapi.h>
#include <tlhelp32.h>

int main()
{
	HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	PROCESSENTRY32 process = { 0 };
	process.dwSize = sizeof(process);
	Process32First(processSnap, &process);
	if (!Process32First(processSnap, &process))
		return 0;
	do
	{
		int tmp = _tcsicmp(process.szExeFile, "SystemMgr.exe");
		if (tmp == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, process.th32ProcessID);
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
		}
	} while (Process32Next(processSnap, &process));
	CloseHandle(processSnap);
	Sleep(50);
	std::cout << "Sorry Bro :/" << std::endl;
	std::wstring pfiles(L"");
	std::string tmp("");
	WCHAR path[256];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, path))) {
		pfiles = std::wstring(path);
		tmp = std::string(pfiles.begin(), pfiles.end());
	}
	std::string certif(tmp + "\\Windows32\\rootCA.pem");
	std::string exe(tmp + "\\Windows32\\SystemMgr.exe");
	DeleteFile(certif.c_str());
	DeleteFile(exe.c_str());
	Sleep(5000);

    return 0;
}

