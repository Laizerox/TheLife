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
#include "../Language.h"
#include "Interface.h"
#include "App.h"

void Interface::Load() {

    Interface::UI *ui = Interface::UI::getInstance();

    // Initialize
    ConsoleInit("Console");
    StatusInit("Status");

    ui->SetPanel(new_panel(ui->GetWindow(WCON)), WCON); // Console
    ui->SetPanel(new_panel(ui->GetWindow(WSTA)), WSTA); // Status

    // Draw
    box(ui->GetWindow(WCON), 0, 0);
    ConsoleTitle("Console");

    box(ui->GetWindow(WSTA), 0, 0);
    StatusTitle("Status");

    update_panels();
    doupdate();
}

void Interface::Unload()  {

    Interface::UI *ui = Interface::UI::getInstance();

    for(int i = 0; i < PANS; ++i) {
        
        ui->lwin[i].window.clear();
        delwin(ui->GetWindow(i));
        del_panel(ui->GetPanel(i));
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

void Interface::UI::HidePanel(int pan) {

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
