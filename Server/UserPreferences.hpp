//
// Created by baptiste on 10/6/17.
//

#ifndef SPIDER_USERPREFERENCES_HPP
# define SPIDER_USERPREFERENCES_HPP

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
//#include "Database.hpp"
#include "DataStorage.hpp"

class UserPreferences
{
  public:
    UserPreferences() = default;
    ~UserPreferences() = default;

    const std::string 	getLocalStoragePath() const { return _localPath; }
    const DatabaseInfo 	getDatabase() const { return _DBinfo; }
    STORAGE_TYPE 	getStorage() const { return _StorageT; }

    void 		getUserPreferences();

  private:
    bool 		askUser(const std::string &question) const;
    bool		retrieveFolder();
    void 		isSlashTerminated(std::string &dir) const;
    bool		createFolder();
    bool		selectLocalPath();
    bool 		setupDefaultDatabaseSettings() const;
    bool 		setupUserDatabaseSettings();
    bool		selectDatabase();
    bool 		storagePreferences();

  private:
    std::string 		_localPath;
    DatabaseInfo		_DBinfo;
    STORAGE_TYPE 		_StorageT;
};


#endif /* !SPIDER_USERPREFERENCES_HPP */
