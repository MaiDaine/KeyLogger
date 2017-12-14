//
// Created by wurtz on 10/7/17.
//

#include <boost/filesystem/operations.hpp>
#include "DataStorage.hpp"
#include "File.hpp"

DataStorage::DataStorage(boost::shared_ptr<DatabaseHandler> &databaseHandler,
                         const std::string &localPath,
                         const STORAGE_TYPE storageType)
  : _databaseHandler(databaseHandler),
    _idDatabase(0),
    _pathFile(localPath),
    _storageType(storageType)
{
}

DataStorage::~DataStorage()
{
}

int                DataStorage::setDatabase(const DatabaseInfo &databaseInfo)
{
    if ((_idDatabase = _databaseHandler->addDatabase(databaseInfo)) == -1)
    {
        _idDatabase = 0;
        return (-1);
    }
    return (_idDatabase);
}

int                DataStorage::setDatabase(const int idDatabase)
{
    if (_databaseHandler->getDatabase(idDatabase) == nullptr)
    {
        _idDatabase = 0;
        return (-1);
    }
    _idDatabase = idDatabase;
    return (idDatabase);
}


bool               DataStorage::setFile(const std::string &pathFile)
{
  boost::filesystem::file_status _s;

  _s = boost::filesystem::status(boost::filesystem::path(pathFile.c_str()));
  if (boost::filesystem::exists(pathFile)
	&& boost::filesystem::is_directory(pathFile)
	&& _s.permissions() >= 0600) {
     _pathFile = pathFile;
    if (!_pathFile.empty() && _pathFile[_pathFile.size() - 1] != '/')
      _pathFile.append("/");
     return true;
   }
   std::cout << "\nCannot open path, please check if it exists,"
      << "if it's a directory and that you have the right permissions on it"
      << std::endl;
  return false;
}

void		   DataStorage::setStorageType(const enum STORAGE_TYPE storageType)
{
  if (storageType == STORAGE_TYPE::DATABASE_STORAGE
      || storageType == STORAGE_TYPE::ALL_STORAGE)
    if (_databaseHandler->getDatabases().empty()
      	|| _databaseHandler->getDatabases().size() <= static_cast<uint32_t>(_idDatabase))
    {
      std::cerr << "Cannot switch storage mode to DATABASE : No reachable Database" << std::endl;
      return ;
    }
  _storageType = storageType;
}

void                DataStorage::postUser(const std::string &token)
{
  File            file(_pathFile);

  switch (_storageType)
  {
    case DATABASE_STORAGE:
    {
      _databaseHandler->getDatabase(_idDatabase)->postUser(token);
      break ;
    }
    case FILE_STORAGE:
    {
      file.postUser(token);
      break ;
    }
    case ALL_STORAGE:
    {
      _databaseHandler->getDatabase(_idDatabase)->postUser(token);
      file.postUser(token);
      break ;
    }
  }
}

void                DataStorage::postLog(const std::string &token,
                                         Log &log)
{
  File            file(_pathFile);

  switch (_storageType)
  {
    case DATABASE_STORAGE:
    {
      _databaseHandler->getDatabase(_idDatabase)->postLog(token, log);
      break ;
    }
    case FILE_STORAGE:
    {
      file.postLog(token, log);
      break ;
    }
    case ALL_STORAGE:
    {
      _databaseHandler->getDatabase(_idDatabase)->postLog(token, log);
      file.postLog(token, log);
      break ;
    }
  }

}

template <>
std::vector<std::map<std::string, std::string>>     DataStorage::getUser<DATABASE_STORAGE>(const std::string &token,
                                                                                           const std::string &accountName,
                                                                                           const int &row)
{
    return (_databaseHandler->getDatabase(_idDatabase)->getUser(token, accountName, row));
}

template <>
std::vector<std::map<std::string, std::string>>     DataStorage::getUser<FILE_STORAGE>(const std::string &token,
                                                                                       const std::string &accountName,
                                                                                       const int &row)
{
  File file(_pathFile);

  return (file.getUser(token, accountName, row));
}

template <>
std::vector<std::map<std::string, std::string>>     DataStorage::getLog<DATABASE_STORAGE>(const std::string &token,
                                                                                          const std::string &inputText,
                                                                                          const std::string &inputSpecial,const std::string &dateTime,
                                                                                          const std::string &windowName,
                                                                                          const std::string &processName,
                                                                                          const int &row)
{
    return (_databaseHandler->getDatabase(_idDatabase)->getLog(token, inputText, inputSpecial, dateTime, windowName, processName, row));
};

template <>
std::vector<std::map<std::string, std::string>>     DataStorage::getLog<FILE_STORAGE>(const std::string &token,
                                                                                      const std::string &inputText,
                                                                                      const std::string &inputSpecial,
                                                                                      const std::string &dateTime,
                                                                                      const std::string &windowName,
                                                                                      const std::string &processName,
                                                                                      const int &row)
{
    File            file(_pathFile);

    return (file.getLog(token, inputText, inputSpecial, dateTime, windowName, processName, row));
};