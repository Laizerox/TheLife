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

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>
#include <vector>
#include <map>

#include "Singleton.h"

#define sLanguage Language::getInstance()

class Language : public Singleton<Language> {

    private:
        /* HeaderData Design
         0: Filename                    | Loaded after vector init
         1: Language name               | This is defined in .lng file
         2: Localization                | This is defined in .lng file
         3: Author                      | This is defined in .lng file
         4: Version                     | This is defined in .lng file
        */
        std::vector<std::vector <std::string> > HeaderData;

        /* LanguageStore Design
         first: Language string "constant"
         second: Language data map ->
            | std::map->first = Languages translation "constant"
            | std::map->second = Translation
         */            
        std::map<std::string, std::map<std::string, std::string> > LanguageStore;
        bool LoadFile(std::string file, std::map<std::string, std::string>& data);

    public:
        Language() {}
        ~Language() {}
            
        void LoadHeader();

        // Header return part
        // TODO: Need's improvement!
        std::string ReturnHeader(int id, int slot = 0)  { return HeaderData[id][slot]; }
        std::string ReadHeadInfo(std::string look, int pos = 1);
        int HeaderDataSize(int id = -1) { return (id > -1) ? HeaderData[id].size() : HeaderData.size(); }
        int HeaderLine(std::string look);

        // Language return part!
        std::string Get(std::string constant);
};

#endif
