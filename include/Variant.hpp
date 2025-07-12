#ifndef _VARIANT_TYPE
#define _VARIANT_TYPE

#include "Defines.hpp"
#include "StringHandler.hpp"
#include "Timestamp.hpp"

using namespace std;

extern "C"
{

typedef enum VariantType
{
    Void =0,
    Char =1,
    UnsignedChar =2,
    AsciiString=3,
    Boolean=4,
    Number=5,
    UnsignedNumber=6,
    Decimal=7,
    TimeStamp=8,
    Raw=9
}VariantType;

class Variant
{
public:
    Variant();
    ~Variant();
    Variant(const Variant& other);
    Variant(const char val);
    Variant(const unsigned char val);
    Variant(const char* val);
    Variant(const unsigned char* val, unsigned int &sz);
    Variant(const bool val);
    Variant(const long val);
    Variant(const unsigned long val);
    Variant(const double val);
    Variant(const struct tm val);

    const void* getData();
    void getString(std::string &str);
    long getSignedNumber();
    unsigned long getUnsignedNumber();
    double getReal();
    bool getBoolean();
    Timestamp getTimestamp();
    char getSignedChar();

    VariantType getType();
    void setType(VariantType vtype);
    unsigned int getSize();

    void setData(const char val);
    void setData(const unsigned char val);
    void setData(const char* val, bool trim=false);
    void setData(const unsigned char* val, int &sz);
    void setData(const bool val);
    void setData(const long val);
    void setData(const unsigned long val);
    void setData(const double val);
    void setData(const struct tm val);

private:
    VariantType variant_type;
    unsigned char raw_buffer[256];
    unsigned int data_size;
};

}

#endif

