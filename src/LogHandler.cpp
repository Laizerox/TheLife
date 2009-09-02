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
#include <string>

#include "config.h"
#include "LogHandler.h"
#include "Language.h"
#include "UI/App.h"

void LogHandler::WriteLog(std::string text) {

    std::string whitespace("\n");
    size_t found;

    std::string path = sConfig->GetVariable("logpath");
    std::string name = sConfig->GetVariable("logname");
    std::string logtime;

    if(path.empty() | name.empty()) {
        path = ".";
        name = "Log.lifelog";
    }

    std::string file = path;
    file += "/";
    file += name;

    time_t rawtime;
    time ( &rawtime );

    logtime += "[";
    logtime += ctime(&rawtime);
    found = logtime.find_last_not_of(whitespace);
    if(found != std::string::npos)
      logtime.erase(found+1);
    else
      logtime.clear();
    logtime += "] ";

    std::fstream LOG (file.c_str(), std::ios::in | std::ios::out);

    if(!LOG)
    {
        NewLog(file, ctime(&rawtime), text, LOG_FILE);
        return;
    }

    LOG.seekp(0, std::ios_base::end);
    LOG << logtime << text << "\n";

    LOG.close();
}

void LogHandler::WriteDebug(std::string text) {

    std::string whitespace("\n");
    size_t found;

    std::string path = sConfig->GetVariable("logpath");
    std::string name = sConfig->GetVariable("debugfile");
    std::string logtime;

    if(path.empty() | name.empty()) {
        path = ".";
        name = "Debug.lifelog";
    }

    std::string file = path;
    file += "/";
    file += name;

    time_t rawtime;
    time ( &rawtime );

    logtime += "[";
    logtime += ctime(&rawtime);
    found = logtime.find_last_not_of(whitespace);
    if(found != std::string::npos)
      logtime.erase(found+1);
    else
      logtime.clear();
    logtime += "] ";

    std::fstream LOG (file.c_str(), std::ios::in | std::ios::out);

    if(!LOG)
    {
        NewLog(file, ctime(&rawtime), text, LOG_DEBUG);
        return;
    }

    LOG.seekp(0, std::ios_base::end);
    LOG << logtime << text;

    LOG.close();
}

void LogHandler::WriteError(std::string text) {

    std::string whitespace("\n");
    size_t found;

    std::string path = sConfig->GetVariable("logpath");
    std::string name = sConfig->GetVariable("errorfile");
    std::string logtime;

    if(path.empty() | name.empty()) {
        path = ".";
        name = "Error.lifelog";
    }

    std::string file = path;
    file += "/";
    file += name;

    time_t rawtime;
    time ( &rawtime );

    logtime += "[";
    logtime += ctime(&rawtime);
    found = logtime.find_last_not_of(whitespace);
    if(found != std::string::npos)
      logtime.erase(found+1);
    else
      logtime.clear();
    logtime += "] ";

    std::fstream LOG (file.c_str(), std::ios::in | std::ios::out);

    if(!LOG)
    {
        NewLog(file, ctime(&rawtime), text, LOG_ERROR);
        return;
    }

    LOG.seekp(0, std::ios_base::end);
    LOG << logtime << text;

    LOG.close();
}

void LogHandler::NewLog(std::string file, std::string time, std::string text, LOG_TYPE log_type) {

    size_t found;
    std::string whitespace("\n");
    std::fstream LOG (file.c_str(), std::ios::out);

    std::string newlog("*** LOG CREATED ON ");
    newlog += time;

    found = newlog.find_last_not_of(whitespace);
    if(found != std::string::npos)
      newlog.erase(found+1);
    else
      newlog.clear();

    newlog += " ***\n";

    LOG.write(newlog.c_str(), newlog.size());
    LOG.close();
    
    switch(log_type) {

        case LOG_NONE: TheLife::Error(Language::Get(ERROR_LOG_WRONG_TYPE)); break;
        case LOG_FILE: WriteLog(text); break;
        case LOG_DEBUG: WriteDebug(text); break;
        case LOG_ERROR: WriteError(text); break;
    }
}
