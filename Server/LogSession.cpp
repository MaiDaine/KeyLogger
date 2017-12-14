//
// Created by baptiste on 9/26/17.
//

#include "LogSession.hpp"
#include "PacketTranslator.hpp"

LogSession::LogSession(boost::shared_ptr<SSLConnection> &handledClient,
		       boost::shared_ptr<DatabaseHandler> &DB,
			const std::string &localStorage,
			const STORAGE_TYPE storageType)
      : _handledClient(handledClient),
	_packetTranslator(this, DB, localStorage, storageType),
	_live(true),
	_errorAnswer("KO"),
	_okAnswer("OK"),
	_id("Unknown")
{
};

void 		LogSession::run()
{
//  std::cout << "I'm in run" << std::endl;
  _handledClient->handShake(boost::bind(&LogSession::packetReceiver,
					shared_from_this(),
					boost::asio::placeholders::error));
}

bool 		LogSession::errorHandler(const boost::system::error_code &error)
{
  if (error)
  {
    stop();
    _live = false;
    std::cerr << "Client \033[1;33m" + _id + "\033[0m has been disconnected for the following reason : \033[1;31m"
	      << error.message() << "\033[0m" << std::endl;
    return true;
  }
  return false;
}

void 		LogSession::packetReceiver(const boost::system::error_code &error)
{
  if (errorHandler(error))
    return ;
  else
  {
//    std::cout << "I'm in packetReceiver" << std::endl;
    _inboundPacket.clear();
    _handledClient->read(_inboundPacket,
			 boost::bind(&LogSession::packetReader,
				     shared_from_this(),
				     boost::asio::placeholders::error));
  }
}

void 		LogSession::packetReader(const boost::system::error_code &error)
{
  _outboundPacket.clear();
  if (errorHandler(error))
    return ;
  else
  {
    _outboundPacket.setPacketType(Packet::SERVER_RESPONSE);
    try {
      _packetTranslator.translatePacket(_inboundPacket);
      _outboundPacket << _okAnswer;
      _handledClient->write(_outboundPacket,
			    boost::bind(&LogSession::packetReceiver,
					shared_from_this(),
					boost::asio::placeholders::error));
    } catch (const std::exception & e){
      e.what();
      _outboundPacket << _errorAnswer;
      _handledClient->write(_outboundPacket,
			    boost::bind(&LogSession::packetReceiver,
					shared_from_this(),
					boost::asio::placeholders::error));
    }
  }
}