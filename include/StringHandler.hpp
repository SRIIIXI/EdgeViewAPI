#ifndef _STRING_HANDLER
#define _STRING_HANDLER

#include "Defines.hpp"

extern "C"
{

class StringHandler
{
public:
    StringHandler();
    ~StringHandler();

    static void split(const std::string &str, std::vector<std::string> &tokens, const std::string &delimiters = " ");
    static void split(const std::string &str, std::vector<std::string> &tokens, char delim=' ');
    static void split(const std::string &str, std::list<std::string> &tokens, const std::string &delimiters = " ");
    static void split(const std::string &str, std::list<std::string> &tokens, char delim=' ');
    static void split(const std::string &str, char delim, std::string &keystr, std::string &valuestr);
    static int split(const std::string &str, const std::string &delim, std::string &keystr, std::string &valuestr);

    static void replace(std::string &srcstr, const std::string oldpattern, const std::string newpattern);
    static void replace(std::string &srcstr, const std::string oldpattern, const char newchar);
    static void	replace(std::string &srcstr, const char oldchar, const char newchar);
    static void replace(std::string &srcstr, const std::string oldpattern, const float npattern);
    static void replace(std::string &srcstr, const std::string oldpattern, const long npattern);
    static void replace(std::string &srcstr, const std::string oldpattern, const double npattern);
    static void replace(std::string &srcstr, const std::string oldpattern, const int npattern);

    static const char* lefttrim(const std::string str);
    static void alltrim(std::string &str);
    static void alltrim(char* str);
    static int charcount(const char *str, char ch);
    static bool isspace(int in);
    static int	substringposition(const char* str,const char* substr);
    static int	characterposition(const char* str,const char ch);

    static void realToString(std::string &str, const double val);
};

}

#endif

