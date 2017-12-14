//
// Created by baptiste on 9/29/17.
//

#ifndef SPIDER_PACKETTRANSLATOR_HPP
# define SPIDER_PACKETTRANSLATOR_HPP

#include <bits/unique_ptr.h>
#include "Packet.hpp"
#include "DatabaseHandler.hpp"
#include "DataStorage.hpp"

class LogSession;

class PacketTranslator
{
  public:
    PacketTranslator(LogSession *managedSession,
		     boost::shared_ptr<DatabaseHandler> &DB,
		     const std::string &localFolder,
		     const STORAGE_TYPE storageType)
      : _managedSession(managedSession), _DS(DB, localFolder, storageType) {}
    ~PacketTranslator() = default;

    void 	translatePacket(Packet & packet);
    DataStorage	&getDataStorage() { return _DS; };

  private:
    void identificationPacket(Packet & packet);
    void logPacket(Packet & packet);
    void imgPacket(Packet & packet) const;

  private:
    LogSession				*_managedSession;
    DataStorage				_DS;
    std::string 			_localStorage;
};


#endif /* !SPIDER_PACKETTRANSLATOR_HPP */
