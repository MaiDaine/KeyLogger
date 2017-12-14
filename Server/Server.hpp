//
// Created by baptiste on 9/29/17.
//

#ifndef SPIDER_SERVER_HPP
# define SPIDER_SERVER_HPP

#include <thread>
#include "CommandAcceptor.hpp"
#include "DatabaseHandler.hpp"

using boost::asio::ip::tcp;

class Server
{
  public:
    ~Server() = default;
    Server(int port, const DatabaseInfo &DBPreferences,
	   const std::string &LSPreferences,
	   const enum STORAGE_TYPE storageType);

    void run() { _io.run(); };

  private:
    const std::string 	getPassword() { return "Spider"; }
    void 		createCommandAcceptor();
    void 		close(const std::string &id);
    void		closeAll();
    void		userQuitCallback();
    void 		watchUserQuit();

    void		startAccept();
    void		handleAccept(SSLConnection::sharedConnectionPtr connection,
				     const boost::system::error_code &error);

    boost::asio::io_service			_io;
    boost::shared_ptr<DatabaseHandler>		_DB;
    std::string					_localPathStorage;
    enum STORAGE_TYPE 				_storageType;
    tcp::acceptor 				_acceptor;
    std::vector<boost::shared_ptr<LogSession>>	_sessions;
    boost::asio::ssl::context 			_context;
    std::future<bool>				_future;
};

#endif /* !SPIDER_SERVER_HPP */
