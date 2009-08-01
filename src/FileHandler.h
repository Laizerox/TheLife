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

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Singleton.h"

#define sFileHandler FileHandler::LD_FILE::getInstance()

#define LDV 0x01
#define LDH 0x01

namespace FileHandler {

        class LD_FILE : public Singleton<LD_FILE> {

            public:
                LD_FILE() {}
                int loaded_ldv;
                int loaded_ldh;
                bool first_load;
        };

        inline bool CheckLDV(int version) { return (version == LDV) ? true : false; }
        inline bool CheckLDH(int header) { return (header == LDH) ?  true : false; }

        // Player Init & Load/Save
        void PlayerInit();
        void PlayerUnload();
        void SavePlayer();
        bool LoadPlayer();
};

#endif
