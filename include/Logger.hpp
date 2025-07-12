#ifndef _LOGGER
#define _LOGGER

#include "Defines.hpp"
#include "StringHandler.hpp"
#include "Directory.hpp"
#include "Timestamp.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#define __FUNCTIONNAME__ __FUNCTION__
#else
#define __FUNCTIONNAME__ __PRETTY_FUNCTION__
#endif

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
    void    write(std::string logEntry, LogLevel llevel, const char* func, const char* file, int line);
    void    writeExtended(LogLevel llevel, const char* func, const char* file, int line, const char* format,...);
    void    setRemotePort(int remotePort);
    void    setRemoteHost(std::string remoteHost);
    void    setLogFileSize(int flsz);
    void    setLogDirectory(std::string dirpath);
    void    setModuleName(const char* mname);
    static Logger*  GetInstance();
private:
    void createBackupFileName(std::string&str);
    int     remote_log_port;
    std::string log_filename;
    std::string  remote_log_host;
    std::string  log_directory;
    std::string  log_backup_directory;
    int     log_file_size;
    std::string  log_module_name;
    FILE*   file_descriptor;
    LogFileMode log_file_mode;
    std::map<LogLevel, std::string> log_level_map;
};

#define writeLog(str,level) Logger::GetInstance()->write(str,level,__FUNCTIONNAME__,__FILE__,__LINE__);
#define writeLogNormal(str) Logger::GetInstance()->write(str,LOG_INFO,__FUNCTIONNAME__,__FILE__,__LINE__);

}

#endif

