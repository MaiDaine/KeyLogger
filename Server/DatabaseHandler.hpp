//
// Created by wurtz on 10/7/17.
//

#ifndef SPIDER_DATAHANDLER_HPP
#define SPIDER_DATAHANDLER_HPP


#include "Database.hpp"

class DatabaseHandler
{
private:
    int                                                 		_counterId;
    std::map<int, std::pair<std::shared_ptr<Database>, DatabaseInfo>>  	_database;

public:
    DatabaseHandler();
    ~DatabaseHandler();

    int                     	addDatabase(const DatabaseInfo &databaseInfo);
    std::shared_ptr<Database>  	getDatabase(const int id);
    const std::map<int, std::pair<std::shared_ptr<Database>, DatabaseInfo>> &getDatabases() const;
};


#endif //SPIDER_DATAHANDLER_HPP
