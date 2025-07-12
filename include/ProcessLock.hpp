#ifndef _PROCESS_LOCK
#define _PROCESS_LOCK
#include "Defines.hpp"
#include "StringHandler.hpp"
#include "Logger.hpp"

class ProcessLock
{
public:
    ProcessLock();
    virtual ~ProcessLock();
    bool lockProcess(std::string&lockfileame);
    void getLockFileName(std::string&lockfileame);
    void getUserName(std::string&uName);
private:
    void getProcessName(std::string&processName);
    void getTempDir(std::string&dirName);
    int lock_file_id;
    std::string lock_filename;
};

#endif
