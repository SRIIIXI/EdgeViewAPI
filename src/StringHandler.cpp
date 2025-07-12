#include "StringHandler.hpp"

StringHandler::StringHandler()
{
}

StringHandler::~StringHandler()
{
}

int StringHandler::substringposition(const char* str,const char* substr)
{
    char* pdest = (char*)strstr( str, substr );
    if(pdest == 0)
    {
        return -1;
    }
    int result = pdest - str;
    return result;
}

void StringHandler::split(const std::string &str, std::vector<std::string> &tokens, const std::string &delimiters)
{
    // Skip delimiters at beginning
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first non-delimiter
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next non-delimiter
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void StringHandler::split(const std::string &str, std::vector<std::string> &tokens, char delim)
{
    std::stringstream ss(str); //convert string to stream
    std::string item;

    while(getline(ss, item, delim))
    {
        tokens.push_back(item); //add token to vector
    }
}

void StringHandler::split(const std::string &str, std::list<std::string> &tokens, const std::string &delimiters)
{
    // Skip delimiters at beginning
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first non-delimiter
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next non-delimiter
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void StringHandler::split(const std::string &str, std::list<std::string> &tokens, char delim)
{
    std::stringstream ss(str); //convert string to stream
    std::string item;

    while(getline(ss, item, delim))
    {
        tokens.push_back(item); //add token to vector
    }
}


void StringHandler::split(const std::string &str, char delim, std::string &keystr, std::string &valuestr)
{
    std::stringstream ss(str); //convert string to stream
    std::string item;

    int ctr = 0;

    while(getline(ss, item, delim))
    {
        if(ctr==0)
        {
            keystr = item;
            ctr++;
            continue;
        }

        if(ctr==1)
        {
            valuestr = item;
            ctr++;
            continue;
        }

        if(ctr>1)
        {
            valuestr += delim;
            valuestr += item;
            ctr++;
        }
    }
}

int StringHandler::split(const std::string &str, const std::string &delim, std::string &keystr, std::string &valuestr)
{
    int pos = str.find(delim);

    if(pos == -1)
    {
        return pos;
    }

    char *tptr = new char[pos+1];
    memset(tptr,0, pos+1);

    memcpy(tptr, str.c_str(), pos);

    keystr = tptr;

    delete tptr;

    tptr = NULL;

    const char *ptr = str.c_str();

    valuestr = &ptr[pos+delim.length()];

    return pos;
}

void StringHandler::replace(std::string &srcstr, const char oldchar, const char newchar)
{
    int len = srcstr.length();
    for(int ctr = 0 ; ctr < len ; ctr++)
    {
        if(srcstr[ctr] == oldchar)
        {
            srcstr[ctr] = newchar;
        }
    }
}


void  StringHandler::replace(std::string &srcstr, const std::string oldpattern, const std::string newpattern)
{
    if (oldpattern.length() == 0 || srcstr.length() == 0)
    {
        return;
    }

    size_t idx = 0;

    for(;;)
    {
        idx = srcstr.find( oldpattern, idx);

        if (idx == std::string::npos)
        {
            break;
        }

        srcstr.replace( idx, oldpattern.length(), newpattern);
        idx += newpattern.length();
    }
}

void StringHandler::replace(std::string &srcstr, const std::string oldpattern, const double npattern)
{
	replace(srcstr, oldpattern, (float)npattern);
}

void StringHandler::replace(std::string &srcstr, const std::string oldpattern, const int npattern)
{
	replace(srcstr, oldpattern, (long)npattern);
}

void StringHandler::replace(std::string &srcstr, const std::string oldpattern, const char newchar)
{
	char buff[2]={0};
	buff[0] = newchar;
	replace(srcstr, oldpattern, buff);
}


void StringHandler::replace(std::string &srcstr, const std::string oldpattern, const float npattern)
{
    if (oldpattern.length() == 0 || srcstr.length() == 0)
    {
        return;
    }

	char ptr[64] = {0};
    sprintf(ptr,"%10.6lf", npattern);
    std::string newpattern = ptr;

    size_t idx = 0;

    for(;;)
    {
        idx = srcstr.find( oldpattern, idx);

        if (idx == std::string::npos)
        {
            break;
        }

        srcstr.replace( idx, oldpattern.length(), newpattern);
        idx += newpattern.length();
    }
}

void StringHandler::replace(std::string &srcstr, const std::string oldpattern, const long npattern)
{
    if (oldpattern.length() == 0 || srcstr.length() == 0)
    {
        return;
    }

    size_t idx = 0;

	char ptr[64] = {0};
    sprintf(ptr,"%lu", npattern);
    std::string newpattern = ptr;

	for(;;)
    {
        idx = srcstr.find( oldpattern, idx);

        if (idx == std::string::npos)
        {
            break;
        }

        srcstr.replace( idx, oldpattern.length(), newpattern);
        idx += newpattern.length();
    }
}


const char *StringHandler::lefttrim(const std::string str)
{
   const char *buf=str.c_str();

   for ( NULL; *buf && isspace(*buf); buf++);
   return buf;
}

void StringHandler::alltrim(std::string &str)
{
    char buffer[4096];
    memset((char*)&buffer,0,4096);
    strcpy(buffer,str.c_str());

    int len = strlen(buffer);

    if(len<1)
        return;

    for(int i = len-1;  ; i--)
    {
        if(!isspace(buffer[i]) || i < 0)
        {
            break;
        }
        buffer[i] = '\0';
    }

    len = strlen(buffer);

    if(len<1)
    {
        str = buffer;
        return;
    }

    const char *buf=(const char*)&buffer[0];

    for ( NULL; *buf && isspace(*buf); buf++);

    str = buf;
}

void StringHandler::alltrim(char *str)
{
    int len = strlen(str);

    if(len<1)
        return;

    for(int i = len-1;  ; i--)
    {
        if(!isspace(str[i]) || i < 0)
        {
            break;
        }
        str[i] = '\0';
    }

    len = strlen(str);

    if(len<1)
    {
        return;
    }

    const char *buf=(const char*)&str[0];

    for ( NULL; *buf && isspace(*buf); buf++);

    str = (char*)buf;
}

bool StringHandler::isspace(int in)
{
   if ((in == 0x20) || (in >= 0x09 && in <= 0x0D)) return true;
   return false;
}

int StringHandler::charcount(const char *str, char ch)
{
    int c=0;
    for(int i=0; str[i] != '\0' ;i++)
        if(str[i]==ch)
            c++;

    return c;
}

int	StringHandler::characterposition(const char* str,const char ch)
{
    for(int ctr = 0; str[ctr] != '\0'; ctr++)
    {
        if(str[ctr]==ch)
        {
            return ctr;
        }
    }
    return -1;
}

void StringHandler::realToString(std::string &str, const double val)
{
    char ptr[255];
    memset((void*)&ptr[0],0,255);
    sprintf(ptr,"%10.6lf",val);
    str = ptr;
}
