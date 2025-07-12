#pragma once

#ifndef	_EVENTHANDLER
#define	_EVENTHANDLER
#include "Defines.hpp"
#include "ResourceHandler.hpp"
#include "AbnfMessage.hpp"
#include "HttpHandler.hpp"
#include "ProcessLock.hpp"
#include "SignalHandler.hpp"
#include "Logger.hpp"

class EventHandler : public HttpHandler, public SignalCallback
{
public:
    EventHandler();
    void setLogVerbosity(bool lvb);
    bool eventHead(const char* url, char **appdata, long &datasize, char **ctype);
    bool eventGet(const char* url, char **appdata, long &datasize, char **ctype);
    bool eventPut(const char* url, char **appdata, long &datasize, char **ctype);
    bool eventPost(const char* url, char **appdata, long &datasize, char **ctype);
    bool eventDelete(const char* url, char **appdata, long &datasize, char **ctype);
    bool eventTrace(const char* url, char **appdata, long &datasize, char **ctype);
    bool eventOptions(const char* url, char **appdata, long &datasize, char **ctype);
    bool eventConnect(const char* url, char **appdata, long &datasize, char **ctype);
    bool userDataFunction(const void* ptr, long len);

    void suspend();
    void resume();
    void shutdown();
    void alarm();
    void reset();
    void childExit();
    void userdefined1();
    void userdefined2();
private:
    std::string session_user;
    bool logverbose;
};

#endif
