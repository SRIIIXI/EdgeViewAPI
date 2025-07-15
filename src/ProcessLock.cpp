#include "ProcessLock.hpp"

ProcessLock::ProcessLock()
{
    lock_file_id = 0;
}

bool ProcessLock::lockProcess(std::string &lockfileame)
{
    /*
    getLockFileName(lock_filename);
    if(lock_file_id != 0 && lock_file_id != -1)
    {
        //File is already open
        return false;
    }

    lock_file_id = open(lock_filename.c_str(), O_CREAT|O_RDWR, 0666);
    if(lock_file_id != -1)
    {
        off_t sz = 0;
        int rc = lockf(lock_file_id, F_TLOCK, sz);
        if(rc == -1)
        {
            close(lock_file_id);
            lock_file_id = 0;
            if(EAGAIN == errno || EACCES == errno)
            {
            }
            else
            {
            }
            return false;
        }

        // Okay! We got a lock
        lockfileame = lock_filename;
        return true;
    }
    else
    {
        lock_file_id = 0;
        return false;
    }
    */

    return false;
}

void ProcessLock::getLockFileName(std::string &lockfileame)
{
    std::string procname, uname, tmpdir;
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
    //close(lock_file_id);
}

void ProcessLock::getProcessName(std::string &processName)
{
    /*
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

        std::vector<std::string> strlist;
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
    */
}

void ProcessLock::getUserName(std::string &uName)
{
    #if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
        char username[1025] = { 0 };
        DWORD buffer_size = 1024;
        ::GetUserName(username, &buffer_size);
	    uName = std::string(username);
    #else
        uName = getenv("USER");
    #endif
}

void ProcessLock::getTempDir(std::string &dirName)
{
    #if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
        char tmppath[2049] = { 0 };
        DWORD buffer_size = 2048;
        ::GetTempPath(buffer_size, tmppath);
        dirName = std::string(tmppath);
    #else
        dirName = getenv("TMPDIR");
    #endif
}
