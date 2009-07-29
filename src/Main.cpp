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

#include "Share.h"
#include "config.h"
#include "Language.h"
#include "UI/App.h"
#include "UI/Interface.h"

int main() {
    
    TheLife::Application *App = TheLife::Application::getInstance();
    Language::LanguageHeader *Lng = Language::LanguageHeader::getInstance();
    config *conf = config::getInstance();
    
    //Language::Load("finnish.lng");
    Lng->LoadHeader();    
    
    conf->LoadConf("data/life.config", conf);
    
    Interface::LoadMenu();
    
    if(App->Active())
        TheLife::LoadLife();
    
    if(App->Unload())
        TheLife::UnloadLife();

    delete App;
    delete conf;
    
    return 0;
}
