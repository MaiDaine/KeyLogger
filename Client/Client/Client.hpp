#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <istream>
#include <string>
#include <openssl\x509.h>
#include <stdio.h>
#include <tchar.h>
#include "Serial.hpp"
#include "SSLConnection.hpp"
#include "Packet.hpp"
#include "debugProtect.h"
#include "Interpreter.hpp"

using boost::asio::ip::tcp;

typedef long long nez_de_david;

class LoggerHandler;

class Client
{
public:
	Client();
	~Client();

	template<typename T>
	void sendLog(const T& data) {
		_waitingToSend = true;
		OUTBOUNDPacket = data;	
		_SSLHandler->getSocket().cancel();
	}

	void hookLogger(LoggerHandler *logger) { _logger = logger; }

	void moveOn(bool);
	void log(const Packet::PacketType &);
	void uninstall();
	void stopSession();

	static size_t ReDialTime;

private:

	void startAll();
	void connect();
	bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx);

	void tryReconnect(const boost::system::error_code &error, const boost::shared_ptr<boost::asio::deadline_timer>& _timer);

	void connectCallback(const boost::system::error_code& error);
	void handshakeCallback(const boost::system::error_code& error);
	void readCallback(const boost::system::error_code &error);
	void writeCallback(const boost::system::error_code& error);

	std::string GetMACaddress(void);

private:
	boost::asio::io_service _io;
	boost::asio::ssl::context _ctx;
	LoggerHandler *_logger;
	boost::shared_ptr<SSLConnection> _SSLHandler;
	Packet MACPacket;
	Packet PINGPacket;
	Packet OUTBOUNDPacket;
	Packet INBOUNDPacket;
	boost::asio::ip::tcp::resolver::iterator _endpoint;
	std::vector<char>			_packetData;
	std::string _MAC;
	Interpreter _interpreter;
	bool _waitingToSend;
};

#endif