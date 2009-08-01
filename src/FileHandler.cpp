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

#include <fstream>
#include <string>

#include "FileHandler.h"
#include "Unit.h"
#include "config.h"
#include "Language.h"
#include "UI/App.h"

void FileHandler::PlayerInit() {

    Unit *u = new Unit(); 

    sUnitStore->StoreUnit(u);
    
    if(!LoadPlayer()) {

      if(sFileHandler->first_load)  {
          sFileHandler->loaded_ldh = LDH;
          sFileHandler->loaded_ldv = LDV;
      }

      SavePlayer();
    }
}

void FileHandler::PlayerUnload() {

    SavePlayer();
    
    for(int i = 0; i < sUnitStore->UnitSize(); ++i)
        delete sUnitStore->ReturnUnit(i);
}

void FileHandler::SavePlayer() {

    Unit *u = sUnitStore->ReturnUnit(0);

    std::string path = sConfig->GetVariable("ldpath");
    std::string name = sConfig->GetVariable("playerfile");
    
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

    tmpInt = sFileHandler->loaded_ldh;
    player_ld.write((char *)&tmpInt, 4);

    tmpInt = sFileHandler->loaded_ldv;
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

    Unit *u = sUnitStore->ReturnUnit(0);

    std::string path = sConfig->GetVariable("ldpath");
    std::string name = sConfig->GetVariable("playerfile");

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

        sFileHandler->first_load = true;
        return false;
    }

    player_ld.read((char *)&tmpInt, 4);
    sFileHandler->loaded_ldh = tmpInt;

    player_ld.read((char *)&tmpInt, 4);
    sFileHandler->loaded_ldv = tmpInt;

    if(!CheckLDH(sFileHandler->loaded_ldh)) {

        TheLife::Error(Language::Get(ERROR_PLAYER_WRONG_DHEAD).c_str(), sFileHandler->loaded_ldh);
        return false;
    }

    if(!CheckLDV(sFileHandler->loaded_ldv)) {

        TheLife::Error(Language::Get(ERROR_PLAYER_WRONG_DVER).c_str(), sFileHandler->loaded_ldv);
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
