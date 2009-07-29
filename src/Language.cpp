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
#include "Language.h"
#include "UI/App.h"

Language::LanguageHeader* Language::LanguageHeader::Instance = NULL;

// NOTE: This table must have same order as LanguageID enum!!!
LanguageInfo lng[] =
{
    { NAME_THELIFE                  , "NAME_THELIFE"                , "TheLife"                                                                     },
    
    // Menu text
    { MSG_MENU_OPT_START            , "MSG_MENU_OPT_START"          , "Begin"                                                                       },
    { MSG_MENU_OPT_OPTIONS          , "MSG_MENU_OPT_OPTIONS"        , "Options"                                                                     },
    { MSG_MENU_OPT_EXIT             , "MSG_MENU_OPT_EXIT"           , "Exit"                                                                        },
    { MSG_MENU_USE                  , "MSG_MENU_USE"                , "Use down or up arrow keys to navigate trought the list."                     },
    { MSG_MENU_DEVEL                , "MSG_MENU_DEVEL"              , "Under development"                                                           },
    
    // CommandHandler output messages
    { CMD_PROCESS                   , "CMD_PROCESS"                 , "Commands start with '/'."                                                    },
    { CMD_CONFIG_LIST               , "CMD_CONFIG_LIST"             , "Configuration [%s] has attribute:\n"                                         },
    { CMD_CONFIG_LIST_ITEM          , "CMD_CONFIG_LIST_ITEM"        , "  * [%s] on point [%d][%d] %s"                                               },
    { CMD_CONFIG_GET                , "CMD_CONFIG_GET"              , "Configuration '%s' has variable[%d]: %s\n"                                   },
    { CMD_TH_POINTER                , "CMD_TH_POINTER"              , "Thread[%d] pointer value is [%p]\n"                                          },
    
    // LOG Load & Unload messages
    { LOG_LOAD_BEGIN                , "LOG_LOAD_BEGIN"              , "*** TheLife Init begin ***"                                                  },
    { LOG_LOAD_CURSES               , "LOG_LOAD_CURSES"             , ">>> [LOAD] Init NCruses"                                                     },
    { LOG_LOAD_COLOR                , "LOG_LOAD_COLOR"              , ">>> [LOAD] Init NCruses Colors"                                              },
    { LOG_LOAD_UI                   , "LOG_LOAD_UI"                 , ">>> [LOAD] Init User Interface"                                              },
    { LOG_LOAD_PLAYER               , "LOG_LOAD_PLAYER"             , ">>> [LOAD] Init Player Unit"                                                 },
    { LOG_LOAD_SIGWINCH             , "LOG_LOAD_SIGWINCH"           , ">>> [LOAD] Init SIGWINCH to SigHandler"                                      },
    { LOG_LOAD_TH_SPAWN             , "LOG_LOAD_TH_SPAWN"           , ">>> [LOAD] Spawning threads"                                                 },
    { LOG_LOAD_TH_ADD               , "LOG_LOAD_TH_ADD"             , ">>> [LOAD] Loading threads pointers to vector"                               },
    { LOG_LOAD_TH_JOIN              , "LOG_LOAD_TH_JOIN"            , ">>> [LOAD] Joinin threads"                                                   },
    { LOG_LOAD_END                  , "LOG_LOAD_END"                , "*** TheLife init end ***"                                                    },
    { LOG_UNLOAD_BEGIN              , "LOG_UNLOAD_BEGIN"            , "*** TheLife Unload begin ***"                                                },
    { LOG_UNLOAD_UI                 , "LOG_UNLOAD_UI"               , ">>> [UNLOAD] Unloading Interface"                                            },
    { LOG_UNLOAD_PLAYER             , "LOG_UNLOAD_PLAYER"           , ">>> [UNLOAD] Saving & Unloading player"                                      },
    { LOG_UNLOAD_INSTANCE           , "LOG_UNLOAD_INSTANCE"         , ">>> [UNLOAD] Deleting instances"                                             },
    { LOG_UNLOAD_END                , "LOG_UNLOAD_END"              , "*** TheLife Unload end ***"                                                  },
    { LOG_TH_RUN                    , "LOG_TH_RUN"                  , ">>> TheLife called *Thread::Run(). Thread called succefully."                },
    { LOG_TH_STATUS                 , "LOG_TH_STATUS"               , ">>> TheLife called *Thread::Status(). Thread called succefully."             },
    
    // Error LOG messages
    { ERROR_WRONG_CMD               , "ERROR_WRONG_CMD"             , "Command were not found or wrong. Your command was '%s'!\n"                   },
    { ERROR_WRONG_CMD_CONFIG        , "ERROR_WRONG_CMD_CONFIG"      , "You requested configuration '%s'. This configuration were not found!"        },
    { ERROR_HIDE_PANEL              , "ERROR_HIDE_PANEL"            , " * Error occured while hiding panel."
                                                                      " Your panel was [%d]. Maximum panels exist [%d][%d].\n"                      },
    { ERROR_CONFIG_LOAD_INV_VAR     , "ERROR_CONFIG_LOAD_INV_VAR"   , "Invalid Variables for '%s'! Configuration not loaded...\n"                   },
    { ERROR_CONFIG_LOAD_INV_CONF    , "ERROR_CONFIG_LOAD_INV_CONF"  , "Invalid configuration '%s'! Missing '='! Configuration not loaded...\n"      },
    { ERROR_PLAYER_WRONG_DHEAD      , "ERROR_PLAYER_WRONG_DHEAD"    , "Player Data has wrong header. Header of Player Data is [%d]\n"               },
    { ERROR_PLAYER_WRONG_DVER       , "ERROR_PLAYER_WRONG_DVER"     , "Player Data has wrong data version. Data version of Player Data is [%d]\n"   },
    { ERROR_LOG_WRONG_TYPE          , "ERROR_LOG_WRONG_TYPE"        , "YOU HAVE AN INVALID LOG_TYPE!\n"                                             },
    { ERROR_LNG_INVALID_ATTR        , "ERROR_LNG_INVALID_ATTR"      , "'%s' is invalid language attribute! Using default english!\n"                },
    { ERROR_LNG_INVALID_STR_ATTR    , "ERROR_LNG_INVALID_STR_ATTR"  , "Invalid string for '%s' language attribute! Using default english!\n"        },
    { ERROR_LNG_MISSING_STR_ATTR    , "ERROR_LNG_MISSING_STR_ATTR"  , "Missing '=' for '%s' language attribute! Using default english!\n"           },
    { ERROR_TH_STATUS_TIMER_CREATE  , "ERROR_TH_STATUS_TIMER_CREATE"   , "TIMER_CREATE RETURNED ERROR: %d\n"                                        },
    { ERROR_TH_STATUS_TIMER_SET     , "ERROR_TH_STATUS_TIMER_SET"      , "TIMER_SETTIME RETURNED ERROR: %d\n"                                       },
    
    // Debug LOG messages
    { DEBUG_LOOKUP_LOOP             , "DEBUG_LOOKUP_LOOP"           , "Loop in [%u] table. Subfunction being looped %d\n"                           },
    { DEBUG_CALL_EXIT               , "DEBUG_CALL_EXIT"             , "Exit function was called. Exiting life!\n"                                   },
    { DEBUG_KEY_HANDLE              , "DEBUG_KEY_HANDLE"            , "KeyHandle triggered with key [%d]\n"                                         },
    { DEBUG_LNG_LOAD_ATTR           , "DEBUG_LNG_LOAD_ATTR"         , "** Language loaded attr[%d] [%s] with info [%s] **\n"                        },
    { DEBUG_NEWLINE_FOUND           , "DEBUG_NEWLINE_FOUND"         , "Newline for line [%s] found in pos [%d]\n"                                   },
    
    { END_LANGUAGE                  , NULL                          , "NULL"                                                                        }
};

Language::LanguageHeader* Language::LanguageHeader::getInstance()
{
    if(Instance == NULL)  {

        Instance = new LanguageHeader();
    }

    return Instance;
}

std::string Language::LanguageHeader::ReadHeadInfo(std::string look, int pos)  {

    std::size_t found = -1;

    for(int i = 0; i < HeaderDataSize(); ++i)  {

        if(ReturnHeader(i).find(look) != -1) {
            found = i;
            break;
        }
    }

    return (found < -1) ? ((pos > 0) ? ReturnHeader(found, pos) : "Cannot be in pos 0\n") : std::string();
}

int Language::LanguageHeader::HeaderLine(std::string look) {

    std::size_t found = -1;

    for(int i = 0; i < HeaderDataSize(); ++i)  {

        if(ReturnHeader(i).find(look) != -1) {
            found = i;
            break;
        }
    }

    return found;
}

void Language::LanguageHeader::LoadHeader() {

    std::string dir = "./language/";
    std::vector<std::string> list = TheLife::DirExtension(dir, ".lng");
    std::string exp = "%";

    for(int i = 0; i < list.size(); ++i) {
        std::string temp = dir + list[i];
        std::fstream file (temp.c_str(), std::ios::in);

        if(!file)
        {
            TheLife::Error("Error loading %s file", list[i].c_str());
            return;
        }

        std::string line;
        
        while(getline(file, line))
        {
            if(line.empty())
                continue;
            else if(line.at(0) == exp.at(0))
            {
                std::vector<std::string> info = TheLife::Explode(" ", line);
                TheLife::Debug("Loading %s from file %s\n", info[1].c_str(), list[i].c_str());
                this->data.push_back(std::vector<std::string>());
                data[i].push_back(info[1]);
            }
        }
        
    }
}

void Language::Load(std::string file) {
    
    setlocale(LC_ALL, "");
    
    // Open language file
    std::fstream LNG (file.c_str(), std::ios::in);

    // If we didn't find any file return
    if(!LNG)
    {
        return;
    }

    std::string c;
    std::string co = "#=\"%";
    
    // Check each name attribute of language file
    while(getline(LNG, c))
    {
        // Language file can be commented with #
        if(c.empty() || TheLife::CharCheck("#", c.c_str()) || TheLife::CharCheck("%", c.c_str()))
            continue;
        else
        {
            std::vector<std::string> vars;
            int quotes = 0;
            int vars_size = 0;

            vars = TheLife::Explode(" ", c);

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

            // If language line has "=" it's usable
            if(vars[1].at(0) == co.at(1)) {
                if(quotes == 2) {
                    
                    std::string line;
                    int entry = -1;

                    for(int j = 0; j < vars_size; ++j) {
  
                        if(vars[j].at(0) == co.at(1))
                            continue;
                        else {
                            for(int x = 0; x < vars[j].size(); x++) {
                                if(vars[j].at(x) == co.at(2)) {
                                    vars[j] = vars[j].erase(x,1);
                                }
                            }
                            for(int x = 2; x < vars_size; ++x) {
                                
                                if(x == 2)
                                    line = vars[x];
                                else {
                                    line += " ";
                                    line += vars[x];
                                }
                            }
                        }
                    }
                    
                    for(int i = 0; lng[i].name != NULL; ++i) {
                        if(!strcmp(lng[i].name,vars[0].c_str()))
                            entry = i;                                
                    }
                    
                    if(entry > -1) {
                        
                        int newline = -1;
                        std::string n("\\n");
                        std::string temp = line;
                        
                        newline = line.rfind(n);
                        if(newline > -1) {
                            temp.replace(newline,n.length(),"\n");
                            lng[entry].str.assign(temp);
                            TheLife::Debug(Get(DEBUG_NEWLINE_FOUND).c_str(), line.c_str(), newline);
                        }
                        else {
                            lng[entry].str.assign(line);
                        }
                        TheLife::Debug(Get(DEBUG_LNG_LOAD_ATTR).c_str(), entry, vars[0].c_str(), line.c_str());
                    }
                    else
                        TheLife::Error(Get(ERROR_LNG_INVALID_ATTR).c_str(), vars[0].c_str());
                }
                else {
                  TheLife::Error(Get(ERROR_LNG_INVALID_STR_ATTR).c_str(), vars[0].c_str());
                }
            }
            else {
              TheLife::Error(Get(ERROR_LNG_MISSING_STR_ATTR).c_str(), vars[0].c_str());
            }
        }
    }

    // Close language file
    LNG.close();
};

std::string Language::Get(LanguageID entry) {

    for(int i = 0; i != END_LANGUAGE; ++i)
        if(i == entry)
            return lng[i].str;
};
