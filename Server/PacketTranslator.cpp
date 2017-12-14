// Created by baptiste on 9/29/17.
//
//

#include <future>
#include "Log.hpp"
#include "LogSession.hpp"
#include "ImgLog.hpp"

void 	PacketTranslator::translatePacket(Packet & packet)
{
  switch (packet.getType())
  {
    case Packet::IDENTIFY :
      identificationPacket(packet);
      break ;
    case Packet::LOG :
      logPacket(packet);
      break ;
    case Packet::IMG :
      imgPacket(packet);
      break ;
    default :
      return ;
  }
}

void 	PacketTranslator::identificationPacket(Packet & packet)
{
  std::string 	macAddr;

  Serial::deserializeData(macAddr, packet.getData());
  if (_managedSession) {
    _managedSession->setId(macAddr);
    _DS.postUser(std::move(macAddr));
  }
}

void 	PacketTranslator::logPacket(Packet & packet)
{
  Log 	log;
  std::vector<std::map<std::string, std::string>> data;

  Serial::deserializeData(log, packet.getData());
  data = log.getData();

  _DS.postLog(_managedSession->getId(), log);
}

void	PacketTranslator::imgPacket(Packet & packet) const
{
  ImgLog	log;

  Serial::deserializeData(log, packet.getData());
  log.getData(_localStorage);
}