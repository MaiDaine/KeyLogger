//
// Created by wurtz on 10/7/17.
//

#ifndef SPIDER_DATASTORAGE_HPP
#define SPIDER_DATASTORAGE_HPP

#include <string>
#include <map>
#include <vector>
#include "Log.hpp"
#include "Database.hpp"
#include "DatabaseHandler.hpp"

enum STORAGE_TYPE
{
    DATABASE_STORAGE,
    FILE_STORAGE,
    ALL_STORAGE
};

class DataStorage
{
private:
    boost::shared_ptr<DatabaseHandler> _databaseHandler;

    int                 _idDatabase;
    std::string         _pathFile;
    enum STORAGE_TYPE   _storageType;

public:
    DataStorage(boost::shared_ptr<DatabaseHandler> &databaseHandler,
                const std::string &localPath,
                const STORAGE_TYPE storageType);
    ~DataStorage();

    int                	setDatabase(const DatabaseInfo &databaseInfo);
    int                	setDatabase(const int idDatabase);
    bool               	setFile(const std::string &pathFile);
    void		setStorageType(const enum STORAGE_TYPE storageType);

    void                postUser(const std::string &token);
    void                postLog(const std::string &token,
                                Log &log);

    template <enum STORAGE_TYPE>
    std::vector<std::map<std::string, std::string>>     getUser(const std::string &token = "",
                                                                const std::string &accountName = "",
                                                                const int &row = 0);
    template <enum STORAGE_TYPE>
    std::vector<std::map<std::string, std::string>>     getLog(const std::string &token = "",
                                                               const std::string &inputText = "",
                                                               const std::string &inputSpecial = "",
                                                               const std::string &datetime = "",
                                                               const std::string &windowName = "",
                                                               const std::string &processName = "",
                                                               const int &row = 0);
};

#endif //SPIDER_DATASTORAGE_HPP
