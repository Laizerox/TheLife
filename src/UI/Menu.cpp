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
#include "../config.h"
#include "../Language.h"
#include "Interface.h"
#include "App.h"

void Interface::LoadMenu()
{
    int y, x, c;
    Interface::Menu *menu = Interface::Menu::getInstance();
    TheLife::Application *App = TheLife::Application::getInstance();
    config *conf = config::getInstance();

    App->SetActive(FALSE);
    App->SetUnload(FALSE);
    menu->SetActive(TRUE);
    
    /* Initialize curses */
    initscr();
    start_color();
    use_default_colors();
    noecho();
    keypad( stdscr, TRUE );
    
    // Initialization
    getmaxyx( stdscr, y, x );

    menu->SetY(y - MENU_HEIGHT);
    menu->SetX(x - MENU_WIDTH);
    menu->SetWindow(newwin( 15, 30, (int)(menu->GetY() * 0.5), (int)(menu->GetX() * 0.5)));
        
    // Create items
    menu->ch = (ITEM **) calloc(4, sizeof(ITEM *));
    menu->ch[0] = new_item(Language::Get(MSG_MENU_OPT_START).c_str(), "");
    menu->ch[1] = new_item(Language::Get(MSG_MENU_OPT_OPTIONS).c_str(), "");
    menu->ch[2] = new_item(Language::Get(MSG_MENU_OPT_EXIT).c_str(), "");
    
    set_item_userptr(menu->ch[0], (void *) &Interface::MenuLife);
    set_item_userptr(menu->ch[2], (void *) &Interface::MenuQuit);   
    
    // Create menu
    menu->SetMenu(new_menu((ITEM **)menu->ch));

    keypad(menu->GetWindow(), TRUE);
     
    // Misc
    getmaxyx( menu->GetWindow(), y, x );
    
    menu_opts_off(menu->GetMenu(), O_SHOWDESC);
    set_menu_win(menu->GetMenu(), menu->GetWindow());
    set_menu_sub(menu->GetMenu(), derwin(menu->GetWindow(), 0, 0, 5, (int)(y * 0.5) + 2));
    set_menu_mark(menu->GetMenu(), " * ");

    mvprintw(LINES - 2, 0, Language::Get(MSG_MENU_USE).c_str());
    mvprintw(LINES - 1, 0, "%s %s - %s", Language::Get(NAME_THELIFE).c_str(), conf->GetVariable("version").c_str(), Language::Get(MSG_MENU_DEVEL).c_str());
    
    box(menu->GetWindow(), 0, 0);
    mvwhline( menu->GetWindow(), 2, 1, 0, x-2);
    refresh();
        
    post_menu(menu->GetMenu());
    wrefresh(menu->GetWindow());

    while(menu->Active())
    {
        // Key Handler
        if ((c = wgetch(menu->GetWindow())) != ERR) {  

            switch(c)
            {
                case KEY_DOWN:
                    menu_driver(menu->GetMenu(), REQ_DOWN_ITEM);
                    break;

                case KEY_UP:
                    menu_driver(menu->GetMenu(), REQ_UP_ITEM);
                    break;
                
                case 10: /* Enter */
                {
                    ITEM *current;
                    void (*p)();
                
                    current = current_item(menu->GetMenu());
                    p = (void (*)()) item_userptr(current);
                    p();
                    pos_menu_cursor(menu->GetMenu());
                    break;
                }
                break;
            }
            wrefresh(menu->GetWindow());
        }
    }
    
    unpost_menu(menu->GetMenu());
    
    for(int i = 0; i < item_count(menu->GetMenu()); ++i)
        free_item(menu->ch[i]);

    free_menu(menu->GetMenu());
    delwin(menu->GetWindow());
    free((void *) menu->ch);
    refresh();
    endwin();
    delete menu;
}

void Interface::MenuQuit() {
    
    Interface::Menu *menu = Interface::Menu::getInstance();
    menu->SetActive(FALSE);
}

void Interface::MenuLife() {
        
    Interface::Menu *menu = Interface::Menu::getInstance();
    TheLife::Application *App = TheLife::Application::getInstance();
    
    App->SetActive(TRUE);
    menu->SetActive(FALSE);
}
