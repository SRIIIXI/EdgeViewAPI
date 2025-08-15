cmake_minimum_required(VERSION 3.10)

project(EdgeViewAPI)

set(EdgeViewAPI_VERSION_MAJOR 1)
set(EdgeViewAPI_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 17)

set(EDGEVIEWAPI_INCLUDE ./include)
set(EDGEVIEWAPI_SOURCES ./src)

if(WIN32)
	include_directories(C:/vcpkg/installed/x64-windows/include ./include)
	link_directories(C:/vcpkg/installed/x64-windows/lib/)
else()
	include_directories(/include/ /usr/local/include /usr/include/ ./include)
	link_directories(/usr/local/lib/ /usr/lib/ /lib/)
 endif()


set(COMMON_SOURCES
   ${COMMON_SOURCES}
   ${EDGEVIEWAPI_SOURCES}/EventHandler.cpp
   ${EDGEVIEWAPI_SOURCES}/HttpHandler.cpp
   ${EDGEVIEWAPI_SOURCES}/Variant.cpp
   ${EDGEVIEWAPI_SOURCES}/Timestamp.cpp
   ${EDGEVIEWAPI_SOURCES}/StringHandler.cpp
   ${EDGEVIEWAPI_SOURCES}/Logger.cpp
   ${EDGEVIEWAPI_SOURCES}/Directory.cpp
   ${EDGEVIEWAPI_SOURCES}/Configuration.cpp
   ${EDGEVIEWAPI_SOURCES}/SignalHandler.cpp
   ${EDGEVIEWAPI_SOURCES}/Mutex.cpp
   ${EDGEVIEWAPI_SOURCES}/ProcessLock.cpp
   ${EDGEVIEWAPI_SOURCES}/AbnfMessage.cpp
   ${EDGEVIEWAPI_SOURCES}/AbnfAdapter.cpp
   ${EDGEVIEWAPI_SOURCES}/Responder.cpp
   ${EDGEVIEWAPI_SOURCES}/ResourceHandler.cpp
   ${EDGEVIEWAPI_SOURCES}/NetworkHelper.cpp
   ${EDGEVIEWAPI_SOURCES}/EdgeViewAPI.cpp
   ${EDGEVIEWAPI_SOURCES}/Model.c
)

set(HEADERS
   ${HEADERS}
   ${EDGEVIEWAPI_INCLUDE}/Model.hpp
   ${EDGEVIEWAPI_INCLUDE}/EventHandler.hpp
   ${EDGEVIEWAPI_INCLUDE}/Version.hpp
   ${EDGEVIEWAPI_INCLUDE}/HttpHandler.hpp
   ${EDGEVIEWAPI_INCLUDE}/Variant.hpp
   ${EDGEVIEWAPI_INCLUDE}/Timestamp.hpp
   ${EDGEVIEWAPI_INCLUDE}/StringHandler.hpp
   ${EDGEVIEWAPI_INCLUDE}/Defines.hpp
   ${EDGEVIEWAPI_INCLUDE}/Logger.hpp
   ${EDGEVIEWAPI_INCLUDE}/Directory.hpp
   ${EDGEVIEWAPI_INCLUDE}/Configuration.hpp
   ${EDGEVIEWAPI_INCLUDE}/SignalHandler.hpp
   ${EDGEVIEWAPI_INCLUDE}/Mutex.hpp
   ${EDGEVIEWAPI_INCLUDE}/ProcessLock.hpp
   ${EDGEVIEWAPI_INCLUDE}/AbnfMessage.hpp
   ${EDGEVIEWAPI_INCLUDE}/AbnfAdapter.hpp
   ${EDGEVIEWAPI_INCLUDE}/Responder.hpp
   ${EDGEVIEWAPI_INCLUDE}/HttpServer.hpp
   ${EDGEVIEWAPI_INCLUDE}/ResourceHandler.hpp
   ${EDGEVIEWAPI_INCLUDE}/NetworkHelper.hpp
)

if(WIN32)

set(WIN32_SOURCES
   ${WIN32_SOURCES}
   ${EDGEVIEWAPI_SOURCES}/HttpServerWinsock.cpp
)

else()

set(POSIX_SOURCES
   ${POSIX_SOURCES}
   ${EDGEVIEWAPI_SOURCES}/HttpServer.cpp
)
endif()

if(WIN32)
	add_executable(EdgeViewAPI ${COMMON_SOURCES} ${WIN32_SOURCES} ${HEADERS})
else()
	add_executable(EdgeViewAPI ${COMMON_SOURCES} ${POSIX_SOURCES} ${HEADERS})
endif()

if(WIN32)
	target_link_libraries(EdgeViewAPI ws2_32 pq)
else()
	target_link_libraries(EdgeViewAPI pthread pq)
endif()