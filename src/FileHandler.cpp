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
#include "FileHandler.h"
#include "Unit.h"
#include "config.h"
#include "Language.h"
#include "UI/App.h"

FileHandler::LD_FILE* FileHandler::LD_FILE::Instance = NULL;

FileHandler::LD_FILE* FileHandler::LD_FILE::getInstance()
{
    if(Instance == NULL)  {

        Instance = new FileHandler::LD_FILE();
    }

    return Instance;
}

void FileHandler::PlayerInit() {

    UnitStore *us = UnitStore::getInstance();
    Unit *u = new Unit(); 
    LD_FILE *ld_f = LD_FILE::getInstance();

    us->StoreUnit(u);
    
    if(!LoadPlayer()) {

      if(ld_f->first_load)  {
          ld_f->loaded_ldh = LDH;
          ld_f->loaded_ldv = LDV;
      }

      SavePlayer();
    }
}

void FileHandler::PlayerUnload() {

    SavePlayer();
    
    UnitStore *us = UnitStore::getInstance();
    LD_FILE *ld_f = LD_FILE::getInstance();
    
    for(int i = 0; i < us->UnitSize(); ++i)
        delete us->ReturnUnit(i);
    
    delete ld_f;
    delete us;
}

void FileHandler::SavePlayer() {

    config *conf = config::getInstance();
    UnitStore *us = UnitStore::getInstance();
    Unit *u = us->ReturnUnit(0);
    LD_FILE *ld_f = LD_FILE::getInstance();

    std::string path = conf->GetVariable("ldpath");
    std::string name = conf->GetVariable("playerfile");
    
    int tmpInt;
    std::string tmpStr;
    bool tmpBool;

    if(path.empty() | name.empty()) {
        path = ".";
        name = "player.ld";
    }

    std::string file = path;
    file += "/";
    file += name;

    std::fstream player_ld (file.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

    tmpInt = ld_f->loaded_ldh;
    player_ld.write((char *)&tmpInt, 4);

    tmpInt = ld_f->loaded_ldv;
    player_ld.write((char *)&tmpInt, 4);

    tmpInt = (u->GetName().size());
    player_ld.write((char *)&tmpInt, 4);

    tmpStr = u->GetName();
    player_ld.write(tmpStr.c_str(), tmpStr.size());

    tmpInt = u->GetAge();
    player_ld.write((char *)&tmpInt, 4);

    tmpInt = u->GetSex();
    player_ld.write((char *)&tmpInt, 4);

    tmpInt = u->GetOcc();
    player_ld.write((char *)&tmpInt, 4);

    player_ld.close();
}

bool FileHandler::LoadPlayer() {

    config *conf = config::getInstance();
    UnitStore *us = UnitStore::getInstance();
    Unit *u = us->ReturnUnit(0);
    LD_FILE *ld_f = LD_FILE::getInstance();

    std::string path = conf->GetVariable("ldpath");
    std::string name = conf->GetVariable("playerfile");

    int tmpInt;
    std::string tmpStr;
    bool tmpBool;

    if(path.empty() | name.empty()) {
        path = ".";
        name = "player.ld";
    }

    std::string file = path;
    file += "/";
    file += name;

    std::fstream player_ld (file.c_str(), std::ios_base::in | std::ios_base::binary);

    if(!player_ld) {

        ld_f->first_load = true;
        return false;
    }

    player_ld.read((char *)&tmpInt, 4);
    ld_f->loaded_ldh = tmpInt;

    player_ld.read((char *)&tmpInt, 4);
    ld_f->loaded_ldv = tmpInt;

    if(!CheckLDH(ld_f->loaded_ldh)) {

        TheLife::Error(Language::Get(ERROR_PLAYER_WRONG_DHEAD).c_str(), ld_f->loaded_ldh);
        return false;
    }

    if(!CheckLDV(ld_f->loaded_ldv)) {

        TheLife::Error(Language::Get(ERROR_PLAYER_WRONG_DVER).c_str(), ld_f->loaded_ldv);
        return false;
    }

    player_ld.read((char *)&tmpInt, 4);
    tmpStr.resize(tmpInt);

    player_ld.read((char *)tmpStr.data(), tmpInt);
    u->SetName(tmpStr);

    player_ld.read((char *)&tmpInt, 4);
    u->SetAge(tmpInt);

    player_ld.read((char *)&tmpInt, 4);
    u->SetSex(tmpInt);

    player_ld.read((char *)&tmpInt, 4);
    u->SetOcc(tmpInt);

    player_ld.close();
    
    return true;
}
