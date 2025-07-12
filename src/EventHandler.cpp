#include "EventHandler.hpp"

char logbuffer[128]={0};

EventHandler::EventHandler()
{
    ProcessLock pl;
    pl.getUserName(session_user);
    logverbose = false;
}

void EventHandler::setLogVerbosity(bool lvb)
{
    logverbose = lvb;
}

bool EventHandler::eventHead(const char* url, char **appdata, long &datasize, char **ctype)
{
    return false;
}

bool EventHandler::eventGet(const char* url, char **appdata, long &datasize, char **ctype)
{
    *appdata = NULL;
    *ctype = NULL;
    datasize = 0;

    std::string reportmonth;
    std::string name, extn;

    std::string resolvedURL = url;

    if(strcmp(resolvedURL.c_str(),"/")==0)
    {
        return false;
    }

    std::vector<std::string> tokenlist;
    // Adjust for leading '/' while splitting the token
    StringHandler::split((const char*)url+1, tokenlist, '/');

    if(tokenlist.size() < 1)
    {
        return false;
    }

    std::string resolvedFileName = server_root;

    if(url[0]=='/' && server_root[server_root.length()-1]=='/')
    {
        resolvedFileName += url+1;
    }
    else
    {
        resolvedFileName += url;
    }

    int pos = -1;
    pos = StringHandler::characterposition(resolvedFileName.c_str(),'?');

    if(pos >= 0)
    {
        resolvedFileName[pos]='\0';
        std::string temp, args, val;
        StringHandler::split(resolvedURL, '?', temp, args);
        resolvedURL = temp;
        StringHandler::replace(resolvedURL, "/", "");

        StringHandler::split(args, '=', temp, val);

        if(temp == "month")
        {
            reportmonth = val;
        }
    }

    resolvedFileName = resolvedURL;
    StringHandler::split(resolvedURL, '.', name, extn);

    return false;
}

bool EventHandler::eventPut(const char* url, char **appdata, long &datasize, char **ctype)
{
    return false;
}

bool EventHandler::eventPost(const char* url, char **appdata, long &datasize, char **ctype)
{
	return eventPost(url, appdata, datasize, ctype);
    //return false;
}

bool EventHandler::eventDelete(const char* url, char **appdata, long &datasize, char **ctype)
{
    return false;
}

bool EventHandler::eventTrace(const char* url, char **appdata, long &datasize, char **ctype)
{
    return false;
}

bool EventHandler::eventOptions(const char* url, char **appdata, long &datasize, char **ctype)
{
    return false;
}

bool EventHandler::eventConnect(const char* url, char **appdata, long &datasize, char **ctype)
{
    return false;
}

bool EventHandler::userDataFunction(const void* ptr, long len)
{
   return false;
}

void EventHandler::suspend()
{
    writeLog("Callback -> suspend", LOG_INFO);
}

void EventHandler::resume()
{
    writeLog("Callback -> resume", LOG_INFO);
}

void EventHandler::shutdown()
{
    writeLog("Callback -> shutdown", LOG_INFO);
}

void EventHandler::alarm()
{
    writeLog("Callback -> alarm", LOG_INFO);
}

void EventHandler::reset()
{
    writeLog("Callback -> reset", LOG_INFO);
}

void EventHandler::childExit()
{
    writeLog("Callback -> childExit", LOG_INFO);
}

void EventHandler::userdefined1()
{
    writeLog("Callback -> userdefined1", LOG_INFO);
}

void EventHandler::userdefined2()
{
    writeLog("Callback -> userdefined2", LOG_INFO);
}

