cmake_minimum_required(VERSION 3.7)
project(Spider)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g3 -lssl -lcrypto -lboost_serialization -lpthread -lmysqlcppconn -Wall -Werror -W")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

find_package(Boost 1.64.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable( SpiderServer
        Boost.hpp
        Serial.hpp
        Packet.hpp
        ImgLog.cpp ImgLog.hpp
        DatabaseHandler.cpp DatabaseHandler.hpp
        DataStorage.cpp DataStorage.hpp
        File.cpp File.hpp
        Server.cpp Server.hpp
        SSLConnectionServer.hpp
        LogSession.cpp LogSession.hpp
        Database.cpp Database.hpp
        Log.cpp Log.hpp
        CommandAcceptor.cpp CommandAcceptor.hpp
        PacketTranslator.cpp PacketTranslator.hpp
        UserPreferences.cpp UserPreferences.hpp
        )

target_link_libraries(SpiderServer ${Boost_LIBRARIES})