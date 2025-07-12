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
    void setFileName(String fname);
    void setDirectory(String dname);
    bool loadConfiguration();
    bool loadCustomConfiguration(const String &configFile);
    std::string getValue(const String &section, const String &settingKey, const String defval="");
    bool isSection(const String &section);
private:
    bool loadConfiguration(const String &configFile);
    void addSection(String &str, const key_value_list &list);
    StringHandler _StrHdl;
    std::map<String,key_value_list> _ConfigurationMap;
    String _ConfigFileName;
    String _ConfigDirName;
};

}

#endif // CONFIGURATION_H
