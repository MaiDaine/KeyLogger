//
// Created by wurtz on 10/7/17.
//

#include "DatabaseHandler.hpp"

DatabaseHandler::DatabaseHandler()
: _counterId(0)
{
}

DatabaseHandler::~DatabaseHandler()
{
}

int                 DatabaseHandler::addDatabase(const DatabaseInfo &databaseInfo)
{
  std::shared_ptr<Database> database;

  for (auto &it : _database)
    {
        if (it.second.second._host == databaseInfo._host
            && it.second.second._port == databaseInfo._port
            && it.second.second._database == databaseInfo._database)
        {
            return (it.first);
        }
    }
    try
    {
        database = std::make_shared<Database>(databaseInfo);
        _database[_counterId].first = database;
        _database[_counterId].second = databaseInfo;
      	++_counterId;
    }
    catch (const sql::SQLException &e)
    {
      std::cout << e.what() << std::endl;
      return (-1);
    }
    return (_counterId);
}

std::shared_ptr<Database>  DatabaseHandler::getDatabase(const int id)
{
    return (_database[id].first);
}

const std::map<int, std::pair<std::shared_ptr<Database>, DatabaseInfo>> &DatabaseHandler::getDatabases() const
{
    return (std::ref(_database));
};
