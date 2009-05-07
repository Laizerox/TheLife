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
#include "Interface.h"

Interface::UI* Interface::UI::Instance = NULL;
Interface::Menu* Interface::Menu::Instance = NULL;

Interface::UI* Interface::UI::getInstance()
{
    if(Instance == NULL)  {

        Instance = new UI();
    }

    return Instance;
}

Interface::Menu* Interface::Menu::getInstance()
{
    if(Instance == NULL)  {

        Instance = new Menu();
    }

    return Instance;
}
