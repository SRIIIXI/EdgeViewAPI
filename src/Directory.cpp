#include "Directory.hpp"

DirectoryHandler::DirectoryHandler()
{

}

DirectoryHandler::~DirectoryHandler()
{

}

void DirectoryHandler::getParentDirectory(char *ptr)
{
    if(ptr == NULL)
        return;

    int len = strlen(ptr);

    if(len < 2)
        return;

    int ctr = len-1;

    while(true)
    {
        ptr[ctr] = 0;
        ctr--;
        if(ptr[ctr]== '/' || ptr[ctr]== '\\')
        {
            break;
        }
    }
}

void DirectoryHandler::getExtension(const char *ptr, std::string &str)
{
    int i = 0;
    str="";
    int len = strlen(ptr);

    if(len<1)
        return;

    for(i = len-1; i>2 ; i--)
    {
        if(ptr[i] == '.')
        {
            str = &ptr[i];
            break;
        }
    }

    return;
}

bool DirectoryHandler::fileExists(const char *ptr)
{
    FILE* fp = fopen(ptr, "r");

    if(fp)
    {
        fclose(fp);
        return true;
    }

    return false;
}

void DirectoryHandler::getName(const char *ptr, std::string &str)
{
    int i = 0;
    str="";
    int len = strlen(ptr);

    if(len<1)
        return;

    for(i = len-1;  ; i--)
    {
        if(ptr[i] == '\\' || ptr[i] == '/')
        {
            str = &ptr[i+1];
            break;
        }
    }

    return;
}

bool DirectoryHandler::isDirectory(const char *ptr)
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
        DWORD attr = GetFileAttributesA(ptr);

        if (attr == INVALID_FILE_ATTRIBUTES)
        {
            return false;
        }
        return true;
    #else

        DIR *dirp;

        dirp = opendir(ptr);
        if(dirp == NULL)
        {
            closedir(dirp);
            return false;
        }
        closedir(dirp);
        return true;
    #endif
}

void DirectoryHandler::getDirectoryList(const std::string &dirname, std::vector<FileInfo> &dlist)
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #else
        DIR *dir;
        struct dirent *dent;

        std::string fullpath;
        std::string str = dirname;

        if(str[str.length()-1]!='/')
        {
            str += "/";
        }

        fullpath = str;

        dlist.clear();

        dir = opendir(dirname.c_str());

        if(dir != NULL)
        {
            while(true)
            {
                dent = readdir(dir);
                if(dent == NULL)
                {
                    break;
                }

                if(dent->d_type == DT_DIR && (strcmp(dent->d_name,".")!=0 && strcmp(dent->d_name,"..")!=0))
                {
                    FileInfo  finfo;
                    finfo.Name = dent->d_name;
                    finfo.FullPath = fullpath + finfo.Name;

				    struct stat filestat;
				    stat(finfo.FullPath.c_str(), &filestat);
				    time_t createtm = filestat.st_mtime;
				    time_t modifytm;
				
				    if(filestat.st_ctime > filestat.st_mtime)
				    {
					    modifytm = filestat.st_mtime;
				    }
				    else
				    {
					    modifytm = filestat.st_ctime;
				    }

				    finfo.CreationTime.buildFromTimeT(createtm);
				    finfo.LastModifiedTime.buildFromTimeT(modifytm);

                    if( (strcmp(dent->d_name,".") == 0) || (strcmp(dent->d_name,"..") == 0) )
                    {
                        continue;
                    }

                    dlist.push_back(finfo);
                }
            }
        }

        closedir(dir);
    #endif
}

void DirectoryHandler::getFileList(const std::string &dirname, std::vector<FileInfo> &dlist, const std::string &extension)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#else
	DIR *dir = NULL;
    struct dirent *dent = NULL;

    std::string fullpath;
    std::string str = dirname;

    if(str[str.length()-1]!='/')
    {
        str += "/";
    }

    fullpath = str;

    dlist.clear();

    dir = opendir(dirname.c_str());

    if(dir != NULL)
    {
        while(true)
        {
            dent = readdir(dir);
            if(dent == NULL)
            {
                break;
            }

			if( (strcmp(dent->d_name,".") == 0) || (strcmp(dent->d_name,"..") == 0) )
			{
				continue;
			}
			else
            {
                FileInfo  finfo;
                finfo.Name = dent->d_name;
                finfo.FullPath = fullpath + finfo.Name;

                String ext;
                getExtension(dent->d_name, ext);

				struct stat filestat;
				stat(finfo.FullPath.c_str(), &filestat);
				time_t createtm = filestat.st_mtime;
				time_t modifytm;
				
				if(filestat.st_ctime > filestat.st_mtime)
				{
					modifytm = filestat.st_mtime;
				}
				else
				{
					modifytm = filestat.st_ctime;
				}

				finfo.CreationTime.buildFromTimeT(createtm);
				finfo.LastModifiedTime.buildFromTimeT(modifytm);

                if(extension.empty() || extension == ".*")
                {
                    dlist.push_back(finfo);
                }
                else
                {
                    if(extension == ext)
                    {
                        dlist.push_back(finfo);
                    }
                }
            }
        }

		closedir(dir);
    }
#endif
}

void DirectoryHandler::createDirectory(const char *str)
{
    #if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
        mkdir(str);
    #else
        mkdir(str,S_IRWXU);
    #endif
}




