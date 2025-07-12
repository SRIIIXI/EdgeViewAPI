#include "Timestamp.hpp"

Timestamp::Timestamp()
{
    time_t rawtime;
    time ( &rawtime );
    time_info = *localtime(&rawtime);
}

Timestamp::Timestamp(time_t tinfo)
{
	time_info = *localtime(&tinfo);
}

Timestamp::Timestamp(const Timestamp& other)
{
    time_info = other.time_info;
}


Timestamp::Timestamp(const std::string str, const std::string format)
{
    time_t rawtime;
    time ( &rawtime );
    time_info = *localtime(&rawtime);

    if(str.length()!=14)
    {
        if(str.length()!=12)
        {
            return;
        }
    }

    size_t pos;
    pos = format.find("yyyy");
    if(pos==std::string::npos)
    {
        pos = format.find("yy");
        if(pos!=std::string::npos)
        {
            time_info.tm_year = atoi(str.substr(pos,2).c_str())+100;
        }
    }
    else
    {
        time_info.tm_year = atoi(str.substr(pos,4).c_str())-1900;
    }

    pos = format.find("MM");
    if(pos!=std::string::npos)
    {
        time_info.tm_mon = atoi(str.substr(pos,2).c_str())-1;
    }

    pos = format.find("dd");
    if(pos!=std::string::npos)
    {
        time_info.tm_mday = atoi(str.substr(pos,2).c_str());
    }

    pos = format.find("hh");
    if(pos!=std::string::npos)
    {
        time_info.tm_hour = atoi(str.substr(pos,2).c_str());
    }

    pos = format.find("mm");
    if(pos!=std::string::npos)
    {
        time_info.tm_min = atoi(str.substr(pos,2).c_str());
    }

    pos = format.find("ss");
    if(pos!=std::string::npos)
    {
        time_info.tm_sec = atoi(str.substr(pos,2).c_str());
    }
}

Timestamp::Timestamp(struct tm tinfo)
{
    time_info = tinfo;
}


void Timestamp::fromString(const std::string str, const std::string format, Timestamp &ts)
{
    ts.buildFromString(str,format);
}

void Timestamp::buildFromTm(const struct tm tmstruct)
{
	time_info = tmstruct;
}

void Timestamp::buildFromTimeT(time_t tinfo)
{
	time_info = *localtime(&tinfo);
}

void Timestamp::buildFromString(const std::string str, const std::string format)
{
    time_t rawtime;
    time ( &rawtime );
    time_info = *localtime(&rawtime);

    if(str.length()!=14)
    {
        if(str.length()!=12)
        {
            return;
        }
    }

    size_t pos;
    pos = format.find("yyyy");
    if(pos==std::string::npos)
    {
        pos = format.find("yy");
        if(pos!=std::string::npos)
        {
            time_info.tm_year = atoi(str.substr(pos,2).c_str())+100;
        }
    }
    else
    {
        time_info.tm_year = atoi(str.substr(pos,4).c_str())-1900;
    }

    pos = format.find("MM");
    if(pos!=std::string::npos)
    {
        time_info.tm_mon = atoi(str.substr(pos,2).c_str())-1;
    }

    pos = format.find("dd");
    if(pos!=std::string::npos)
    {
        time_info.tm_mday = atoi(str.substr(pos,2).c_str());
    }

    pos = format.find("hh");
    if(pos!=std::string::npos)
    {
        time_info.tm_hour = atoi(str.substr(pos,2).c_str());
    }

    pos = format.find("mm");
    if(pos!=std::string::npos)
    {
        time_info.tm_min = atoi(str.substr(pos,2).c_str());
    }

    pos = format.find("ss");
    if(pos!=std::string::npos)
    {
        time_info.tm_sec = atoi(str.substr(pos,2).c_str());
    }
}

Timestamp::~Timestamp()
{

}

Timestamp& Timestamp::operator=(const Timestamp& other)
{
    time_info = other.time_info;
    return *this;
}

bool Timestamp::operator!=( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    if(t1!=t2)
    {
        return true;
    }
    return false;
}

bool Timestamp::operator==( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    if(t1==t2)
    {
        return true;
    }
    return false;
}

bool Timestamp::operator>=( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    if(t1>=t2)
    {
        return true;
    }
    return false;
}

bool Timestamp::operator<=( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    if(t1<=t2)
    {
        return true;
    }
    return false;
}

bool Timestamp::operator>( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    if(t1>t2)
    {
        return true;
    }
    return false;
}

bool Timestamp::operator<( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    if(t1<t2)
    {
        return true;
    }
    return false;
}

Timestamp& Timestamp::operator+( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    t1 = t1+t2;

    time_info = *localtime(&t1);

    return *this;
}

Timestamp& Timestamp::operator-( const Timestamp& other)
{
    time_t t1 = mktime(&time_info);
    time_t t2 = mktime((tm*)&other.time_info);

    t1 = t1 - t2;

    time_info = *localtime(&t1);

    return *this;
}

std::string Timestamp::getDateString(const char *format)
{
    std::string str = format;
    size_t pos = 0;
    bool ap = false;

    char buffer[256];
    memset((char*)&buffer[0],0,256);

    pos = str.find("ss");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"ss","%S");
    }

    pos = str.find("mm");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"mm","%M");
    }

    pos = str.find("hh");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"hh","%H");
    }
    else
    {   pos = str.find("h");
        if(pos!=std::string::npos)
        {
            StringHandler::replace(str,"h","%I");
            ap = true;
        }
    }

    pos = str.find("dd");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"dd","%d");
    }

    pos = str.find("MMMM");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"MMMM","%B");
    }
    else
    {
        pos = str.find("MM");
        if(pos!=std::string::npos)
        {
            StringHandler::replace(str,"MM","%m");
        }
    }

    pos = str.find("yyyy");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"yyyy","%Y");
    }
    else
    {
        pos = str.find("yy");
        if(pos!=std::string::npos)
        {
            StringHandler::replace(str,"yy","%y");
        }
    }

    if(ap)
    {
        str += "%p";

    }

	if(time_info.tm_year < 100)
	{
		time_info.tm_year += 100;
	}

    strftime(buffer,256,str.c_str(),&time_info);

    return buffer;
}

std::string Timestamp::getDateString()
{
    std::string str = "yyyy/MM/dd hh:mm:ss";
    size_t pos = 0;
    bool ap = false;

    char buffer[256];
    memset((char*)&buffer[0],0,256);

    pos = str.find("ss");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"ss","%S");
    }

    pos = str.find("mm");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"mm","%M");
    }

    pos = str.find("hh");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"hh","%H");
    }
    else
    {   pos = str.find("h");
        if(pos!=std::string::npos)
        {
            StringHandler::replace(str,"h","%I");
            ap = true;
        }
    }

    pos = str.find("dd");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"dd","%d");
    }

    pos = str.find("MMMM");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"MMMM","%B");
    }
    else
    {
        pos = str.find("MM");
        if(pos!=std::string::npos)
        {
            StringHandler::replace(str,"MM","%m");
        }
    }

    pos = str.find("yyyy");
    if(pos!=std::string::npos)
    {
        StringHandler::replace(str,"yyyy","%Y");
    }
    else
    {
        pos = str.find("yy");
        if(pos!=std::string::npos)
        {
            StringHandler::replace(str,"yy","%y");
        }
    }

    if(ap)
    {
        str += "%p";

    }

	if(time_info.tm_year < 100)
	{
		time_info.tm_year += 100;
	}

    strftime(buffer,256,str.c_str(),&time_info);

    return buffer;
}


void Timestamp::addDays(int val)
{
    addSeconds(val*60*60*24);
}

void Timestamp::addHours(int val)
{
    addSeconds(val*60*60);
}

void Timestamp::addMinutes(int val)
{
    addSeconds(val*60);
}

void Timestamp::addSeconds(int val)
{
    // Commented due to GCC non POSIX behaviour
    //time_t t = mktime(&timeinfo);
    //t = t + val;
    //timeinfo = *localtime(&t);

    time_info.tm_sec = time_info.tm_sec + val;
    time_t t = mktime(&time_info);
    time_info = *localtime(&t);
}

int Timestamp::getDays()
{
    return time_info.tm_mday;
}

int Timestamp::getMonths()
{
    return time_info.tm_mon+1;
}

int Timestamp::getYears()
{
    return time_info.tm_year+1900;
}

int Timestamp::getHours()
{
    return time_info.tm_hour;
}

int Timestamp::getMinutes()
{
    return time_info.tm_min;
}

int Timestamp::getSeconds()
{
    return time_info.tm_sec;
}

const struct tm* Timestamp::getTimeStruct()
{
	return &time_info;
}

void Timestamp::setDay(int val)
{
    time_info.tm_mday = val;
}

void Timestamp::setMonth(int val)
{
    time_info.tm_mon = val-1;
}

void Timestamp::setYear(int val)
{
    time_info.tm_year = val-1900;
}

void Timestamp::setHour(int val)
{
    time_info.tm_hour = val;
}

void Timestamp::setMinute(int val)
{
    time_info.tm_min = val;
}

void Timestamp::setSecond(int val)
{
    time_info.tm_sec = val;
}
