#include "Configuration.hpp"

Configuration::Configuration()
{
    _ConfigFileName = "";
}

Configuration::~Configuration()
{
}

void Configuration::setFileName(String fname)
{
    _ConfigFileName = fname;
}

void Configuration::setDirectory(String dname)
{
    _ConfigDirName = dname;
}

bool Configuration::isSection(const String &section)
{
    map<String,key_value_list>::const_iterator confmapiter;

    confmapiter = _ConfigurationMap.find(section);
    if(confmapiter == _ConfigurationMap.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::string Configuration::getValue(const String &section, const String &settingKey, const String defval)
{
    map<String,key_value_list>::const_iterator confmapiter;
    key_value_list::const_iterator kviter;
    String str;

    confmapiter = _ConfigurationMap.find(section);
    if(confmapiter == _ConfigurationMap.end())
    {
        return defval;
    }
    else
    {
        key_value_list list = confmapiter->second;
        kviter = list.find(settingKey);

        if(kviter == list.end())
        {
            return defval;
        }
        str = (String)kviter->second;
    }
    return str;
}

bool Configuration::loadCustomConfiguration(const String &configFile)
{
    return loadConfiguration(configFile);
}

bool Configuration::loadConfiguration()
{
    char filepathbuffer[2048];
    memset((char*)&filepathbuffer[0],0,sizeof(filepathbuffer));

    strcat(filepathbuffer, _ConfigDirName.c_str());
    strcat(filepathbuffer, "/");
    strcat(filepathbuffer, _ConfigFileName.c_str());

    if(!loadConfiguration(filepathbuffer))
    {
        return false;
    }

    return true;
}

bool Configuration::loadConfiguration(const String &configFile)
{
    std::ifstream cfgfile(configFile.c_str());
    std::string line, leftstr, rightstr;
    string_list linelist;

    // Following is a Windows INI style configuration file parsing algorithm
    // The first iteration only loads relevent lines from as a list of strings
    if(!cfgfile.is_open())
    {
        return false;
    }
    else
    {
        while(cfgfile.good())
        {
              line.erase();
              std::getline(cfgfile,line);
              StringHandler::alltrim(line);

              if(line.length() < 1 || line[0]==';' || line[0]=='#' || line.empty())
              {
                  //Skip comment or blank lines;
                  continue;
              }

              if(!isalnum(line[0]))
              {
                  if(line[0]=='[' && line[line.length()-1]==']')
                  {
                      //Section header
                      linelist.push_back(line);
                  }
                  //Garbage or Invalid line
                  continue;
              }
              else
              {
                  //Normal line
                  linelist.push_back(line);
              }
        }
        // The file can be closed off
        cfgfile.close();
    }

    //Now we would iterate the string list and segregate key value pairs by section groups
    String curSecHeader = "";
    key_value_list kvlist;

    for(string_list::size_type i = 0; i != linelist.size(); i++)
    {
        line = linelist[i];
        //Section header line
        if(line[0]=='[' && line[line.length()-1]==']')
        {
            //Check whether this is the first instance of a section header
            if(_ConfigurationMap.size()<1)
            {
                //Don't need to do anything
                if(curSecHeader.length()<1)
                {
                }
                else
                {
                    //We reach here when a section is being read for the first time
                    addSection(curSecHeader,kvlist);
                }
            }
            else
            {
                //Before staring a new section parsing we need to store the last one
                addSection(curSecHeader,kvlist);
            }

            //Store the string as current section header and clear the key value list
            curSecHeader = line;
            kvlist.clear();
        }
        else
        {
            leftstr = rightstr = "";
            StringHandler::split(line,'=', leftstr, rightstr);
            kvlist[leftstr]=rightstr;
        }
    }
    addSection(curSecHeader,kvlist);
    return true;
}

void Configuration::addSection(String &str, const key_value_list &list)
{
    str.erase(0,1);
    str.erase(str.length()-1,1);
    _ConfigurationMap[str] = list;

}

