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

#include "Interface.h"

void Interface::ConsoleInit(std::string text) {

    int y, x;

    // Initialization
    getmaxyx( stdscr, y, x );

    sUI->SetY(y, WCON);
    sUI->SetX(x - CONS_WIDTH, WCON);
    sUI->SetWindow(newwin( sUI->GetY(WCON), sUI->GetX(WCON), 0, 0 ), WCON);
    sUI->SetConsole(derwin( sUI->GetWindow(WCON), sUI->GetY(WCON) - 6 , sUI->GetX(WCON) - 4, 1, 2 ), WCON);
    idlok( sUI->GetConsole(WCON, 0), TRUE );
    scrollok( sUI->GetConsole(WCON, 0), TRUE );

    sUI->SetConsole(derwin( sUI->GetWindow(WCON), 3, sUI->GetX(WCON) - 4, sUI->GetY(WCON) - 4, 2 ), WCON);

    sUI->lwin[WCON].field_buf[0] = 0x00;
    sUI->lwin[WCON].field_ptr = 0x00;
    sUI->lwin[WCON].field_length = 0x00;

    // Draw
    getmaxyx( sUI->GetWindow(WCON), y, x );

    box( sUI->GetWindow(WCON), 0, 0 );
    ConsoleTitle( text );
    mvwhline( sUI->GetWindow(WCON), y - 5, 1, 0, x - 2 );
}

void Interface::ConsoleTitle(std::string text) {

    wattron( sUI->GetWindow(WCON), A_BOLD );
    mvwaddnstr( sUI->GetWindow(WCON), 0, 2, text.c_str(), sUI->GetX(0) - 4 );
    wattroff( sUI->GetWindow(WCON), A_BOLD );
}

void Interface::ConsoleBuffer(WINDOW *cons, char *buffer, int height, int width, int ptr) {

    int a, y, x, area, offset;

    area = height * width;
    offset = (ptr / area) * area;
    a = ptr % area;
    y = a / width;
    x = a % width;

    werase(cons);
    waddnstr(cons, &buffer[offset], area);
    wmove(cons, y, x);
    wrefresh(cons);
}

void Interface::ConsoleOutput(std::string text, ...) {

    char buf[2048];
    va_list ap;

    va_start(ap, text.c_str());
    vsnprintf(buf, sizeof(buf) - 1, text.c_str(), ap);
    va_end(ap);

    if (CSBUF <= (strlen(sUI->lwin[WCON].buf) + strlen(buf))) {
        strncpy(sUI->lwin[WCON].buf, &sUI->lwin[WCON].buf[strlen(buf)], CSBUF - 1);
        sUI->lwin[WCON].buf[strlen(buf)] = 0x00;
    }
    strncat(sUI->lwin[WCON].buf, buf, CSBUF - 1 - strlen(buf) - strlen(sUI->lwin[WCON].buf));
    waddstr(sUI->GetConsole(WCON, 0), buf);

    if(sUI)
    {
        wrefresh(sUI->GetConsole(WCON, 0));
    }
}

