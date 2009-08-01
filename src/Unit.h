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

#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>

#include "Singleton.h"

#define sUnitStore UnitStore::getInstance()

enum SEX {

    NONE    = 0,
    MALE    = 1,
    FEMALE  = 2
};

enum OCCUPATION {

    UNEMPLOYEED   = 1,
    STUDENT       = 2,
    EMPLOYEE      = 3
};

class Unit {

    private:
        std::string name;
        int sex;
        int age;
        int occupation;

    public:
        Unit() {

            name = "NONE";
            sex = 0;
            age = 0;
            occupation = 0; }

        // Return functions.
        virtual std::string GetName() { return name; }
        virtual int GetSex() { return sex; }
        virtual int GetAge() { return age; }
        virtual int GetOcc() { return occupation; }

        // Set functions.
        virtual void SetName(std::string name) { this->name = name; }
        virtual void SetSex(int sex) { this->sex = sex; }
        virtual void SetAge(int age) { this->age = age; }
        virtual void SetOcc(int occ) { this->occupation = occ; }

};

class UnitStore : public Singleton<UnitStore> {

    private:
        std::vector<Unit *> StoredUnits;

    public:
        UnitStore() {}
        
        void StoreUnit(Unit *u) { this->StoredUnits.push_back(u); }
        virtual Unit *ReturnUnit(int i) { return StoredUnits[i]; }
        int UnitSize() { return StoredUnits.size(); }
};

#endif
