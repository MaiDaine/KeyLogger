//
// Created by wurtz on 9/28/17.
//

#ifndef CPP_SPIDER_TEST_SCRAPER_HPP
#define CPP_SPIDER_TEST_SCRAPER_HPP

#include <iostream>
#include <vector>

namespace SP
{
    class Scraper
    {
    private:

    public:
        Scraper();
        ~Scraper();

        std::vector<std::string>                             splitBySpecial(std::string log);

        std::vector<std::map<std::string, std::string>>      emailPassword(std::string log);
        std::vector<std::string>                             password(std::string log);
    };
};


#endif //CPP_SPIDER_TEST_SCRAPER_HPP
