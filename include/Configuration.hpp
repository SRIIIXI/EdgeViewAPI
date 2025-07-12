#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "StringHandler.hpp"
#include "Directory.hpp"

using namespace std;

extern "C"
{

class Configuration
{
public:
    Configuration();
    ~Configuration();
    void setFileName(std::string fname);
    void setDirectory(std::string dname);
    bool loadConfiguration();
    bool loadCustomConfiguration(const std::string&configFile);
    std::string getValue(const std::string&section, const std::string&settingKey, const std::string defval="");
    bool isSection(const std::string&section);
private:
    bool loadConfiguration(const std::string&configFile);
    void addSection(std::string&str, const std::map<std::string, std::string>&list);
    StringHandler _StrHdl;
    std::map<std::string, std::map<std::string, std::string>> configuration_map;
    std::string configuration_filename;
    std::string configuration_directory;
};

}

#endif // CONFIGURATION_H
