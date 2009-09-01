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

    sUI->SetY(y, WINDOW_STATUS);
    sUI->SetX(CONS_WIDTH, WINDOW_STATUS);
    sUI->SetWindow(newwin( sUI->GetY(WINDOW_STATUS), sUI->GetX(WINDOW_STATUS), 0, x - CONS_WIDTH ), WINDOW_STATUS);
    sUI->SetStatus(derwin( sUI->GetWindow(WINDOW_STATUS), sUI->GetY(WINDOW_STATUS) - 6 , sUI->GetX(WINDOW_STATUS) - 4, 1, 2 ), WINDOW_STATUS);
    idlok( sUI->GetStatus(WINDOW_STATUS, 0), TRUE );
    scrollok( sUI->GetStatus(WINDOW_STATUS, 0), TRUE );

    // Draw
    box( sUI->GetWindow(WINDOW_STATUS), 0, 0 );
    StatusTitle( text );
}

void Interface::StatusTitle(std::string text) {

    wattron( sUI->GetWindow(WINDOW_STATUS), A_BOLD );
    mvwaddnstr( sUI->GetWindow(WINDOW_STATUS), 0, 2, text.c_str(), sUI->GetX(WINDOW_STATUS) - 4 );
    wattroff( sUI->GetWindow(WINDOW_STATUS), A_BOLD );
}

void Interface::StatusOutput(std::string text, ...) {

    char buf[2048];
    va_list ap;

    va_start(ap, text.c_str());
    vsnprintf(buf, sizeof(buf) - 1, text.c_str(), ap);
    va_end(ap);

    if (CSBUF <= (strlen(sUI->lwin[WINDOW_STATUS].buf) + strlen(buf))) {
        strncpy(sUI->lwin[WINDOW_STATUS].buf, &sUI->lwin[WINDOW_STATUS].buf[strlen(buf)], CSBUF - 1);
        sUI->lwin[WINDOW_STATUS].buf[strlen(buf)] = 0x00;
    }
    strncat(sUI->lwin[WINDOW_STATUS].buf, buf, CSBUF - 1 - strlen(buf) - strlen(sUI->lwin[WINDOW_STATUS].buf));
    waddstr(sUI->GetStatus(WINDOW_STATUS, 0), buf);

    if(sUI)
    {
        wrefresh(sUI->GetStatus(WINDOW_STATUS, 0));
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

        werase(GetStatus(WINDOW_STATUS, 0));

        if(sTheLife->Active()) {
            
            StatusOutput("Name: %s\n", u->GetName().c_str());
            StatusOutput("Sex: %s\n", sex.c_str());
            StatusOutput("Age: %d\n", u->GetAge());
            StatusOutput("Occ: %s\n", occ.c_str());
        }
    }
}
