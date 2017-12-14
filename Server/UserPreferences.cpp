//
// Created by baptiste on 10/6/17.
//

#include "UserPreferences.hpp"

void 		UserPreferences::getUserPreferences()
{
  std::cout << "\t***\t\033[1;32mWelcome to Spider Server\033[0m\t***\n" << std::endl;
  std::cout << "Configuring your storage preferences :" << std::endl;

  storagePreferences();
}

bool 		UserPreferences::askUser(const std::string &question) const
{
  std::string 	_input;

  std::cout << question;
  std::getline(std::cin, _input);
  return std::tolower(_input[0]) == 'y';
}

bool		UserPreferences::retrieveFolder()
{
  std::string 	_input;
  boost::filesystem::file_status _s;

  while (1) {
    std::cout << "Enter folder path : ";
    std::getline(std::cin, _input);
    _s = boost::filesystem::status(boost::filesystem::path(_input.c_str()));
    if (boost::filesystem::exists(_input)
	&& boost::filesystem::is_directory(_input)
	&& _s.permissions() >= 0600) {
      isSlashTerminated(_input);
      _localPath = _input;
      return true;
    }
    std::cout << "Cannot open path, please check if it exists,"
	      << "if it's a directory and that you have the right permissions on it"
	      << std::endl;
    if (!askUser("Retry? [y/n] : "))
      return false;
  }
}

void 		UserPreferences::isSlashTerminated(std::string &dir) const
{
  if (!dir.empty() && dir[dir.size() - 1] != '/')
    dir.append("/");
}

bool		UserPreferences::createFolder()
{
  std::string 	_input;

  while (1) {
    std::cout << "Choose new folder path : ";
    std::getline(std::cin, _input);
    isSlashTerminated(_input);
    try {
      boost::filesystem::path dir(_input);
      if (boost::filesystem::create_directory(dir))
	_localPath = _input;
    } catch (const std::exception &e) {
      if (!askUser("Cannot create this directory, retry? [y/n] : "))
	return false;
      continue ;
    }
    return true;
  }
}

bool		UserPreferences::selectLocalPath()
{
  std::cout << "\nLocal storage configuration\n" << std::endl;

  while (1) {
    if (askUser("Use an existing folder? [y/n]: ") && retrieveFolder())
      return true;
    if (askUser("Create a folder? [y/n]: ") && createFolder())
      return true;
    if (!askUser("Local storage settings cannot be setup, retry? [y/n] : "))
      return false;
  }
}

bool 		UserPreferences::setupDefaultDatabaseSettings() const { return true; }

bool 		UserPreferences::setupUserDatabaseSettings()
{
  std::string 	_input;

  std::cout << "Choose HOST: ";
  std::getline(std::cin, _input);
  _DBinfo._host = _input;
  while (1) {
    std::cout << "Choose PORT : ";
    std::getline(std::cin, _input);
    try {_DBinfo._host = std::stoi(_input); break ;}
    catch (const std::exception &e)
    {
      if (askUser("Invalid port number retry? [y/n] : "))
	continue ;
      return false;
    }
  }
  std::cout << "Choose SCHEMA : ";
  std::getline(std::cin, _input);
  _DBinfo._database = _input;
  std::cout << "Choose USER : ";
  std::getline(std::cin, _input);
  _DBinfo._user = _input;
  std::cout << "Choose PASSWORD : ";
  std::getline(std::cin, _input);
  _DBinfo._password = _input;
  return true;
}

bool		UserPreferences::selectDatabase()
{
  std::cout << "\nSQL database configuration\n" << std::endl;

  while (1) {
    if (askUser("Use the default associated database? [y/n] : "))
      return setupDefaultDatabaseSettings();
    if (askUser("Use personnal database? [y/n] : "))
      return setupUserDatabaseSettings();
    if (!askUser("SQL Database storage settings cannot be setup, retry? [y/n] : "))
      return false;
  }
}

bool 		UserPreferences::storagePreferences()
{
  std::string	choice;

  while (1) {
    std::cout << "Would you like to use [1] local storage, [2] SQL database, [3] both ? [1,2,3] : ";
    std::getline(std::cin, choice);
    if (choice[0] == '1' && selectLocalPath()) {
      _StorageT = STORAGE_TYPE::FILE_STORAGE;
      return true;
    }
    else if (choice[0] == '2' && selectDatabase()) {
      _StorageT = STORAGE_TYPE::DATABASE_STORAGE;
      return true;
    }
    else if (choice[0] == '3' && selectLocalPath()
	     && selectDatabase()) {
      _StorageT = STORAGE_TYPE::ALL_STORAGE;
      return true;
    }
    if (!askUser("Something went wrong. Retry configuring storage preferences? [y/n] : "))
      return false;
  }
}
