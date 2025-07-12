#ifndef _LOGGER
#define _LOGGER

#include "Defines.hpp"
#include "StringHandler.hpp"
#include "Directory.hpp"
#include "Timestamp.hpp"

#define __FUNCTIONNAME__ __PRETTY_FUNCTION__

extern "C"
{

typedef enum LogLevel
{
    LOG_INFO=0,
    LOG_ERROR=1,
    LOG_WARNING=2,
    LOG_CRITICAL=3,
    LOG_PANIC=4
}LogLevel;

typedef enum LogFileMode
{
    FILE_APPEND=0,
    FILE_CREATE_NEW=1
}LogFileMode;

class Logger
{
public:
	Logger();
	~Logger();

    void    startLogging(LogFileMode fmode);
    void    stopLogging();
    void    write(String logEntry, LogLevel llevel, const char* func, const char* file, int line);
    void    writeExtended(LogLevel llevel, const char* func, const char* file, int line, const char* format,...);
    void    setRemotePort(int remotePort);
    void    setRemoteHost(String remoteHost);
    void    setLogFileSize(int flsz);
    void    setLogDirectory(String dirpath);
    void    setModuleName(const char* mname);
    static Logger*  GetInstance();
private:
    void createBackupFileName(String &str);
    int     remoteLogPort;
    String logfilename;
    String  remoteLogHost;
    String  logDirectory;
    String  logBackupDirectory;
    int     logFileSize;
    String  moduleName;
    FILE*   logFile;
    LogFileMode filemode;
    std::map<LogLevel, String> logLevelMap;
};

#define writeLog(str,level) Logger::GetInstance()->write(str,level,__FUNCTIONNAME__,__FILE__,__LINE__);
#define writeLogNormal(str) Logger::GetInstance()->write(str,LOG_INFO,__FUNCTIONNAME__,__FILE__,__LINE__);

}

#endif

