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

void DirectoryHandler::getExtension(const char *ptr, String &str)
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

void DirectoryHandler::getName(const char *ptr, String &str)
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
    DIR *dirp;

    dirp = opendir(ptr);
    if(dirp == NULL)
    {
        closedir(dirp);
        return false;
    }
    closedir(dirp);
    return true;
}

void DirectoryHandler::getDirectoryList(const String &dirname, DirList &dlist)
{
    DIR *dir;
    struct dirent *dent;

    String fullpath;
    String str = dirname;

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
}

void DirectoryHandler::getFileList(const String &dirname, DirList &dlist, const String &extension)
{
	/*
    DIR *dir;
    struct dirent *dent;

    String fullpath;
    String str = dirname;

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

            if(dent->d_type == DT_REG)
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
    }

    closedir(dir);
	*/

	// Workaround for GCC 4.9 bug

	DIR *dir = NULL;
    struct dirent *dent = NULL;

    String fullpath;
    String str = dirname;

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
}

void DirectoryHandler::createDirectory(const char *str)
{
    mkdir(str,S_IRWXU);
}




