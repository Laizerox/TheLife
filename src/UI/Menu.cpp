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

#include <cstdlib>

#include "../config.h"
#include "../Language.h"
#include "Interface.h"
#include "App.h"

void Interface::LoadMenu()
{
    int y, x, c;

    sTheLife->SetActive(false);
    sTheLife->SetUnload(false);
    sMenu->SetActive(true);
    
    /* Initialize curses */
    initscr();
    start_color();
    use_default_colors();
    noecho();
    keypad(stdscr, true);
    
    // Initialization
    getmaxyx( stdscr, y, x );

    sMenu->SetY(y - MENU_HEIGHT);
    sMenu->SetX(x - MENU_WIDTH);
    sMenu->SetWindow(newwin(15, 30, (int)(sMenu->GetY() * 0.5), (int)(sMenu->GetX() * 0.5)));
        
    // Create items
    sMenu->ch = (ITEM **) calloc(4, sizeof(ITEM *));
    sMenu->ch[0] = new_item(sLanguage->Get("MSG_MENU_OPT_START").c_str(), "");
    sMenu->ch[1] = new_item(sLanguage->Get("MSG_MENU_OPT_OPTIONS").c_str(), "");
    sMenu->ch[2] = new_item(sLanguage->Get("MSG_MENU_OPT_EXIT").c_str(), "");
    
    set_item_userptr(sMenu->ch[0], (void *) &Interface::MenuLife);
    set_item_userptr(sMenu->ch[2], (void *) &Interface::MenuQuit);   
    
    // Create menu
    sMenu->SetMenu(new_menu((ITEM **)sMenu->ch));

    keypad(sMenu->GetWindow(), true);
     
    // Misc
    getmaxyx( sMenu->GetWindow(), y, x );
    
    menu_opts_off(sMenu->GetMenu(), O_SHOWDESC);
    set_menu_win(sMenu->GetMenu(), sMenu->GetWindow());
    set_menu_sub(sMenu->GetMenu(), derwin(sMenu->GetWindow(), 0, 0, 5, (int)(y * 0.5) + 2));
    set_menu_mark(sMenu->GetMenu(), " * ");

    mvprintw(LINES - 2, 0, sLanguage->Get("MSG_MENU_USE").c_str());
    mvprintw(LINES - 1, 0, "%s %s - %s / Compiled with: %s (%s)", sLanguage->Get("NAME_THELIFE").c_str(), config::getInstance()->GetVariable("version").c_str(), sLanguage->Get("MSG_MENU_DEVEL").c_str(), __VERSION__, __DATE__);
    
    box(sMenu->GetWindow(), 0, 0);
    mvwhline( sMenu->GetWindow(), 2, 1, 0, x-2);
    refresh();
        
    post_menu(sMenu->GetMenu());
    wrefresh(sMenu->GetWindow());

    while(sMenu->Active())
    {
        // Key Handler
        if ((c = wgetch(sMenu->GetWindow())) != ERR) {  

            switch(c)
            {
                case KEY_DOWN:
                    menu_driver(sMenu->GetMenu(), REQ_DOWN_ITEM);
                    break;

                case KEY_UP:
                    menu_driver(sMenu->GetMenu(), REQ_UP_ITEM);
                    break;
                
                case 10: /* Enter */
                {
                    ITEM *current;
                    void (*p)();
                
                    current = current_item(sMenu->GetMenu());
                    p = (void (*)()) item_userptr(current);
                    p();
                    pos_menu_cursor(sMenu->GetMenu());
                    break;
                }
                break;
            }
            wrefresh(sMenu->GetWindow());
        }
    }
    
    unpost_menu(sMenu->GetMenu());
    
    for(int i = 0; i < item_count(sMenu->GetMenu()); ++i)
        free_item(sMenu->ch[i]);

    free_menu(sMenu->GetMenu());
    delwin(sMenu->GetWindow());
    free((void *) sMenu->ch);
    refresh();
    endwin();
}

void Interface::MenuQuit() {
    
    sMenu->SetActive(false);
}

void Interface::MenuLife() {
    
    sTheLife->SetActive(true);
    sMenu->SetActive(false);
}
