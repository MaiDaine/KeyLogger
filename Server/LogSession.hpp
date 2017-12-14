//
// Created by baptiste on 9/26/17.
//

#ifndef SPIDER_LOGSESSION_HPP
# define SPIDER_LOGSESSION_HPP

#include "Boost.hpp"
#include "SSLConnectionServer.hpp"
#include "Packet.hpp"
#include "Serial.hpp"
#include "PacketTranslator.hpp"

class LogSession : public boost::enable_shared_from_this<LogSession>
{
  public:
    static boost::shared_ptr<LogSession> create(boost::shared_ptr<SSLConnection> handledClient,
						boost::shared_ptr<DatabaseHandler> &DB,
    						const std::string &localStorage,
						const STORAGE_TYPE storageType)
    { return boost::shared_ptr<LogSession>(new LogSession(handledClient, DB, localStorage, storageType)); };

    bool				operator==(const std::string &id) { return _id == id; }

    bool 				isLive() const { return _live; }
    void 				stop() { _handledClient->getSocket().close(); }
    void 				run();

    void 				setId(const std::string & id) { _id = id; }
    const std::string 			&getId() const { return _id; }

    template <typename T>
    boost::system::error_code		sendRequest(const T & data) const
    { return (_handledClient->sync_write(data)); }

    void	changeStorageMethod(STORAGE_TYPE method) { _packetTranslator.getDataStorage().setStorageType(method); }
    bool	setLocalStorage(const std::string &f) { return (_packetTranslator.getDataStorage().setFile(f)); }
    bool	setDB(int id) { return (_packetTranslator.getDataStorage().setDatabase(id) != -1); }
    bool	setDB(const DatabaseInfo &dbI) { return (_packetTranslator.getDataStorage().setDatabase(dbI) != -1); }

  private:
    LogSession(boost::shared_ptr<SSLConnection> &handledClient,
	       boost::shared_ptr<DatabaseHandler> &DB,
	       const std::string &localStorage,
	       const STORAGE_TYPE storageType);

    bool 	errorHandler(const boost::system::error_code &error);
    void 	packetReceiver(const boost::system::error_code &error);
    void 	packetReader(const boost::system::error_code &error);

  private:
    Packet 					_inboundPacket;
    Packet 					_outboundPacket;
    boost::shared_ptr<SSLConnection> 		_handledClient;
    PacketTranslator				_packetTranslator;
    bool					_live;
    std::string					_errorAnswer;
    std::string					_okAnswer;
    std::string 				_id;
};


#endif /* !SPIDER_LOGSESSION_HPP */
