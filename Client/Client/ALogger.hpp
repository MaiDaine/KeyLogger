#ifndef ALOGGER_HPP_
#define ALOGGER_HPP_

#include <string>

class LoggerHandler;

class ALogger
{
public:
	ALogger();
	~ALogger();
	virtual	bool				start()= 0;
	virtual void				stop() const = 0;
	virtual std::string			getLog() = 0;

private:
};

#endif /* !ALOGGER_HPP_ */
