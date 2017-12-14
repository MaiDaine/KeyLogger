#include <SDKDDKVer.h>
#include "Client.hpp"
#include <Shlobj.h>
#include <Windows.h>
#include "resource.h"

debugProtect dbg;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::string logPath;
	WCHAR path[256];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path))) {
		std::wstring pfiles(path);
		logPath = std::string(pfiles.begin(), pfiles.end()) + "\\AppData";
	}
	if (CreateDirectory(logPath.c_str(), NULL))
	{
		logPath += "\\Roaming";
		CreateDirectory(logPath.c_str(), NULL);
	}
	SetUnhandledExceptionFilter(UnhandledExcepFilter<dbg>);
	dbg.ProtectMe();
	try {
		Client client;
	}
	catch (std::exception&) {
		return 1;
	}
	return 0;
}