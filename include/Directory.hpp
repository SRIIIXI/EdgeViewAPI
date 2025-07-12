#ifndef _DIRECTORY
#define _DIRECTORY

#include "Defines.hpp"
#include "StringHandler.hpp"
#include "Timestamp.hpp"

using namespace std;

extern "C"
{

typedef struct FileInfo
{
    std::string Name;
    std::string FullPath;
	Timestamp CreationTime;
	Timestamp LastModifiedTime;
}FileInfo;

#define DIRECTORY_SEPARATOR '/'

class DirectoryHandler
{
public:
    DirectoryHandler();
    ~DirectoryHandler();
    static void getParentDirectory(char *ptr);
    static void getExtension(const char *ptr, std::string &str);
    static void getName(const char *ptr, std::string &str);
    static bool isDirectory(const char *ptr);
    static bool fileExists(const char *ptr);
    static void getDirectoryList(const std::string &dirname, std::vector<FileInfo> &dlist);
    static void getFileList(const std::string &dirname, std::vector<FileInfo> &dlist, const std::string &extension);
    static void createDirectory(const char *str);
};

}

#endif

