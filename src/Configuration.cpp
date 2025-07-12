#include "Configuration.hpp"

Configuration::Configuration()
{
    configuration_filename = "";
}

Configuration::~Configuration()
{
}

void Configuration::setFileName(std::string fname)
{
    configuration_filename = fname;
}

void Configuration::setDirectory(std::string dname)
{
    configuration_directory = dname;
}

bool Configuration::isSection(const std::string &section)
{
    map<std::string, std::map<std::string, std::string>>::const_iterator confmapiter;

    confmapiter = configuration_map.find(section);
    if(confmapiter == configuration_map.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::string Configuration::getValue(const std::string &section, const std::string &settingKey, const std::string defval)
{
    map<std::string, std::map<std::string, std::string>>::const_iterator confmapiter;
    std::map<std::string, std::string>::const_iterator kviter;
    std::string str;

    confmapiter = configuration_map.find(section);
    if(confmapiter == configuration_map.end())
    {
        return defval;
    }
    else
    {
        std::map<std::string, std::string> list = confmapiter->second;
        kviter = list.find(settingKey);

        if(kviter == list.end())
        {
            return defval;
        }
        str = (std::string)kviter->second;
    }
    return str;
}

bool Configuration::loadCustomConfiguration(const std::string &configFile)
{
    return loadConfiguration(configFile);
}

bool Configuration::loadConfiguration()
{
    char filepathbuffer[2048];
    memset((char*)&filepathbuffer[0],0,sizeof(filepathbuffer));

    strcat(filepathbuffer, configuration_directory.c_str());
    strcat(filepathbuffer, "/");
    strcat(filepathbuffer, configuration_filename.c_str());

    if(!loadConfiguration(filepathbuffer))
    {
        return false;
    }

    return true;
}

bool Configuration::loadConfiguration(const std::string &configFile)
{
    std::ifstream cfgfile(configFile.c_str());
    std::string line, leftstr, rightstr;
    std::vector<std::string> linelist;

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
    std::string curSecHeader = "";
    std::map<std::string, std::string> kvlist;

    for(std::vector<std::string>::size_type i = 0; i != linelist.size(); i++)
    {
        line = linelist[i];
        //Section header line
        if(line[0]=='[' && line[line.length()-1]==']')
        {
            //Check whether this is the first instance of a section header
            if(configuration_map.size()<1)
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

void Configuration::addSection(std::string &str, const std::map<std::string, std::string> &list)
{
    str.erase(0,1);
    str.erase(str.length()-1,1);
    configuration_map[str] = list;

}

