#include "ProcessLock.hpp"

ProcessLock::ProcessLock()
{
    _LockFile = 0;
}

bool ProcessLock::lockProcess(String &lockfileame)
{
    getLockFileName(_LockFileName);
    if(_LockFile != 0 && _LockFile != -1)
    {
        //File is already open
        return false;
    }

    _LockFile = open(_LockFileName.c_str(), O_CREAT|O_RDWR, 0666);
    if(_LockFile != -1)
    {
        off_t sz = 0;
        int rc = lockf(_LockFile, F_TLOCK, sz);
        if(rc == -1)
        {
            close(_LockFile);
            _LockFile = 0;
            if(EAGAIN == errno || EACCES == errno)
            {
            }
            else
            {
            }
            return false;
        }

        // Okay! We got a lock
        lockfileame = _LockFileName;
        return true;
    }
    else
    {
        _LockFile = 0;
        return false;
    }

    return false;
}

void ProcessLock::getLockFileName(String &lockfileame)
{
    String procname, uname, tmpdir;
    getProcessName(procname);
    getUserName(uname);
    getTempDir(tmpdir);

    lockfileame = tmpdir;

    lockfileame += "/";
    lockfileame += procname;
    lockfileame += ".";
    lockfileame += uname;
    lockfileame += ".lock";
}

ProcessLock::~ProcessLock()
{
    close(_LockFile);
}

void ProcessLock::getProcessName(String &processName)
{
    FILE *pipein_fp;
    char readbuf[80];

    int ownpid = getpid();

    char cmdbuffer[256]={0};
    sprintf(cmdbuffer, "ps aux | tr -s ' ' | cut -d ' ' -f2,11 | grep %d", ownpid);

    // Create one way pipe line with call to popen()
    if (( pipein_fp = popen(cmdbuffer, "r")) == NULL)
    {
            return;
    }


    bool found = false;

    // Processing loop
    while(true)
    {
        memset((void*)&readbuf, 0, sizeof(readbuf));
        char *ptr = fgets(readbuf, 80, pipein_fp);
        if(ptr == NULL)
        {
            break;
        }

        for(int idx = 0; idx < 80; idx++)
        {
            if(readbuf[idx] == '\r' || readbuf[idx] == '\n')
            {
                readbuf[idx] = 0;
            }
        }

        if(strlen(readbuf) < 1)
        {
            continue;
        }

        // Check for zombie processes
        if(strstr(readbuf, "<defunct>") != NULL)
        {
            continue;
        }

        std::vector<String> strlist;
        StringHandler::split(readbuf, strlist, " ");

        if(strlist.size() < 2)
        {
            continue;
        }

        processName = strlist[1];
        StringHandler::replace(processName, ".", "");
        StringHandler::replace(processName, "&", "");
        StringHandler::replace(processName, "/", "");

    }
    // Close the pipes
    pclose(pipein_fp);
}

void ProcessLock::getUserName(String &uName)
{
//    register struct passwd *pw;
//    register uid_t uid;
//    int c;

//    uid = geteuid ();
//    pw = getpwuid (uid);
//    if (pw)
//    {
//        processName = pw->pw_name;
//    }

    uName = getenv("USER");
}

void ProcessLock::getTempDir(String &dirName)
{
    dirName = "/tmp";
}
