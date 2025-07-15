#include "Logger.hpp"

Logger objLogger;

Logger*  Logger::GetInstance()
{
    return &objLogger;
}

Logger::Logger()
{
    remote_log_port = 9090;
    remote_log_host = "127.0.0.1";
    log_directory = "";
    log_file_size = 1024;
    file_descriptor = NULL;

    char pidstr[16];
    memset((char*)&pidstr[0],0,16);
    sprintf(pidstr,"%d",getpid());
    log_module_name = pidstr;

    log_level_map.clear();

    log_level_map[LOG_INFO]       ="Information";
    log_level_map[LOG_WARNING]    ="Warning    ";
    log_level_map[LOG_ERROR]      ="Error      ";
    log_level_map[LOG_CRITICAL]   ="Critical   ";
    log_level_map[LOG_PANIC]      ="Panic      ";
}

Logger::~Logger()
{
    stopLogging();
}

void Logger::stopLogging()
{
    if(file_descriptor!=NULL)
    {
        fflush(file_descriptor);
        fclose(file_descriptor);
    }
    log_level_map.clear();
}

void Logger::createBackupFileName(std::string &str)
{
    Timestamp ts;
    std::string tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
    char temp[1024];
    memset((char*)&temp[0],0,16);
    sprintf(temp,"%s_%s.log",log_module_name.c_str(),tstamp.c_str());
    str = temp;
}


void Logger::startLogging(LogFileMode fmode)
{
    log_file_mode = fmode;
    if(log_directory.empty() || log_directory.length()<1)
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

        log_directory = filepathbuffer;
    }

    log_filename = log_directory;
    log_filename += "/";
    log_filename += log_module_name;
    log_filename += ".log";

    if(log_file_mode == FILE_APPEND)
    {
        file_descriptor = fopen(log_filename.c_str(),"a+");
    }
    else
    {
       file_descriptor = fopen(log_filename.c_str(),"w+");
    }
}

void Logger::write(std::string logEntry, LogLevel llevel, const char* func, const char* file, int line)
{
    if(file_descriptor!=NULL)
    {
        int sz = ftell(file_descriptor);

        if(sz >= log_file_size*1024)
        {
            std::string temp;
            createBackupFileName(temp);
            std::string backupfile = log_backup_directory + temp;
            stopLogging();
            rename(log_filename.c_str(),backupfile.c_str());
            startLogging(log_file_mode);
        }

        std::string sourcefile;
        DirectoryHandler::getName(file, sourcefile);
        std::string lvel = log_level_map[llevel];

        Timestamp ts;
        std::string tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
        char temp[1024];
        memset((char*)&temp[0],0,16);

        char fname[256]={0};
        memcpy(fname,func,255);
        #if defined(_WIN32) || defined(WIN32)
        #else
        int pos = StringHandler::characterposition(fname,'(');
        fname[pos]=0;
        #endif

        std::string left, right;
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
        fprintf(file_descriptor,"%s\n",logEntry.c_str());
        fflush(file_descriptor);
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

        log_module_name = buffer;
    }
    else
    {
        log_module_name = mname;
    }

    StringHandler::replace(log_module_name, ".exe", "");
    StringHandler::replace(log_module_name, ".EXE", "");
}

void Logger::setRemotePort(int remotePort)
{
    remote_log_port = remotePort;
}

void Logger::setRemoteHost(std::string remoteHost)
{
    remote_log_host = remoteHost;
}

void Logger::setLogFileSize(int flsz)
{
    log_file_size = flsz;
}

void Logger::setLogDirectory(std::string dirpath)
{
    log_directory = dirpath;

    char buffer[2048]={0};

    strcpy(buffer, log_directory.c_str());

    if(buffer[strlen(buffer)-1]== '/' || buffer[strlen(buffer)-1]== '\\')
    {
        buffer[strlen(buffer)-1] = 0;
    }

    strcat(buffer, ".bak/");

    log_backup_directory = buffer;

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


