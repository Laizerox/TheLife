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

#include <iostream>

#include "config.h"
#include "Language.h"
#include "UI/App.h"
#include "UI/Interface.h"

int main() {

    //Language::Load("finnish.lng");
    sLanguage->LoadHeader();
    sTheLife->SetActiveLanguage("Russian");
    
    sConfig->LoadConf("data/life.config");
    
    Interface::LoadMenu();
    
    if(sTheLife->Active())
        TheLife::LoadLife();
    
    if(sTheLife->Unload())
        TheLife::UnloadLife();
    
    return 0;
}
