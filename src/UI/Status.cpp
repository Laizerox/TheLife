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

#include <cstring>

#include "../Thread.h"
#include "../Unit.h"
#include "App.h"
#include "Interface.h"

void Interface::StatusInit(std::string text) {

    int y, x;

    // Initialization
    getmaxyx( stdscr, y, x );

    sUI->SetY(y, WSTA);
    sUI->SetX(CONS_WIDTH, WSTA);
    sUI->SetWindow(newwin( sUI->GetY(WSTA), sUI->GetX(WSTA), 0, x - CONS_WIDTH ), WSTA);
    sUI->SetStatus(derwin( sUI->GetWindow(WSTA), sUI->GetY(WSTA) - 6 , sUI->GetX(WSTA) - 4, 1, 2 ), WSTA);
    idlok( sUI->GetStatus(WSTA, 0), TRUE );
    scrollok( sUI->GetStatus(WSTA, 0), TRUE );

    // Draw
    box( sUI->GetWindow(WSTA), 0, 0 );
    StatusTitle( text );
}

void Interface::StatusTitle(std::string text) {

    wattron( sUI->GetWindow(WSTA), A_BOLD );
    mvwaddnstr( sUI->GetWindow(WSTA), 0, 2, text.c_str(), sUI->GetX(WSTA) - 4 );
    wattroff( sUI->GetWindow(WSTA), A_BOLD );
}

void Interface::StatusOutput(std::string text, ...) {

    char buf[2048];
    va_list ap;

    va_start(ap, text.c_str());
    vsnprintf(buf, sizeof(buf) - 1, text.c_str(), ap);
    va_end(ap);

    if (CSBUF <= (strlen(sUI->lwin[WSTA].buf) + strlen(buf))) {
        strncpy(sUI->lwin[WSTA].buf, &sUI->lwin[WSTA].buf[strlen(buf)], CSBUF - 1);
        sUI->lwin[WSTA].buf[strlen(buf)] = 0x00;
    }
    strncat(sUI->lwin[WSTA].buf, buf, CSBUF - 1 - strlen(buf) - strlen(sUI->lwin[WSTA].buf));
    waddstr(sUI->GetStatus(WSTA, 0), buf);

    if(sUI)
    {
        wrefresh(sUI->GetStatus(WSTA, 0));
    }
}

void Interface::UI::StatusUpdate()  {
    
    Unit *u = sUnitStore->ReturnUnit(0);

    std::string sex;
    std::string occ;
    
    if(u) {

        switch(u->GetSex()) {

            case MALE: sex = "Male";
                 break;

            case FEMALE: sex = "Female";
                 break;

            case NONE:
            default: sex = "None";
        }

        switch(u->GetOcc()) {

            case UNEMPLOYEED: occ = "Unemployeed";
                 break;

            case EMPLOYEE: occ = "Employeed";
                 break;

            case STUDENT: occ = "Student";
                 break;

            case NONE:
            default: occ = "None";
        }

        werase(GetStatus(WSTA, 0));

        if(sTheLife->Active()) {
            
            StatusOutput("Name: %s\n", u->GetName().c_str());
            StatusOutput("Sex: %s\n", sex.c_str());
            StatusOutput("Age: %d\n", u->GetAge());
            StatusOutput("Occ: %s\n", occ.c_str());
        }
    }
}
