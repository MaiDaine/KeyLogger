#ifndef SYSTEMLOGGER_HPP_
 #define SYSTEMLOGGER_HPP_

#include <Windows.h>
#include "ALogger.hpp"

class LoggerHandler;

class SystemLogger : public ALogger
{
public:
	SystemLogger();
	SystemLogger(LoggerHandler *handler);
	~SystemLogger();

	static VOID CALLBACK	WinEventProcCallback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent,
		HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
	
	bool			start();
	void			stop()const;
	std::string		getLog();
	std::string		getNameForScreenshot()const;

private:
	static LoggerHandler	*_handler;
	HWINEVENTHOOK			_systemHook;
	static HWND				_hwnd;
	const std::string		_flag = "\a";

	std::string				getProcName()const;
	std::string				getWindowName()const;
	std::string				getDate()const;
};

#endif /* !SYSTEMLOGGER_HPP_ */
