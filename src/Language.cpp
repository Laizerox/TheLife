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

#include <fstream>
#include <cstring>
#include <iostream>
#include <cstdlib>

#include "Language.h"
#include "UI/App.h"
std::string Language::ReadHeadInfo(std::string look, int pos)  {

    std::size_t found = -1;

    for(int i = 0; i < HeaderDataSize(); ++i)  {

        if(ReturnHeader(i).find(look) != -1) {
            found = i;
            break;
        }
    }

    return (found < -1) ? ((pos > 0) ? ReturnHeader(found, pos) : "Cannot be in pos 0\n") : std::string();
}

int Language::HeaderLine(std::string look) {

    std::size_t found = -1;

    for(int i = 0; i < HeaderDataSize(); ++i)  {

        if(ReturnHeader(i).find(look) != -1) {
            found = i;
            break;
        }
    }

    return found;
}

void Language::LoadHeader() {

    std::string dir = "./language/";
    std::vector<std::string> list = TheLife::DirExtension(dir, ".lng");

    if(list.empty()) {
        TheLife::Error("There was no language files found in folder %s. Aborting TheLife.", dir.c_str());
        exit(1);
    }

    for(int i = 0; i < list.size(); ++i) {
        std::string temp = dir + list[i];
        std::fstream file (temp.c_str(), std::ios::in);

        if(!file) {
            TheLife::Error("File %s cannot be loaded or not found. Aborting TheLife!", list[i].c_str());
            exit(1);
        }
        
        std::string line;
        std::string toCheck;
        std::map<std::string, std::string> data;
        this->HeaderData.push_back(std::vector<std::string>());
        HeaderData[i].push_back(list[i]); // Language file as whole for "identifier"
        
        while(getline(file, line))
        {
            if(line.empty())
                continue;          

            toCheck = line.at(0);
            
            if(TheLife::CharCheck("%", toCheck))
            {
                std::vector<std::string> info = TheLife::Explode(" ", line);
                TheLife::CharStr("%", info[0]);
                TheLife::Debug("Loading %s %s from file %s\n", info[0].c_str(), info[1].c_str(), list[i].c_str());
                this->HeaderData[i].push_back(info[1]);
            }
        }

        if(!LoadFile(temp, data)) {
            TheLife::Error("Failed to load language data for file %s!\n", list[i].c_str());
            continue;
        }

        this->LanguageStore.insert(std::pair<std::string, std::map<std::string, std::string> >(ReturnHeader(i, 1), data));
    }

    if(this->LanguageStore.empty()) {
        TheLife::Error("No language data found at all... Aborting TheLife!\n");
        exit(1);
    }
}

bool Language::LoadFile(std::string file, std::map<std::string, std::string>& data) {

    setlocale(LC_ALL, "");
    
    // Open language file
    std::fstream lngFile (file.c_str(), std::ios::in);
    std::string c;
    std::string toCheck;

    // If we didn't find any file return
    if(!lngFile) {
        TheLife::Error("File %s cannot be loaded or not found. Aborting TheLife!", file.c_str());
        exit(1);
    }
    
    // Check each name attribute of language file
    while(getline(lngFile, c))
    {
        // Language file can be commented with #
        if(c.empty())
            continue;

        toCheck = c.at(0);

        if(TheLife::CharCheck("#", toCheck) || TheLife::CharCheck("%", toCheck))
            continue;
        
        std::vector<std::string> vars = TheLife::Explode(" ", c, 2, "\"");

        if(vars.empty())
            return false;
        
        // If language line has "=" it's usable
        if(!TheLife::CharCheck("=", vars[1])) {
            TheLife::Error("Invalid constant %s. Missing or misplaced '='. Aborting load...", vars[0].c_str());
            return false;
        }
                    
        for(int i = 0; i < vars.size(); ++i)
            TheLife::CharStr("\"", vars[i]);
        
        std::string line = TheLife::Implode(" ", vars, 2);

        data.insert(std::pair<std::string, std::string>(vars[0], line));
    }

    // Close language file
    lngFile.close();

    return true;
}

std::string Language::Get(std::string constant) {

    std::map<std::string, std::string> info = LanguageStore.find(sTheLife->ActiveLanguage())->second;
    return info.find(constant)->second;
}