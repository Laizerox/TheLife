/* 
 * Copyright (C) 2008 The Life
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * 
 */

#include "Share.h"
#include "config.h"
#include "Language.h"
#include "UI/App.h"

config* config::Instance = NULL;

config* config::getInstance()
{
    if(Instance == NULL)  {

        Instance = new config();
    }

    return Instance;
}

// config::GetConf(std::string); read Conf data from string data vector in class config
std::string config::GetVariable(std::string look, int pos)  {

    std::size_t found = -1;

    // Check that config exist
    for(int i = 0; i < GetDataSize(); ++i)  {

        if(GetData(i).find(look) != -1) {
            found = i;
            break;
        }
    }

    return (found < -1) ? ((pos > 0) ? GetData(found, pos) : "Cannot be in pos 0\n") : std::string();
}

int config::GetLine(std::string look) {

    std::size_t found = -1;

    // Check that config exist
    for(int i = 0; i < GetDataSize(); ++i)  {

        if(GetData(i).find(look) != -1) {
            found = i;
            break;
        }
    }

    return found;
}

// config::LoadConf(std::string) loads data from file to string data vector
void config::LoadConf(std::string file, config *conf)  {

    // Open file
    std::fstream CONF (file.c_str(), std::ios::in);

    // If we didn't find any file return
    if(!CONF)
    {
        CreateDefaultConfig(file, conf);
        return;
    }

    std::string c;
    std::string co = "#=\"";
    int i = 0;

    // Loading happens here
    while(getline(CONF, c))
    {
        // If config first char is comment char "#" then just skip it, if data is correct then put into vector
        if(c.at(0) == co.at(0))
            continue;
        else
        {
            std::vector<std::string> vars;
            int quotes = 0;
            int vars_size = 0;

            vars = TheLife::SeparateString(c);

            for(int j = 0; j < vars.size(); ++j) {
                for(int x = 0; x < vars[j].size(); ++x) {
                    if(vars[j].at(x) == co.at(2)) {
                        ++quotes;
                    }
                }

                if(quotes == 2) {
                    vars_size = j+1;
                    break;
                }
            }

            // If word == '='
            if(vars[1].at(0) == co.at(1)) {
                if(quotes == 2) {
                    this->data.push_back(std::vector<std::string>());

                    // For each 'vars'
                    for(int j = 0; j < vars_size; ++j) {
  
                        // If var == '=', skip, else insert into 'data'
                        if(vars[j].at(0) == co.at(1))
                            continue;
                        else {
                            for(int x = 0; x < vars[j].size(); x++) {
                                if(vars[j].at(x) == co.at(2)) {
                                    vars[j] = vars[j].erase(x,1);
                                }
                            }
                            this->data[i].push_back(vars[j]);
                        }
                    }
                    i++;
                }
                else {
                    TheLife::Error(Language::Get(ERROR_CONFIG_LOAD_INV_VAR).c_str(), vars[0].c_str());
                }
            }
            else {
                TheLife::Error(Language::Get(ERROR_CONFIG_LOAD_INV_CONF).c_str(), vars[0].c_str());
            }
        }
    }

    // WE MUST ALWAYS CLOSE THE FILE!
    CONF.close();
}

void config::EditConfig(std::string file, std::string name) {

    // Open file
    std::fstream CONF (file.c_str(), std::ios::in);

    // If we didn't find any file return
    if(!CONF)
        return;

    std::string c;
    std::string co = "#";

    // Loading happens here
    while(getline(CONF, c))
    {
        // If config first char is comment char "#" then just skip it, if data is correct then put into vector
        if(c.at(0) == co.at(0))
            continue;
        else
        {
            if(c.find(name) != -1) {
                TheLife::Error("Conf %s position %d", name.c_str(), (int)CONF.tellp());
            }
        }
    }

    // WE MUST ALWAYS CLOSE THE FILE!
    CONF.close();
}

void config::CreateDefaultConfig(std::string file, config *conf) {

    std::string confS = ("# This configuration autogenerated!\n"
                        "# Conf file version: 1.0\n"
                        "version = \"0.1ALPHA\"\n"
                        "logpath = \"logs\"\n"
                        "logname = \"Log.lifelog\"\n"
                        "errorfile = \"Error.lifelog\"\n"
                        "debugfile = \"Debug.lifelog\"\n"
                        "ldpath = \"data\"\n"
                        "playerfile = \"player.ld\"\n");

    std::fstream CONF (file.c_str(), std::ios::out);

    CONF.write(confS.c_str(), confS.size());
    CONF.close();
    LoadConf(file, conf);
}

// config::WriteToConfig(std::string, std::string, config) adds a setting to the end of the config-file
bool config::WriteToConfig(std::string file, std::string variable) {

    if (variable.empty() || !GetVariable(variable).empty())
      return false;

    // If no file is given, revert to default
    if (file.empty())
        file = "data/life.config";
	
    std::vector<std::string> vars;
    vars = TheLife::SeparateString(variable);

    std::string confLine;

    int lineNum = this->GetDataSize();

    if(vars[0].at(0) == '#')
        confLine = "\n" + variable;
    else
    {

        // If there is less than a setting and a value, return false
        if (vars.size() < 2)
          return false;

        this->data.push_back(std::vector<std::string>());

        this->data[lineNum].push_back(std::string(vars[0]));

        confLine = "\n" + vars[0];
        confLine += " =";

        // For each 'vars'
        for(int j = 1; j < vars.size(); ++j) {

          // Insert into 'data'
          this->data[lineNum].push_back(std::string(vars[j]));
  
          confLine += " " + vars[j];
        }
    }

    // Open conf file at the end of the file
    std::fstream CONF (file.c_str(), std::ios::app | std::ios::out);

    // If we didn't find any file return
    if(!CONF)
        return false;

    CONF.write(confLine.c_str(), confLine.size());
    CONF.close();
    
    return true;
}
