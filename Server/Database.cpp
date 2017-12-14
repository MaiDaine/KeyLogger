//
// Created by wurtz on 9/25/17.
//

#include "Database.hpp"

Database::Database(const DatabaseInfo &databaseInfo)
{
    sql::ConnectOptionsMap  optionsMap;
    sql::Driver             *driver;

    optionsMap["hostName"] = databaseInfo._host;
    //optionsMap["port"] = databaseInfo._port;
    optionsMap["schema"] = databaseInfo._database;
    optionsMap["userName"] = databaseInfo._user;
    optionsMap["password"] = databaseInfo._password;
    optionsMap["OPT_RECONNECT"] = false;
    optionsMap["OPT_CONNECT_TIMEOUT"] = 1;

    driver = get_driver_instance();
  if (driver != NULL)
    _connection = driver->connect(optionsMap);
}

Database::~Database()
{
  if (_connection)
  _connection->close();
}

void                    Database::executeQuery(const std::string &query)
{
    sql::Statement      *stmt;

    stmt = _connection->createStatement();
    stmt->execute(query);
    stmt->close();
    delete (stmt);
}

sql::ResultSet          *Database::executeQueryResponse(const std::string &query)
{
    sql::Statement      *stmt;
    sql::ResultSet      *res;

    stmt = _connection->createStatement();
    res = stmt->executeQuery(query);
    stmt->close();
    delete (stmt);
    return (res);
}

void                        Database::executePostLog(const std::string &token,
                                                     std::map<std::string, std::string> data)
{
    std::ostringstream     query;

    query << "INSERT INTO logs(" << Log::delimiterString[Log::tokenDeliName] << ", " << Log::delimiterString[Log::inputTextDeliName] << ", " << Log::delimiterString[Log::inputSpecialDeliName] << ", " << Log::delimiterString[Log::dateTimeDeliName] << ", " << Log::delimiterString[Log::windowNameDeliName] << ", " << Log::delimiterString[Log::processNameDeliName] << ") VALUES('"
          << token << "', '" << data[Log::delimiterString[Log::inputTextDeliName]] << "', '" << 
data[Log::delimiterString[Log::inputSpecialDeliName]] << "', " << "NOW()" << ", '"
          << data[Log::delimiterString[Log::windowNameDeliName]] << "', '" << data[Log::delimiterString[Log::processNameDeliName]] << "');";
    try
    {
        executeQuery(query.str());
    }
    catch (const sql::SQLException &e)
    {
      std::cout << e.what() << std::endl;
    }
}

void                    Database::postUser(const std::string &token)
{
    std::ostringstream  query;

    try
    {
        if (getUser(token).empty())
        {
            query << "INSERT INTO users(" << Log::delimiterString[Log::tokenDeliName] << ", " << Log::delimiterString[Log::accountNameDeliName] << ") VALUES('" << token << "', '" << "accountName" << "');";
            executeQuery(query.str());
        }
        else
        {
            query << "UPDATE users SET " << Log::delimiterString[Log::accountNameDeliName] << "='" << "accountName" << "' WHERE " << Log::delimiterString[Log::tokenDeliName] << "='" << token << "';";
            executeQuery(query.str());
        }
    }
    catch (const sql::SQLException &e)
    {
      std::cout << e.what() << std::endl;
    }
}

void                    Database::postLog(const std::string &token,
                                          Log &log)
{
    std::vector<std::map<std::string, std::string>> data;

    data = log.getData();
    for (auto &it : data)
    {
        executePostLog(token, it);
    }
}

std::vector<std::map<std::string, std::string>>         Database::getUser(const std::string &token,
                                                                          const std::string &accountName,
                                                                          const int &row)
{
    std::ostringstream                                  query;
    sql::ResultSet                                      *res;
    std::vector<std::map<std::string, std::string>>     data;

    query << "SELECT * FROM users WHERE 1";
    if (!token.empty())
        query << " AND " << Log::delimiterString[Log::tokenDeliName] << "='" << token << "'";
    if (!accountName.empty())
        query << " AND " << Log::delimiterString[Log::accountNameDeliName] << "='" << accountName << "'";
    if (row != 0)
        query << " LIMIT " << row;
    query << ";";
    try
    {
        res = executeQueryResponse(query.str());
        while (res->next())
        {
            data.push_back({{Log::delimiterString[Log::idDeliName], res->getString(Log::delimiterString[Log::idDeliName])},
                            {Log::delimiterString[Log::tokenDeliName], res->getString(Log::delimiterString[Log::tokenDeliName])},
                            {Log::delimiterString[Log::accountNameDeliName], res->getString(Log::delimiterString[Log::accountNameDeliName])}});
        }
        res->close();
        delete (res);
    }
    catch (const std::exception &e)
    {
      std::cout << e.what() << std::endl;
    }
    return (data);
};

std::vector<std::map<std::string, std::string>>         Database::getLog(const std::string &token,
                                                                         const std::string &inputText,
                                                                         const std::string &inputSpecial,
                                                                         const std::string &datetime,
                                                                         const std::string &windowName,
                                                                         const std::string &processName,
                                                                         const int &row)
{
    std::ostringstream                                  query;
    sql::ResultSet                                      *res;
    std::vector<std::map<std::string, std::string>>     data;

    query << "SELECT * FROM logs WHERE 1";
    if (!token.empty())
        query << " AND " << Log::delimiterString[Log::tokenDeliName] << "='" << token << "'";
    if (!inputText.empty())
        query << " AND " << Log::delimiterString[Log::inputTextDeliName] << " LIKE '%" << inputText << "%'";
    if (!inputSpecial.empty())
        query << " AND " << Log::delimiterString[Log::inputSpecialDeliName] << " LIKE '%" << inputSpecial << "%'";
    if (!datetime.empty())
        query << " AND " << Log::delimiterString[Log::dateTimeDeliName] << " LIKE '%" << datetime << "%'";
    if (!windowName.empty())
        query << " AND " << Log::delimiterString[Log::windowNameDeliName] << " LIKE '%" << windowName << "%'";
    if (!processName.empty())
        query << " AND " << Log::delimiterString[Log::processNameDeliName] << " LIKE '%" << processName << "%'";
    if (row != 0)
        query << " LIMIT " << row;
    query << ";";
    try
    {
        res = executeQueryResponse(query.str());
        while (res->next())
        {
            data.push_back({{Log::delimiterString[Log::idDeliName], res->getString(Log::delimiterString[Log::idDeliName])},
                            {Log::delimiterString[Log::tokenDeliName], res->getString(Log::delimiterString[Log::tokenDeliName])},
                            {Log::delimiterString[Log::inputTextDeliName], res->getString(Log::delimiterString[Log::inputTextDeliName])},
                            {Log::delimiterString[Log::inputSpecialDeliName], res->getString(Log::delimiterString[Log::inputSpecialDeliName])},
                            {Log::delimiterString[Log::dateTimeDeliName], res->getString(Log::delimiterString[Log::dateTimeDeliName])},
                            {Log::delimiterString[Log::windowNameDeliName], res->getString(Log::delimiterString[Log::windowNameDeliName])},
                            {Log::delimiterString[Log::processNameDeliName], res->getString(Log::delimiterString[Log::processNameDeliName])}});
        }
        res->close();
        delete (res);
    }
    catch (const sql::SQLException &e)
    {
      std::cout << e.what() << std::endl;
    }
    return (data);
}
