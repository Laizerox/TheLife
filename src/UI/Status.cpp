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

#include "../Share.h"
#include "../Thread.h"
#include "../Unit.h"
#include "App.h"
#include "Interface.h"

void Interface::StatusInit(std::string text) {

    int y, x;
    Interface::UI *ui = Interface::UI::getInstance();

    // Initialization
    getmaxyx( stdscr, y, x );

    ui->SetY(y, WSTA);
    ui->SetX(CONS_WIDTH, WSTA);
    ui->SetWindow(newwin( ui->GetY(WSTA), ui->GetX(WSTA), 0, x - CONS_WIDTH ), WSTA);
    ui->SetStatus(derwin( ui->GetWindow(WSTA), ui->GetY(WSTA) - 6 , ui->GetX(WSTA) - 4, 1, 2 ), WSTA);
    idlok( ui->GetStatus(WSTA, 0), TRUE );
    scrollok( ui->GetStatus(WSTA, 0), TRUE );

    // Draw
    box( ui->GetWindow(WSTA), 0, 0 );
    StatusTitle( text );
}

void Interface::StatusTitle(std::string text) {

    Interface::UI *ui = Interface::UI::getInstance();

    wattron( ui->GetWindow(WSTA), A_BOLD );
    mvwaddnstr( ui->GetWindow(WSTA), 0, 2, text.c_str(), ui->GetX(WSTA) - 4 );
    wattroff( ui->GetWindow(WSTA), A_BOLD );
}

void Interface::StatusOutput(std::string text, ...) {

    char buf[2048];
    va_list ap;

    Interface::UI *ui = Interface::UI::getInstance();

    va_start(ap, text.c_str());
    vsnprintf(buf, sizeof(buf) - 1, text.c_str(), ap);
    va_end(ap);

    if (CSBUF <= (strlen(ui->lwin[WSTA].buf) + strlen(buf))) {
        strncpy(ui->lwin[WSTA].buf, &ui->lwin[WSTA].buf[strlen(buf)], CSBUF - 1);
        ui->lwin[WSTA].buf[strlen(buf)] = 0x00;
    }
    strncat(ui->lwin[WSTA].buf, buf, CSBUF - 1 - strlen(buf) - strlen(ui->lwin[WSTA].buf));
    waddstr(ui->GetStatus(WSTA, 0), buf);

    if(ui)
    {
        wrefresh(ui->GetStatus(WSTA, 0));
    }
}

void Interface::UI::StatusUpdate()  {

    UnitStore *us = UnitStore::getInstance();
    Unit *u = us->ReturnUnit(0);
    TheLife::Application *App = TheLife::Application::getInstance();

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

        if(App->Active()) {
            
            StatusOutput("Name: %s\n", u->GetName().c_str());
            StatusOutput("Sex: %s\n", sex.c_str());
            StatusOutput("Age: %d\n", u->GetAge());
            StatusOutput("Occ: %s\n", occ.c_str());
        }
    }
}
