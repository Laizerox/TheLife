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

    sUI->SetY(y, WINDOW_CONSOLE);
    sUI->SetX(x - CONS_WIDTH, WINDOW_CONSOLE);
    sUI->SetWindow(newwin( sUI->GetY(WINDOW_CONSOLE), sUI->GetX(WINDOW_CONSOLE), 0, 0 ), WINDOW_CONSOLE);
    sUI->SetConsole(derwin( sUI->GetWindow(WINDOW_CONSOLE), sUI->GetY(WINDOW_CONSOLE) - 6 , sUI->GetX(WINDOW_CONSOLE) - 4, 1, 2 ), WINDOW_CONSOLE);
    idlok( sUI->GetConsole(WINDOW_CONSOLE, 0), TRUE );
    scrollok( sUI->GetConsole(WINDOW_CONSOLE, 0), TRUE );

    sUI->SetConsole(derwin( sUI->GetWindow(WINDOW_CONSOLE), 3, sUI->GetX(WINDOW_CONSOLE) - 4, sUI->GetY(WINDOW_CONSOLE) - 4, 2 ), WINDOW_CONSOLE);

    sUI->lwin[WINDOW_CONSOLE].field_buf[0] = 0x00;
    sUI->lwin[WINDOW_CONSOLE].field_ptr = 0x00;
    sUI->lwin[WINDOW_CONSOLE].field_length = 0x00;

    // Draw
    getmaxyx( sUI->GetWindow(WINDOW_CONSOLE), y, x );

    box( sUI->GetWindow(WINDOW_CONSOLE), 0, 0 );
    ConsoleTitle( text );
    mvwhline( sUI->GetWindow(WINDOW_CONSOLE), y - 5, 1, 0, x - 2 );
}

void Interface::ConsoleTitle(std::string text) {

    wattron( sUI->GetWindow(WINDOW_CONSOLE), A_BOLD );
    mvwaddnstr( sUI->GetWindow(WINDOW_CONSOLE), 0, 2, text.c_str(), sUI->GetX(WINDOW_CONSOLE) - 4 );
    wattroff( sUI->GetWindow(WINDOW_CONSOLE), A_BOLD );
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

    if (CSBUF <= (strlen(sUI->lwin[WINDOW_CONSOLE].buf) + strlen(buf))) {
        strncpy(sUI->lwin[WINDOW_CONSOLE].buf, &sUI->lwin[WINDOW_CONSOLE].buf[strlen(buf)], CSBUF - 1);
        sUI->lwin[WINDOW_CONSOLE].buf[strlen(buf)] = 0x00;
    }
    strncat(sUI->lwin[WINDOW_CONSOLE].buf, buf, CSBUF - 1 - strlen(buf) - strlen(sUI->lwin[WINDOW_CONSOLE].buf));
    waddstr(sUI->GetConsole(WINDOW_CONSOLE, 0), buf);

    if(sUI)
    {
        wrefresh(sUI->GetConsole(WINDOW_CONSOLE, 0));
    }
}

