//
// Created by baptiste on 9/25/17.
//

#include <future>
#include "Server.hpp"
#include "UserPreferences.hpp"

using boost::asio::ip::tcp;

Server::Server(int port, const DatabaseInfo &DBPreferences,
	       const std::string &LSPreferences,
	       const enum STORAGE_TYPE storageType)
  : _DB(boost::make_shared<DatabaseHandler>()),
    _localPathStorage(LSPreferences),
    _storageType(storageType),
    _acceptor(_io, tcp::endpoint(tcp::v4(), port)),
    _context(_io, boost::asio::ssl::context::sslv23)
{
  _context.set_options(
    boost::asio::ssl::context::default_workarounds
    | boost::asio::ssl::context::no_sslv2
    | boost::asio::ssl::context::single_dh_use);
  _context.set_password_callback(boost::bind(&Server::getPassword, this));
  _context.use_certificate_chain_file("rootCA.pem");
  _context.use_private_key_file("root.pem", boost::asio::ssl::context::pem);
  if (_DB->addDatabase(std::move(DBPreferences)) == -1)
     if (_DB->addDatabase(std::move(DatabaseInfo{HOST, PORT, DATABASE, USER, PASSWORD})) == -1)
       throw std::runtime_error("Unable to connect to any database");
  createCommandAcceptor();
  startAccept();
  watchUserQuit();
}

void 	Server::close(const std::string &id)
{
  std::find_if(_sessions.begin(), _sessions.end(),
	       [&id](boost::shared_ptr<LogSession> &session)
	       { return  *(session.get()) == id; })->get()->stop();
  _sessions.erase(std::remove_if(_sessions.begin(), _sessions.end(),
				 [&id](boost::shared_ptr<LogSession> &session)
				 { return  *(session.get()) == id; }),
		  _sessions.end());
};

void	Server::closeAll()
{
  std::for_each(_sessions.begin(), _sessions.end(),
		[](boost::shared_ptr<LogSession> &session)
		{ session->stop(); });
  _sessions.clear();
  std::cout << "**\t\033[1;31mAll connections to server have been closed\033[0m\t**" << std::endl;
};

void	Server::userQuitCallback()
{
  if (_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
  {
    closeAll();
    _io.stop();
    return ;
  }
  watchUserQuit();
}

void 	Server::watchUserQuit() {
  boost::asio::deadline_timer _timer(_io, boost::posix_time::seconds(5));
  _timer.async_wait(boost::bind(&Server::userQuitCallback, this));
}

void 	Server::createCommandAcceptor()
{
  CommandAcceptor commandAcceptor(&_sessions, std::ref(_DB));

  _future = std::async(std::launch::async, &CommandAcceptor::run, std::move(commandAcceptor));
}

void 	Server::startAccept()
{
  SSLConnection::sharedConnectionPtr connection =
  boost::make_shared<SSLConnection>(_acceptor.get_io_service(), _context);

  _acceptor.async_accept(connection->getSocket(),
			boost::bind(&Server::handleAccept, this, connection,
				    boost::asio::placeholders::error));
}

void 	Server::handleAccept(SSLConnection::sharedConnectionPtr connection,
			  const boost::system::error_code &error)
{
  boost::remove_erase_if(_sessions,
			 [](const boost::shared_ptr<LogSession> &session)
			 { return !session->isLive(); });
  if (!error)
  {
    _sessions.push_back(LogSession::create(connection, std::ref(_DB), _localPathStorage, _storageType));
    _sessions.back()->run();
    startAccept();
  }
}

int main()
{
  try
  {
    UserPreferences userPref;
    userPref.getUserPreferences();

    Server server(7171, userPref.getDatabase(), userPref.getLocalStoragePath(), userPref.getStorage());
    server.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return (0);
}