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

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

struct CommandHandler;

struct CommandTable {
    const char *name;
    void(CommandHandler::*Handler)(std::string arg);
    CommandTable *SubCMD;
};

class CommandHandler {

    protected:
        CommandTable* commands();
        void help(std::string arg);
        void exit(std::string arg);
        void about(std::string arg);
        void threads(std::string arg);
        void ConfigList(std::string arg);
        void GetConfig(std::string arg);
        void UnitEditName(std::string arg);
        void UnitEditAge(std::string arg);
        void UnitEditSex(std::string arg);
        void UnitEditOcc(std::string arg);
        void hide(std::string arg);
        void ConfigAddConfig(std::string arg);
        void ConfigAddComment(std::string arg);
    public:
        void const LookupCommand(CommandTable *table, std::string cmd);
        void const CommandProcess(char *buffer);
};

#endif
