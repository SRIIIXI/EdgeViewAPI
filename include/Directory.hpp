#ifndef _DIRECTORY
#define _DIRECTORY

#include "Defines.hpp"
#include "StringHandler.hpp"
#include "Timestamp.hpp"

using namespace std;
typedef std::string String;

extern "C"
{

class FileInfo
{
public:
    String Name;
    String FullPath;
	Timestamp CreationTime;
	Timestamp LastModifiedTime;
};

typedef std::vector<FileInfo> DirList;

#define DIRECTORY_SEPARATOR '/'

class DirectoryHandler
{
public:
    DirectoryHandler();
    ~DirectoryHandler();
    static void getParentDirectory(char *ptr);
    static void getExtension(const char *ptr, String &str);
    static void getName(const char *ptr, String &str);
    static bool isDirectory(const char *ptr);
    static bool fileExists(const char *ptr);
    static void getDirectoryList(const String &dirname, DirList &dlist);
    static void getFileList(const String &dirname, DirList &dlist, const String &extension);
    static void createDirectory(const char *str);
};

}

#endif

