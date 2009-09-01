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

#include <iostream>
#include <csignal>
#include <cstring>
#include <dirent.h>

#include "../SignalDef.h"
#include "../CommandHandler.h"
#include "../LogHandler.h"
#include "../FileHandler.h"
#include "../Thread.h"
#include "../Language.h"
#include "App.h"
#include "Interface.h"

void TheLife::LoadLife() {

    
    struct timespec start;
    struct timespec end;
    struct timespec diff;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    LogHandler::WriteLog(Language::Get(LOG_LOAD_BEGIN));
    
    pthread_t main, status;

    //Life Initialization
    sUI->lwin[0].buf[0] = 0x00;
    sUI->lwin[1].buf[0] = 0x00;
    
    // Init NCurses
    LogHandler::WriteLog(Language::Get(LOG_LOAD_CURSES));
    initscr();
    start_color();
    use_default_colors();
    //noecho();
    keypad( stdscr, TRUE );

    // Init colors
    LogHandler::WriteLog(Language::Get(LOG_LOAD_COLOR));
    init_pair(1, 1, -1);
    init_pair(2, 2, -1);
    init_pair(3, 3, -1);
    init_pair(4, 4, -1);

    // Init UI
    LogHandler::WriteLog(Language::Get(LOG_LOAD_UI));
    Interface::Load();
    
    sUI->SetHidden(FALSE, WINDOW_CONSOLE);
    sUI->SetHidden(FALSE, WINDOW_STATUS);    
    
    // Init Player
    LogHandler::WriteLog(Language::Get(LOG_LOAD_PLAYER));
    FileHandler::PlayerInit();

    // Init SIGWICH (Console change signal) to SigHandler
    LogHandler::WriteLog(Language::Get(LOG_LOAD_SIGWINCH));
    signal(SIGWINCH, TheLife::SigHandler);
    
    // Thread spawning
    LogHandler::WriteLog(Language::Get(LOG_LOAD_TH_SPAWN));
    pthread_create(&status, NULL, Thread::Status, NULL);
    pthread_create(&main, NULL, Thread::Run, NULL);
    LogHandler::WriteLog(Language::Get(LOG_LOAD_TH_ADD));
    sThread->AddThread(status);
    sThread->AddThread(main);
    LogHandler::WriteLog(Language::Get(LOG_LOAD_TH_JOIN));
    pthread_join(status,NULL);
    pthread_join(main, NULL);    
    
    LogHandler::WriteLog(Language::Get(LOG_LOAD_END));
    clock_gettime(CLOCK_MONOTONIC, &end);
    diff = diff_timespec(start, end);
    Debug("Load took %lld milliseconds\n", millisec_elapsed(diff));
}

void TheLife::UnloadLife() {
    
    LogHandler::WriteLog(Language::Get(LOG_UNLOAD_BEGIN));
    
    // Save & Unload Player
    LogHandler::WriteLog(Language::Get(LOG_UNLOAD_PLAYER));
    FileHandler::PlayerUnload();
    
    // Unload interface
    LogHandler::WriteLog(Language::Get(LOG_UNLOAD_UI));
    Interface::Unload();
    
    // Delete interface & thread instances
    LogHandler::WriteLog(Language::Get(LOG_UNLOAD_INSTANCE));
    TheLife::Application::destroy();
    Thread::cThread::destroy();
    //config::destroy();
    Interface::UI::destroy();
    Interface::Menu::destroy();
    FileHandler::LD_FILE::destroy();
    Language::LanguageHeader::destroy();

    LogHandler::WriteLog(Language::Get(LOG_UNLOAD_END));
    std::cout << "TheLife exit succefully!\n";
}

int TheLife::KeyHandle(int key, int keycolor) {

    int width, height, i;
    CommandHandler *Command;
    
    width = sUI->GetX(WINDOW_CONSOLE) - 1;
    height = 1;

    Debug(Language::Get(DEBUG_KEY_HANDLE).c_str(), key);

    switch (key) {
        case KEY_LEFT:
            if (sUI->lwin[WINDOW_CONSOLE].field_ptr > 0)
                sUI->lwin[WINDOW_CONSOLE].field_ptr--;
            break;
        case KEY_RIGHT:
            if (sUI->lwin[WINDOW_CONSOLE].field_ptr < sUI->lwin[WINDOW_CONSOLE].field_length)
                sUI->lwin[WINDOW_CONSOLE].field_ptr++;
            break;
        case KEY_BACKSPACE:
        case 127:
            if (sUI->lwin[WINDOW_CONSOLE].field_ptr > 0) {
                sUI->lwin[WINDOW_CONSOLE].field_ptr--;
                sUI->lwin[WINDOW_CONSOLE].field_length--;
                for (i = sUI->lwin[WINDOW_CONSOLE].field_ptr; i < sUI->lwin[WINDOW_CONSOLE].field_length; i++)
                    sUI->lwin[WINDOW_CONSOLE].field_buf[i] = sUI->lwin[WINDOW_CONSOLE].field_buf[i + 1];
                sUI->lwin[WINDOW_CONSOLE].field_buf[i] = 0x00;
            }
            break;
        case '\n':
        case '\r':
            if (sUI->lwin[WINDOW_CONSOLE].field_buf[0] == 0x00)
               break;

            wattron(sUI->GetConsole(WINDOW_CONSOLE, 0), COLOR_PAIR(keycolor));
            Command->CommandProcess(sUI->lwin[WINDOW_CONSOLE].field_buf);
            wattroff(sUI->GetConsole(WINDOW_CONSOLE, 0), COLOR_PAIR(keycolor));

            sUI->lwin[WINDOW_CONSOLE].field_buf[0] = 0x00;
            sUI->lwin[WINDOW_CONSOLE].field_ptr = 0x00;
            sUI->lwin[WINDOW_CONSOLE].field_length = 0x00;
            werase(sUI->GetConsole(WINDOW_CONSOLE, 1));
            break;
        case -1:
        case KEY_RESIZE:
        case KEY_UP:
        case KEY_DOWN:
            return -1;
            break;
        default:
            if (sUI->lwin[WINDOW_CONSOLE].field_length >= FIELDBUFSIZE - 1)
                return -1;

            for (i = sUI->lwin[WINDOW_CONSOLE].field_length; i > sUI->lwin[WINDOW_CONSOLE].field_ptr && i > 0; i--)
                sUI->lwin[WINDOW_CONSOLE].field_buf[i] = sUI->lwin[WINDOW_CONSOLE].field_buf[i - 1];

            sUI->lwin[WINDOW_CONSOLE].field_buf[sUI->lwin[WINDOW_CONSOLE].field_ptr] = key;
            sUI->lwin[WINDOW_CONSOLE].field_buf[sUI->lwin[WINDOW_CONSOLE].field_length + 1] = 0x00;
            sUI->lwin[WINDOW_CONSOLE].field_ptr++;
            sUI->lwin[WINDOW_CONSOLE].field_length++;
            break;
        }
        Interface::ConsoleBuffer(sUI->GetConsole(WINDOW_CONSOLE, 1), sUI->lwin[WINDOW_CONSOLE].field_buf, height, width, sUI->lwin[WINDOW_CONSOLE].field_ptr);
        wrefresh(sUI->GetConsole(WINDOW_CONSOLE, 1));
        return 0;
}

void TheLife::Debug(std::string debug, ...) {

    bool DEBUG = FALSE;
  
    time_t rawtime;
    time ( &rawtime );
    std::string times = ctime(&rawtime);
    std::size_t found = times.find_last_not_of("\n");
    if(found != std::string::npos)
      times.erase(found+1);
    else
      times.clear();

    char buffer[256];
    va_list arg;
    va_start(arg, debug.c_str());
    vsprintf(buffer, debug.c_str(), arg);
    va_end(arg);

    debug = buffer;

    if(DEBUG) {
      wattron(sUI->GetConsole(WINDOW_CONSOLE, 0), COLOR_PAIR(4));
        Interface::ConsoleOutput("Debug[TIME:%s]: %s\n", times.c_str(), debug.c_str());
      wattroff(sUI->GetConsole(WINDOW_CONSOLE, 0), COLOR_PAIR(4));
    }
  
    LogHandler::WriteDebug(debug);
}

void TheLife::Error(std::string error, ...) {

    char buffer[256];
    va_list arg;
    va_start(arg, error.c_str());
    vsprintf(buffer, error.c_str(), arg);
    va_end(arg);
    
    error = buffer;

    /*
    wattron(sUI->GetConsole(WINDOW_CONSOLE, 0), COLOR_PAIR(1));
    Interface::ConsoleOutput("Error: %s\n", error.c_str());
    wattroff(sUI->GetConsole(WINDOW_CONSOLE, 0), COLOR_PAIR(1)); */

    LogHandler::WriteError(error);
}

std::vector<std::string> TheLife::Explode(std::string delimiter, std::string str) {

    std::vector<std::string> vars;
    char tmp;
    int y = 0;
    
    for(std::string::size_type a = 0; a < str.size(); ++a) {

        std::string word;
        
        while(a < str.size()) {
            tmp = str.at(a);
    
            if(CharCheck(delimiter, (const char *)&tmp))
                y = a;

            if(!CharCheck(delimiter, (const char *)&tmp))
                word.push_back(tmp);

            if(y >= a && a != 0 || str.size() == a+1) {
                vars.push_back(word);
                word.clear();
            }
            ++a;
        }
    }
    return vars;
}

void TheLife::SigHandler(int sig) {

    if(sig == SIGWINCH) {
        Interface::Resize();
    }
    else if(sig == SIG_STATUS) {
        sUI->StatusUpdate();
    }
}

struct timespec TheLife::diff_timespec(struct timespec start, struct timespec end) {
    
    struct timespec result;

    if (end.tv_nsec < start.tv_nsec) { // peform carry like in normal subtraction
        //                123456789
        result.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
        result.tv_sec = end.tv_sec - 1 - start.tv_sec;
    }
    else {
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
        result.tv_sec = end.tv_sec - start.tv_sec;
    }

    return result;
}

long long TheLife::millisec_elapsed(struct timespec diff) {
    //                                123                     123456
    return ((long long)diff.tv_sec * 1000) + (diff.tv_nsec / 1000000);
}

std::vector<std::string> TheLife::DirExtension(std::string dir, std::string rexp) {
    
    struct dirent *items;
    std::vector<std::string> iName;

    if(DIR *folder = opendir(dir.c_str()))
    {
        while(items = readdir (folder)) {
            if(strlen(items->d_name) > rexp.size()) {
                if(CharCheck(rexp, items->d_name)) {
                    iName.push_back(items->d_name);
                }
            }
        }
        closedir(folder);
        return iName;
    }
    else
        Error("Error while opening folder %s\n", dir.c_str());
}

void TheLife::CharStr(std::string ch, std::string& str) {

    for(int i = 0; i < str.size(); ++i) {
        if(!strncmp((const char *)&str[i], ch.c_str(), ch.size())) {
            str.erase(i, ch.size());
        }
    }
}

bool TheLife::CharCheck(std::string ch, const char *str, int n) {

    int a = 0;
    
    if(n > a) {
        while(n != a){
            for(int i = 0; i < strlen(str); ++i) {
                if(!strncmp((strlen(str) > 1) ? &str[i] : str, ch.c_str(), ch.size())) {
                    ++a;
                }
            }
        }
    }
    else {
        for(int i = 0; i < strlen(str); ++i) {
            //std::cout << str[i] << " " << str << " " << ch << " " << ch.size() << " " << str.size() << std::endl;
            if(!strncmp((strlen(str) > 1) ? &str[i] : str, ch.c_str(), ch.size())) {
                return TRUE;
            }
        }
    }

    return (n > a) ? ((a >= n) ? FALSE : TRUE) : FALSE;
}