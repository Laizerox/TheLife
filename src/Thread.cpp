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

#include <csignal>
#include <cerrno>
#include <cstdio>
#include <ncursesw/ncurses.h>

#include "SignalDef.h"
#include "Thread.h"
#include "Language.h"
#include "LogHandler.h"
#include "UI/App.h"

#define MICRO 1000000

void *Thread::Run(void *arg) {

    int key;

    // Life Main loop
    while(sTheLife->Active()) {
  
        // Key Handler
        if ((key = getch()) != ERR) {
            TheLife::KeyHandle(key, 3);    
        }
    }
    
    LogHandler::WriteLog(sLanguage->Get("LOG_TH_RUN"));
}

void *Thread::Status(void *arg) {

    struct itimerspec itime;
    struct sigaction action;
    struct sigevent event;
    
    timer_t timer_id;
    errno = 0;
 
    action.sa_handler = TheLife::SigHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = NULL;   
    sigaction(SIG_STATUS, &action, NULL);
    
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIG_STATUS;
    
    if(timer_create(CLOCK_REALTIME, &event, &timer_id) < 0)  {
        TheLife::Error(sLanguage->Get("ERROR_TH_STATUS_TIMER_CREATE").c_str(), errno);
        return (void *)NULL;
    }
    
    itime.it_value.tv_sec = 0;
    itime.it_value.tv_nsec = 100000000; 
    itime.it_interval.tv_sec = 0;
    itime.it_interval.tv_nsec = 100000000; 

    if(timer_settime(timer_id, 0, &itime, NULL) < 0)  {
        TheLife::Error(sLanguage->Get("ERROR_TH_STATUS_TIMER_SET").c_str(), errno);
        return (void *)NULL;
    }
    
    LogHandler::WriteLog(sLanguage->Get("LOG_TH_STATUS"));
}
