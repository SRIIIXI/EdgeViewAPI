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
    bool lockProcess(String &lockfileame);
    void getLockFileName(String &lockfileame);
    void getUserName(String &uName);
private:
    void getProcessName(String &processName);
    void getTempDir(String &dirName);
    int _LockFile;
    String _LockFileName;
};

#endif
