cmake_minimum_required(VERSION 3.10)

project(EdgeViewAPI)

set(HttpServer_VERSION_MAJOR 1)
set(HttpServer_VERSION_MINOR 0)

set(HTTPSERVER_INCLUDE ./include)
set(HTTPSERVER_SOURCES ./src)

include_directories(./sources/ /usr/include/ ./include)

link_directories(/usr/local/lib/)

set(SOURCES
   ${SOURCES}
   ${HTTPSERVER_SOURCES}/EventHandler.cpp
   ${HTTPSERVER_SOURCES}/HttpHandler.cpp
   ${HTTPSERVER_SOURCES}/Variant.cpp
   ${HTTPSERVER_SOURCES}/Timestamp.cpp
   ${HTTPSERVER_SOURCES}/StringHandler.cpp
   ${HTTPSERVER_SOURCES}/Logger.cpp
   ${HTTPSERVER_SOURCES}/Directory.cpp
   ${HTTPSERVER_SOURCES}/Configuration.cpp
   ${HTTPSERVER_SOURCES}/SignalHandler.cpp
   ${HTTPSERVER_SOURCES}/Mutex.cpp
   ${HTTPSERVER_SOURCES}/ProcessLock.cpp
   ${HTTPSERVER_SOURCES}/AbnfMessage.cpp
   ${HTTPSERVER_SOURCES}/AbnfAdapter.cpp
   ${HTTPSERVER_SOURCES}/Responder.cpp
   ${HTTPSERVER_SOURCES}/HttpServer.cpp
   ${HTTPSERVER_SOURCES}/ResourceHandler.cpp
   ${HTTPSERVER_SOURCES}/NetworkHelper.cpp
   ${HTTPSERVER_SOURCES}/EdgeViewAPI.cpp
)

set(HEADERS
   ${HEADERS}
   ${HTTPSERVER_INCLUDE}/EventHandler.hpp
   ${HTTPSERVER_INCLUDE}/Version.hpp
   ${HTTPSERVER_INCLUDE}/HttpHandler.hpp
   ${HTTPSERVER_INCLUDE}/Variant.hpp
   ${HTTPSERVER_INCLUDE}/Timestamp.hpp
   ${HTTPSERVER_INCLUDE}/StringHandler.hpp
   ${HTTPSERVER_INCLUDE}/Defines.hpp
   ${HTTPSERVER_INCLUDE}/Logger.hpp
   ${HTTPSERVER_INCLUDE}/Directory.hpp
   ${HTTPSERVER_INCLUDE}/Configuration.hpp
   ${HTTPSERVER_INCLUDE}/SignalHandler.hpp
   ${HTTPSERVER_INCLUDE}/Mutex.hpp
   ${HTTPSERVER_INCLUDE}/ProcessLock.hpp
   ${HTTPSERVER_INCLUDE}/AbnfMessage.hpp
   ${HTTPSERVER_INCLUDE}/AbnfAdapter.hpp
   ${HTTPSERVER_INCLUDE}/Responder.hpp
   ${HTTPSERVER_INCLUDE}/HttpServer.hpp
   ${HTTPSERVER_INCLUDE}/ResourceHandler.hpp
   ${HTTPSERVER_INCLUDE}/NetworkHelper.hpp
)

add_executable(EdgeViewAPI ${SOURCES} ${HEADERS})
target_link_libraries(EdgeViewAPI pthread)
