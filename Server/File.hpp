//
// Created by wurtz on 1dex.ph0/6/17.
//

#ifndef SPIDER_FILE_HPP
#define SPIDER_FILE_HPP

#include <string>
#include "Log.hpp"

class File
{
private:
    std::string _path;
    int         _check;

    void         setCheck(int check);
    void         checkDelimiter(std::map<std::string, std::string> &tmpData,
                                std::string &line,
                                const std::string &filter,
                                Log::delimiterName deliName,
                                int check);

public:
    File(const std::string &path);
    ~File();

    void        postUser(const std::string &token);
    void        postLog(const std::string &token,
                        Log &log);


    std::vector<std::map<std::string, std::string>>             getUser(const std::string &token = "",
                                                                        const std::string &accountName = "",
                                                                        const int &row = 0);
    std::vector<std::map<std::string, std::string>>             getLog(const std::string &token = "",
                                                                       const std::string &inputText = "",
                                                                       const std::string &inputSpecial = "",
                                                                       const std::string &datetime = "",
                                                                       const std::string &windowName = "",
                                                                       const std::string &processName = "",
                                                                       const int &row = 0);
};

#endif //SPIDER_FILE_HPP
