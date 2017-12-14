#pragma once

#include "Boost.hpp"

class Serial {
  public:
    template<typename T>
    static std::string serializeData(const T &data) {
      std::ostringstream archiveStream;
      std::string serialData;
      try {
	boost::archive::text_oarchive archive(archiveStream);
	archive << data;
	serialData = archiveStream.str();
      }
      catch (std::exception &) {
	return "ERROR";
      }
      return serialData;
    }

    template<typename T>
    static bool deserializeData(T &dataStructure, const std::string &data) {
      try {
	std::istringstream archiveStream(data);
	boost::archive::text_iarchive archive(archiveStream);
	archive >> dataStructure;
      }
      catch (std::exception &) {
	return false;
      }
      return true;
    }
};