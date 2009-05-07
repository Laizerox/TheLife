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
#include "Interface.h"

void Interface::ConsoleInit(std::string text) {

    int y, x;
    Interface::UI *ui = Interface::UI::getInstance();

    // Initialization
    getmaxyx( stdscr, y, x );

    ui->SetY(y, WCON);
    ui->SetX(x - CONS_WIDTH, WCON);
    ui->SetWindow(newwin( ui->GetY(WCON), ui->GetX(WCON), 0, 0 ), WCON);
    ui->SetConsole(derwin( ui->GetWindow(WCON), ui->GetY(WCON) - 6 , ui->GetX(WCON) - 4, 1, 2 ), WCON);
    idlok( ui->GetConsole(WCON, 0), TRUE );
    scrollok( ui->GetConsole(WCON, 0), TRUE );

    ui->SetConsole(derwin( ui->GetWindow(WCON), 3, ui->GetX(WCON) - 4, ui->GetY(WCON) - 4, 2 ), WCON);

    ui->lwin[WCON].field_buf[0] = 0x00;
    ui->lwin[WCON].field_ptr = 0x00;
    ui->lwin[WCON].field_length = 0x00;

    // Draw
    getmaxyx( ui->GetWindow(WCON), y, x );

    box( ui->GetWindow(WCON), 0, 0 );
    ConsoleTitle( text );
    mvwhline( ui->GetWindow(WCON), y - 5, 1, 0, x - 2 );
}

void Interface::ConsoleTitle(std::string text) {

    Interface::UI *ui = Interface::UI::getInstance();

    wattron( ui->GetWindow(WCON), A_BOLD );
    mvwaddnstr( ui->GetWindow(WCON), 0, 2, text.c_str(), ui->GetX(0) - 4 );
    wattroff( ui->GetWindow(WCON), A_BOLD );
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

    Interface::UI *ui = Interface::UI::getInstance();

    va_start(ap, text.c_str());
    vsnprintf(buf, sizeof(buf) - 1, text.c_str(), ap);
    va_end(ap);

    if (CSBUF <= (strlen(ui->lwin[WCON].buf) + strlen(buf))) {
        strncpy(ui->lwin[WCON].buf, &ui->lwin[WCON].buf[strlen(buf)], CSBUF - 1);
        ui->lwin[WCON].buf[strlen(buf)] = 0x00;
    }
    strncat(ui->lwin[WCON].buf, buf, CSBUF - 1 - strlen(buf) - strlen(ui->lwin[WCON].buf));
    waddstr(ui->GetConsole(WCON, 0), buf);

    if(ui)
    {
        wrefresh(ui->GetConsole(WCON, 0));
    }
}

