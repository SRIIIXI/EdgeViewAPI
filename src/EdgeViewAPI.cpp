// HTTPServer.cpp : Defines the entry point for the console application.
//

#include "NetworkHelper.hpp"
#include "Responder.hpp"
#include "HttpServer.hpp"
#include "HttpHandler.hpp"
#include "Directory.hpp"
#include "Configuration.hpp"
#include "ProcessLock.hpp"
#include "Logger.hpp"
#include "SignalHandler.hpp"
#include "EventHandler.hpp"
#include "Version.hpp"

int main(int argc, char* argv[])
{
    if(argc > 1 && (strcmp(argv[1], "-v")==0))
    {
        printf("EdgeViewAPI server version %s\n", appversion);
        return 0;
    }

    HttpServer lstnr;
    Configuration cfg;
    SignalHandler sdlr;

    int  serverport =  9090;

    bool path_found = false;
    std::string standard_path;
    std::string log_file_path;
    std::string cfg_file_path;
    std::filesystem::file_status s;
    std::string home = std::getenv("HOME");
    standard_path += home;

    log_file_path = standard_path;
    cfg_file_path = standard_path;

    log_file_path += "/.local/EdgeViewAPI/logs";
    cfg_file_path += "/.config/EdgeViewAPI";

    std::filesystem::path lp(log_file_path);
    s = std::filesystem::file_status{};

    path_found = std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(lp);
    if (!path_found)
    {
        std::filesystem::create_directory(std::string(std::getenv("HOME"))+"/.local");
        std::filesystem::create_directory(std::string(std::getenv("HOME"))+"/.local/EdgeViewAPI");
        std::filesystem::create_directory(std::string(std::getenv("HOME"))+"/.local/EdgeViewAPI/logs");
    }

    Logger::GetInstance()->setLogDirectory(log_file_path);
    Logger::GetInstance()->setLogFileSize(1024);
    Logger::GetInstance()->setModuleName("EdgeViewAPI");
    Logger::GetInstance()->startLogging(FILE_APPEND);

    std::filesystem::path cp(cfg_file_path);
    s = std::filesystem::file_status{};
    bool config_found = false;

    path_found = std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(cp);
    if (!path_found)
    {
        std::filesystem::create_directory(std::string(std::getenv("HOME"))+"/.config");
        std::filesystem::create_directory(std::string(std::getenv("HOME"))+"/.config/EdgeViewAPI");
    }
    else
    {
        cfg.setDirectory(cfg_file_path);
        cfg.setFileName("EdgeViewAPI.ini");
        if(!cfg.loadConfiguration())
        {
            config_found = false;
            writeLog("Error while loading standard configuration", LOG_ERROR);
        }
        else
        {
            config_found = true;
            serverport =  atoi(cfg.getValue("Server", "Port", "9090").c_str());
        }
    }

    if(!path_found || !config_found)
    {
        serverport =  9090;
    }

    sdlr.registerSignalHandlers();
    sdlr.registerCallbackClient(&lstnr);

    ProcessLock plk;
    std::string un;

    plk.getUserName(un);

    if(un == "root")
    {
        writeLog("This process cannot be allowed to be run under root privileges", LOG_ERROR);
        return -1;
    }

    std::string lfile;
    plk.getLockFileName(lfile);
    if(!plk.lockProcess(lfile))
    {
        writeLog("Could not acquire process lock\n", LOG_ERROR);
        return -1;
    }

    char logbuffer[128]={0};

    memset((char*)&logbuffer[0], 0, sizeof(logbuffer));
    sprintf(logbuffer, "Server Port : %d", serverport);
    writeLog(logbuffer, LOG_INFO);

    writeLog("Server Intialized", LOG_INFO);

    memset((char*)&logbuffer[0], 0, sizeof(logbuffer));
    sprintf(logbuffer, "Listener starting on port %d", serverport);
    writeLog(logbuffer, LOG_INFO);

    memset((char*)&logbuffer[0], 0, sizeof(logbuffer));
    sprintf(logbuffer, "The process ID is %d", getpid());
    writeLog(logbuffer, LOG_INFO);

    RunState rstate;
    rstate = lstnr.run(serverport);

    if(rstate == BindFailed)
	{
        writeLog("Socket bind() failed", LOG_ERROR);
        return -1;
	}

    if(rstate == ListenFailed)
    {
        memset((char*)&logbuffer[0], 0, sizeof(logbuffer));
        sprintf(logbuffer, "Socket listen() failed on port %d", serverport);
        writeLog(logbuffer, LOG_ERROR);
        return -1;
    }

    memset((char*)&logbuffer[0], 0, sizeof(logbuffer));
    writeLog("Server has finished monitoring queue, exiting now", LOG_INFO);

	return 0;
}
