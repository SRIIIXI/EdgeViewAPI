#include "Variant.hpp"

Variant::Variant()
{
    dataType = Void;
    memset((void*)&rawBuffer[0],0,256);
    dataSize = 0;
}

Variant::Variant(const Variant& other)
{
    dataType = other.dataType;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer,other.rawBuffer, other.dataSize);
    dataSize = other.dataSize;
}

Variant::~Variant()
{
}

Variant::Variant(const char val)
{
    dataType = Char;
    rawBuffer[0] = val;
    rawBuffer[1] = 0;
    dataSize = sizeof(char);
}

Variant::Variant(const unsigned char val)
{
    dataType = UnsignedChar;
    rawBuffer[0] = val;
    rawBuffer[1] = 0;
    dataSize = sizeof(unsigned char);
}

Variant::Variant(const char *val)
{
    int sz = strlen(val);
    if(sz > 255)
    {
        sz = 255;
    }

    dataType = AsciiString;
    memset((void*)&rawBuffer[0],0,256);
    strncpy((char*)&rawBuffer[0], val, sz);
    dataSize = sz;
}

Variant::Variant(const unsigned char* val, unsigned int &sz)
{
    if(sz > 255)
    {
        sz = 255;
    }
    dataType = Raw;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sz);
    dataSize = sz;
}

Variant::Variant(const bool val)
{
    dataType = Boolean;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(bool));
    dataSize = sizeof(bool);
}

Variant::Variant(const long val)
{
    dataType = Number;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(long));
    dataSize = sizeof(long);
}

Variant::Variant(const unsigned long val)
{
    dataType = UnsignedNumber;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(unsigned long));
    dataSize = sizeof(unsigned long);
}

Variant::Variant(const double val)
{
    dataType = Decimal;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(double));
    dataSize = sizeof(double);
}

Variant::Variant(const tm val)
{
    dataType = TimeStamp;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(struct tm));
    dataSize = sizeof(struct tm);
}

VariantType Variant::getType()
{
    return dataType;
}

void Variant::setType(VariantType vtype)
{
    dataType = vtype;
}

unsigned int Variant::getSize()
{
    return dataSize;
}

const void *Variant::getData()
{
    return &rawBuffer[0];
}

void Variant::setData(const char val)
{
    dataType = Char;
    rawBuffer[0] = val;
    rawBuffer[1] = 0;
    dataSize = sizeof(char);
}

void Variant::setData(const unsigned char val)
{
    dataType = UnsignedChar;
    rawBuffer[0] = val;
    rawBuffer[1] = 0;
    dataSize = sizeof(unsigned char);
}

void Variant::setData(const char *val, bool trim)
{
    int sz = strlen(val);
    if(sz > 255)
    {
        sz = 255;
    }

    dataType = AsciiString;
    memset((void*)&rawBuffer[0],0,256);
    strncpy((char*)&rawBuffer[0], val, sz);

    if(trim)
    {
        dataSize = strlen((char*)&rawBuffer[0]);

        if(dataSize<1)
            return;

        for(int i = dataSize-1;  ; i--)
        {
            if(i < 0)
            {
                break;
            }

            if(rawBuffer[i]>32 && rawBuffer[i]<127)
            {
                break;
            }

            rawBuffer[i] = '\0';
        }

        dataSize = strlen((char*)&rawBuffer[0]);
    }
}

void Variant::setData(const unsigned char* val, int &sz)
{
    if(sz > 255)
    {
        sz = 255;
    }
    dataType = Raw;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sz);
    dataSize = sz;
}

void Variant::setData(const bool val)
{
    dataType = Boolean;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(bool));
    dataSize = sizeof(bool);
}

void Variant::setData(const long val)
{
    dataType = Number;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(long));
    dataSize = sizeof(long);
}

void Variant::setData(const unsigned long val)
{
    dataType = UnsignedNumber;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(unsigned long));
    dataSize = sizeof(unsigned long);
}

void Variant::setData(const double val)
{
    dataType = Decimal;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(double));
    dataSize = sizeof(double);
}

void Variant::setData(const tm val)
{
    dataType = TimeStamp;
    memset((void*)&rawBuffer[0],0,256);
    memcpy(rawBuffer, &val, sizeof(struct tm));
    dataSize = sizeof(struct tm);
}

void Variant::getString(std::string &str)
{
    str.clear();

    char ptr[255];
    memset((void*)&ptr[0],0,255);

    switch(dataType)
    {
        case Char:
        {
            sprintf(ptr,"%c", rawBuffer[0]);
            str = ptr;
            break;
        }
        case UnsignedChar:
        {
            sprintf(ptr,"%c",(char)rawBuffer[0]);
            str = ptr;
            break;
        }
        case AsciiString:
        {
            str = (char*)&rawBuffer[0];
            break;
        }
        case Boolean:
        {
            if(getBoolean())
            {
                str = "true";
            }
            else
            {
                str = "false";
            }
            break;
        }
        case Number:
        {
            sprintf(ptr,"%ld", getSignedNumber());
            str = (char*)&ptr[0];
            break;
        }
        case UnsignedNumber:
        {
            sprintf(ptr,"%lu", getUnsignedNumber());
            str = ptr;
            break;
        }
        case Decimal:
        {
            sprintf(ptr,"%10.6lf", getReal());
            str = ptr;
            break;
        }
        case TimeStamp:
        {
            Timestamp dt = getTimestamp();
            str = dt.getDateString("yyyy/MM/dd hh:mm:ss");
            break;
        }
        case Raw:
        {
            str = (char*)&rawBuffer[0];
            break;
        }
        case Void:
        {
            str = "";
            break;
        }
        default:
        {
            str = "";
            break;
        }
    }
}

long Variant::getSignedNumber()
{
    if(dataType == Number)
    {
        long temp;
        memcpy((void*)&temp, (void*)&rawBuffer[0], sizeof(long));
        return temp;
    }

    return 0;
}

unsigned long Variant::getUnsignedNumber()
{
    if(dataType == Number)
    {
        unsigned long temp;
        memcpy((void*)&temp, (void*)&rawBuffer[0], sizeof(unsigned long));
        return temp;
    }

    return 0;
}

double Variant::getReal()
{
    if(dataType == Decimal)
    {
        double temp;
        memcpy((void*)&temp, (void*)&rawBuffer[0], sizeof(double));
        return temp;
    }

    return (double)0.0;
}

bool Variant::getBoolean()
{
    if(dataType == Boolean)
    {
        bool temp;
        memcpy((void*)&temp, (void*)&rawBuffer[0], sizeof(bool));
        return temp;
    }
    return false;
}


Timestamp Variant::getTimestamp()
{
    if(dataType != TimeStamp)
    {
        Timestamp ts;
        return ts;
    }

    struct tm temp;
    memcpy((void*)&temp, (void*)&rawBuffer[0], sizeof(struct tm));

    Timestamp ts(temp);
    return ts;
}

char Variant::getSignedChar()
{
    if(dataType == Char || dataType == AsciiString)
    {
        return rawBuffer[0];
    }

    return '\0';
}
