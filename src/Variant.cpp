#include "Variant.hpp"

Variant::Variant()
{
    variant_type = Void;
    memset((void*)&raw_buffer[0],0,256);
    data_size = 0;
}

Variant::Variant(const Variant& other)
{
    variant_type = other.variant_type;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer,other.raw_buffer, other.data_size);
    data_size = other.data_size;
}

Variant::~Variant()
{
}

Variant::Variant(const char val)
{
    variant_type = Char;
    raw_buffer[0] = val;
    raw_buffer[1] = 0;
    data_size = sizeof(char);
}

Variant::Variant(const unsigned char val)
{
    variant_type = UnsignedChar;
    raw_buffer[0] = val;
    raw_buffer[1] = 0;
    data_size = sizeof(unsigned char);
}

Variant::Variant(const char *val)
{
    int sz = strlen(val);
    if(sz > 255)
    {
        sz = 255;
    }

    variant_type = AsciiString;
    memset((void*)&raw_buffer[0],0,256);
    strncpy((char*)&raw_buffer[0], val, sz);
    data_size = sz;
}

Variant::Variant(const unsigned char* val, unsigned int &sz)
{
    if(sz > 255)
    {
        sz = 255;
    }
    variant_type = Raw;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sz);
    data_size = sz;
}

Variant::Variant(const bool val)
{
    variant_type = Boolean;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(bool));
    data_size = sizeof(bool);
}

Variant::Variant(const long val)
{
    variant_type = Number;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(long));
    data_size = sizeof(long);
}

Variant::Variant(const unsigned long val)
{
    variant_type = UnsignedNumber;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(unsigned long));
    data_size = sizeof(unsigned long);
}

Variant::Variant(const double val)
{
    variant_type = Decimal;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(double));
    data_size = sizeof(double);
}

Variant::Variant(const tm val)
{
    variant_type = TimeStamp;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(struct tm));
    data_size = sizeof(struct tm);
}

VariantType Variant::getType()
{
    return variant_type;
}

void Variant::setType(VariantType vtype)
{
    variant_type = vtype;
}

unsigned int Variant::getSize()
{
    return data_size;
}

const void *Variant::getData()
{
    return &raw_buffer[0];
}

void Variant::setData(const char val)
{
    variant_type = Char;
    raw_buffer[0] = val;
    raw_buffer[1] = 0;
    data_size = sizeof(char);
}

void Variant::setData(const unsigned char val)
{
    variant_type = UnsignedChar;
    raw_buffer[0] = val;
    raw_buffer[1] = 0;
    data_size = sizeof(unsigned char);
}

void Variant::setData(const char *val, bool trim)
{
    int sz = strlen(val);
    if(sz > 255)
    {
        sz = 255;
    }

    variant_type = AsciiString;
    memset((void*)&raw_buffer[0],0,256);
    strncpy((char*)&raw_buffer[0], val, sz);

    if(trim)
    {
        data_size = strlen((char*)&raw_buffer[0]);

        if(data_size<1)
            return;

        for(int i = data_size-1;  ; i--)
        {
            if(i < 0)
            {
                break;
            }

            if(raw_buffer[i]>32 && raw_buffer[i]<127)
            {
                break;
            }

            raw_buffer[i] = '\0';
        }

        data_size = strlen((char*)&raw_buffer[0]);
    }
}

void Variant::setData(const unsigned char* val, int &sz)
{
    if(sz > 255)
    {
        sz = 255;
    }
    variant_type = Raw;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sz);
    data_size = sz;
}

void Variant::setData(const bool val)
{
    variant_type = Boolean;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(bool));
    data_size = sizeof(bool);
}

void Variant::setData(const long val)
{
    variant_type = Number;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(long));
    data_size = sizeof(long);
}

void Variant::setData(const unsigned long val)
{
    variant_type = UnsignedNumber;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(unsigned long));
    data_size = sizeof(unsigned long);
}

void Variant::setData(const double val)
{
    variant_type = Decimal;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(double));
    data_size = sizeof(double);
}

void Variant::setData(const tm val)
{
    variant_type = TimeStamp;
    memset((void*)&raw_buffer[0],0,256);
    memcpy(raw_buffer, &val, sizeof(struct tm));
    data_size = sizeof(struct tm);
}

void Variant::getString(std::string &str)
{
    str.clear();

    char ptr[255];
    memset((void*)&ptr[0],0,255);

    switch(variant_type)
    {
        case Char:
        {
            sprintf(ptr,"%c", raw_buffer[0]);
            str = ptr;
            break;
        }
        case UnsignedChar:
        {
            sprintf(ptr,"%c",(char)raw_buffer[0]);
            str = ptr;
            break;
        }
        case AsciiString:
        {
            str = (char*)&raw_buffer[0];
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
            str = (char*)&raw_buffer[0];
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
    if(variant_type == Number)
    {
        long temp;
        memcpy((void*)&temp, (void*)&raw_buffer[0], sizeof(long));
        return temp;
    }

    return 0;
}

unsigned long Variant::getUnsignedNumber()
{
    if(variant_type == Number)
    {
        unsigned long temp;
        memcpy((void*)&temp, (void*)&raw_buffer[0], sizeof(unsigned long));
        return temp;
    }

    return 0;
}

double Variant::getReal()
{
    if(variant_type == Decimal)
    {
        double temp;
        memcpy((void*)&temp, (void*)&raw_buffer[0], sizeof(double));
        return temp;
    }

    return (double)0.0;
}

bool Variant::getBoolean()
{
    if(variant_type == Boolean)
    {
        bool temp;
        memcpy((void*)&temp, (void*)&raw_buffer[0], sizeof(bool));
        return temp;
    }
    return false;
}


Timestamp Variant::getTimestamp()
{
    if(variant_type != TimeStamp)
    {
        Timestamp ts;
        return ts;
    }

    struct tm temp;
    memcpy((void*)&temp, (void*)&raw_buffer[0], sizeof(struct tm));

    Timestamp ts(temp);
    return ts;
}

char Variant::getSignedChar()
{
    if(variant_type == Char || variant_type == AsciiString)
    {
        return raw_buffer[0];
    }

    return '\0';
}
