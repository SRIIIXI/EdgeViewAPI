#include "Logger.hpp"

Logger objLogger;

Logger*  Logger::GetInstance()
{
    return &objLogger;
}

Logger::Logger()
{
    remoteLogPort = 9090;
    remoteLogHost = "127.0.0.1";
    logDirectory = "";
    logFileSize = 1024;
    logFile = NULL;

    char pidstr[16];
    memset((char*)&pidstr[0],0,16);
    sprintf(pidstr,"%d",getpid());
    moduleName = pidstr;

    logLevelMap.clear();

    logLevelMap[LOG_INFO]       ="Information";
    logLevelMap[LOG_WARNING]    ="Warning    ";
    logLevelMap[LOG_ERROR]      ="Error      ";
    logLevelMap[LOG_CRITICAL]   ="Critical   ";
    logLevelMap[LOG_PANIC]      ="Panic      ";
}

Logger::~Logger()
{
    stopLogging();
}

void Logger::stopLogging()
{
    if(logFile!=NULL)
    {
        fflush(logFile);
        fclose(logFile);
    }
    logLevelMap.clear();
}

void Logger::createBackupFileName(String &str)
{
    Timestamp ts;
    String tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
    char temp[1024];
    memset((char*)&temp[0],0,16);
    sprintf(temp,"%s_%s.log",moduleName.c_str(),tstamp.c_str());
    str = temp;
}


void Logger::startLogging(LogFileMode fmode)
{
    filemode = fmode;
    if(logDirectory.empty() || logDirectory.length()<1)
    {
        char filepathbuffer[1024];
        memset((char*)&filepathbuffer[0],0,1024);
        getcwd(&filepathbuffer[0],1024);
        DirectoryHandler::getParentDirectory(&filepathbuffer[0]);
        strcat(filepathbuffer,"config");
        filepathbuffer[strlen(filepathbuffer)] = DIRECTORY_SEPARATOR;

        if(!DirectoryHandler::isDirectory(filepathbuffer))
        {
            DirectoryHandler::createDirectory(filepathbuffer);
        }

        logDirectory = filepathbuffer;
    }

    logfilename = logDirectory;
    logfilename += "/";
    logfilename += moduleName;
    logfilename += ".log";

    if(filemode == FILE_APPEND)
    {
        logFile = fopen(logfilename.c_str(),"a+");
    }
    else
    {
       logFile = fopen(logfilename.c_str(),"w+");
    }
}

void Logger::write(String logEntry, LogLevel llevel, const char* func, const char* file, int line)
{
    if(logFile!=NULL)
    {
        int sz = ftell(logFile);

        if(sz >= logFileSize*1024)
        {
            String temp;
            createBackupFileName(temp);
            String backupfile = logBackupDirectory + temp;
            stopLogging();
            rename(logfilename.c_str(),backupfile.c_str());
            startLogging(filemode);
        }

        String sourcefile;
        DirectoryHandler::getName(file, sourcefile);
        String lvel = logLevelMap[llevel];

        Timestamp ts;
        String tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
        char temp[1024];
        memset((char*)&temp[0],0,16);

        char fname[256]={0};
        memcpy(fname,func,255);
        #if defined(_WIN32) || defined(WIN32)
        #else
        int pos = StringHandler::characterposition(fname,'(');
        fname[pos]=0;
        #endif

        String left, right;
        StringHandler::split(fname, "::", left, right);
        if(right.length()>1)
        {
            strcpy(fname,right.c_str());
        }
        StringHandler::split(fname, " ", left, right);
        if(right.length()>1)
        {
            strcpy(fname,right.c_str());
        }

        sprintf(temp,"%s|%s|%05d|%s|%s| ",tstamp.c_str(),lvel.c_str(),line,fname,sourcefile.c_str());

        logEntry = temp + logEntry;
        fprintf(logFile,"%s\n",logEntry.c_str());
        fflush(logFile);
    }
}

void Logger::setModuleName(const char *mname)
{
    int len = strlen(mname);

    int ctr = 0;

    int pos1 = 0;
    int pos2 = 0;

    pos1 = StringHandler::characterposition(mname, '/');
    pos2 = StringHandler::characterposition(mname, '\\');

    if(pos1 > -1 || pos2 > -1)
    {
        for(ctr = len; ; ctr--)
        {
            if(mname[ctr] == '/' || mname[ctr] == '\\')
            {
                break;
            }
        }
        char buffer[32]={0};

        strncpy((char*)&buffer[0], (char*)&mname[ctr+1], 32);

        moduleName = buffer;
    }
    else
    {
        moduleName = mname;
    }

    StringHandler::replace(moduleName, ".exe", "");
    StringHandler::replace(moduleName, ".EXE", "");
}

void Logger::setRemotePort(int remotePort)
{
    remoteLogPort = remotePort;
}

void Logger::setRemoteHost(String remoteHost)
{
    remoteLogHost = remoteHost;
}

void Logger::setLogFileSize(int flsz)
{
    logFileSize = flsz;
}

void Logger::setLogDirectory(String dirpath)
{
    logDirectory = dirpath;

    char buffer[2048]={0};

    strcpy(buffer, logDirectory.c_str());

    if(buffer[strlen(buffer)-1]== '/' || buffer[strlen(buffer)-1]== '\\')
    {
        buffer[strlen(buffer)-1] = 0;
    }

    strcat(buffer, ".bak/");

    logBackupDirectory = buffer;

    if(!DirectoryHandler::isDirectory(buffer))
    {
        DirectoryHandler::createDirectory(buffer);
    }
}

void Logger::writeExtended(LogLevel llevel, const char *func, const char *file, int line, const char* format,...)
{
    char tempbuf[1024];
    memset((char*)&tempbuf[0],0,1024);
    va_list args;
    va_start(args, format);
    vsprintf(tempbuf, format, args);
    tempbuf[1023]=0;
    write(tempbuf,llevel,func,file,line);
}


