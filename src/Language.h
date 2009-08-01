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

#include "Singleton.h"

#define sLangHeader Language::LanguageHeader::getInstance()

// NOTE: This enum must have same order as Language table!!!
enum LanguageID {

    //default
    NAME_THELIFE,
    
    //Menu
    MSG_MENU_OPT_START,
    MSG_MENU_OPT_OPTIONS,
    MSG_MENU_OPT_EXIT,
    MSG_MENU_USE,
    MSG_MENU_DEVEL,
    
    // CommandHandler output
    CMD_PROCESS,
    CMD_CONFIG_LIST,
    CMD_CONFIG_LIST_ITEM,
    CMD_CONFIG_GET,
    CMD_TH_POINTER,
    
    // Load & Unload
    LOG_LOAD_BEGIN,
    LOG_LOAD_CURSES,
    LOG_LOAD_COLOR,
    LOG_LOAD_UI,
    LOG_LOAD_PLAYER,
    LOG_LOAD_SIGWINCH,
    LOG_LOAD_TH_SPAWN,
    LOG_LOAD_TH_ADD,
    LOG_LOAD_TH_JOIN,
    LOG_LOAD_END,
    LOG_UNLOAD_BEGIN,
    LOG_UNLOAD_UI,
    LOG_UNLOAD_PLAYER,
    LOG_UNLOAD_INSTANCE,
    LOG_UNLOAD_END,
    LOG_TH_RUN,
    LOG_TH_STATUS,
    
    // Error
    ERROR_WRONG_CMD,
    ERROR_WRONG_CMD_CONFIG,
    ERROR_HIDE_PANEL,
    ERROR_CONFIG_LOAD_INV_VAR,
    ERROR_CONFIG_LOAD_INV_CONF,
    ERROR_PLAYER_WRONG_DHEAD,
    ERROR_PLAYER_WRONG_DVER,
    ERROR_LOG_WRONG_TYPE,
    ERROR_LNG_INVALID_ATTR,
    ERROR_LNG_INVALID_STR_ATTR,
    ERROR_LNG_MISSING_STR_ATTR,
    ERROR_TH_STATUS_TIMER_CREATE,
    ERROR_TH_STATUS_TIMER_SET,
    
    // Debug
    DEBUG_LOOKUP_LOOP,
    DEBUG_CALL_EXIT,
    DEBUG_KEY_HANDLE,
    DEBUG_LNG_LOAD_ATTR,
    DEBUG_NEWLINE_FOUND,
        
    // TODO: ???
    END_LANGUAGE
};

struct LanguageInfo {

    LanguageID entry;
    const char *name;
    std::string str;
};

namespace Language {

    void Load(std::string life);
	std::string Get(LanguageID entry);
	
	class LanguageHeader : public Singleton<LanguageHeader> {

        private:
            /* LanguageHeader Data Design
             0: Filename                    | Loaded after vector init
             1: Language name               | This is defined in .lng file
             2: Localization                | This is defined in .lng file
             3: Author                      | This is defined in .lng file
             4: Version                     | This is defined in .lng file
            */
            std::vector<std::vector <std::string> > data;

        public:
            LanguageHeader() {}
            
            void LoadHeader();
            
            virtual std::string ReturnHeader(int i, int j = 0)  { return data[i][j]; }
            virtual std::string ReadHeadInfo(std::string look, int pos = 1);
            virtual int HeaderDataSize(int slot = -1) { return (slot > -1) ? data[slot].size() : data.size(); }
            virtual int HeaderLine(std::string look);            
	};
};

#endif
