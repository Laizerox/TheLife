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

#include "../Language.h"
#include "Interface.h"
#include "App.h"

void Interface::Load() {

    // Initialize
    ConsoleInit("Console");
    StatusInit("Status");

    sUI->SetPanel(new_panel(sUI->GetWindow(WINDOW_CONSOLE)), WINDOW_CONSOLE); // Console
    sUI->SetPanel(new_panel(sUI->GetWindow(WINDOW_STATUS)), WINDOW_STATUS); // Status

    // Draw
    box(sUI->GetWindow(WINDOW_CONSOLE), 0, 0);
    ConsoleTitle("Console");

    box(sUI->GetWindow(WINDOW_STATUS), 0, 0);
    StatusTitle("Status");

    update_panels();
    doupdate();
}

void Interface::Unload()  {

    for(int i = 0; i < PANS; ++i) {
        
        sUI->lwin[(WinCodeID)i].window.clear();
        delwin(sUI->GetWindow((WinCodeID)i));
        del_panel(sUI->GetPanel((WinCodeID)i));
    }
    
    refresh();
    endwin();
}

void Interface::Resize() {
    
    Unload();
    initscr();
    erase();
    refresh();
    Load();
}

void Interface::UI::HidePanel(WinCodeID pan) {

    if(pan > PANS-1) {
        
        TheLife::Error(Language::Get(ERROR_HIDE_PANEL).c_str(), pan, PANS-2, PANS-1);
        return;
    }
    
    if(!GetHidden(pan)) {
        show_panel(GetPanel(pan));
        SetHidden(FALSE, pan);
    }
    else {
        hide_panel(GetPanel(pan));
        SetHidden(TRUE, pan);
    }
    
    update_panels();
    doupdate();
}
