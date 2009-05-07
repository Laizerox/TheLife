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

#ifndef INTERFACE_H
#define INTERFACE_H

#define CONS_WIDTH   20
#define MENU_HEIGHT  15
#define MENU_WIDTH   30
#define PANS 2

enum Buffers {

    FIELDBUFSIZE  = 512,
    CSBUF         = 2048
};

enum WinTypeID {

    UINIT   = 0, // uninitialized
    CONSOLE = 1, // normal console
    STATUS  = 2, // status window
    DEBUG   = 3  // debug console
};

enum WinCodeID {

    WCON = 0,
    WMEN = 0,
    WSTA = 1,
    WDEB = 2
};

typedef struct _LIFE_WINDOW {

    std::vector<WINDOW *> window;
    char buf[CSBUF], field_buf[FIELDBUFSIZE];
    int field_ptr, field_length;
} LIFE_WINDOW;

typedef struct _LIFE_PANEL {

    bool hidden;
    int x, y;
    WINDOW *win;
    int type;
} LIFE_PANEL;

typedef struct _LIFE_MENU {

    int x, y;
    WINDOW  *mwin;
    MENU    *menu;
} LIFE_MENU;

namespace Interface {

        // Console Related Functions
        void ConsoleInit(std::string text);
        void ConsoleTitle(std::string text);
        void ConsoleBuffer(WINDOW *cons, char *buffer, int height, int width, int ptr);
        void ConsoleOutput(std::string text, ...);
        
        // Status Box Related Functions
        void StatusInit(std::string text);
        void StatusTitle(std::string text);
        void StatusOutput(std::string text, ...);

        // Interface Initialization
        void Load();
        void Unload();
        void Resize();

        class UI {

            protected:
                static UI* Instance;
                UI() {}

            private:
                PANEL *panel[PANS];
                LIFE_PANEL lpan[PANS];

            public:
                static UI* getInstance();

                LIFE_WINDOW lwin[PANS];

                // Console handler
                WINDOW *GetConsole(int win, int ex) { return lwin[win].window[ex]; }
                void SetConsole(WINDOW *cons, int win) { this->lwin[win].window.push_back(cons); }
                
                // Status handler
                WINDOW *GetStatus(int win, int ex) { return lwin[win].window[ex]; }
                void SetStatus(WINDOW *stat, int win) { this->lwin[win].window.push_back(stat); }
                void StatusUpdate();

                // Windows handling
                // -- Get functions --
                int GetY(int pan) { return lpan[pan].y; }
                int GetX(int pan) { return lpan[pan].x; }
                
                // -- Set functions --
                void SetY(int y, int pan) { this->lpan[pan].y = y; }
                void SetX(int x, int pan) { this->lpan[pan].x = x; }
                
                // Panel handling
                void HidePanel(int pan);
                
                // -- Get functions
                PANEL *GetPanel(int pan) { return panel[pan]; }
                bool GetHidden(int pan) { return lpan[pan].hidden; }
                int GetType(int pan) { return lpan[pan].type; }
                WINDOW *GetWindow(int pan) { return lpan[pan].win; }
                
                // -- Set functions --
                void SetPanel(PANEL *panel, int pan) { this->panel[pan] = panel; }
                void SetHidden(bool hide, int pan) { this->lpan[pan].hidden = hide; }
                void SetType(int type, int pan) { this->lpan[pan].type = type; }
                void SetWindow(WINDOW *win, int pan) { this->lpan[pan].win = win; }
        };
        
        // Menu Section
        void LoadMenu();
        void MenuQuit();
        void MenuLife();
        void MenuOptions();
        
        class Menu {
        
            protected:
                static Menu* Instance;
                Menu() {}

            private:
                LIFE_MENU lmenu;
                bool quit;

            public:
                static Menu* getInstance();
                ITEM    **ch;
                
                // -- Get functions --
                int GetY() { return lmenu.y; }
                int GetX() { return lmenu.x; }
                WINDOW *GetWindow() { return lmenu.mwin; }
                MENU *GetMenu() { return lmenu.menu; }
                
                // -- Set functions --
                void SetY(int y) { this->lmenu.y = y; }
                void SetX(int x) { this->lmenu.x = x; }
                void SetWindow(WINDOW *win) { this->lmenu.mwin = win; }
                void SetMenu(MENU *menu) { this->lmenu.menu = menu; }
                
                // ??? -- TODO
                bool Active() { return quit; }
                void SetActive(bool q) { this->quit = q; }
        };
};

#endif

