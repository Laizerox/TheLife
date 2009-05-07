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
#include "CommandHandler.h"
#include "config.h"
#include "Unit.h"
#include "Language.h"
#include "Thread.h"
#include "UI/App.h"
#include "UI/Interface.h"

CommandTable * CommandHandler::commands()
{
    // This is our command table. Here we list commands with pointed functions to be executed
    static CommandTable pHints[] =
    {
        { "about",    &CommandHandler::about,   NULL },
        { "help",     &CommandHandler::help,    NULL },
        { NULL,       NULL,                     NULL }
    };

    static CommandTable pUnit[] =
    {
        { "name",     &CommandHandler::UnitEditName,   NULL },
        { "age",      &CommandHandler::UnitEditAge,    NULL },
        { "sex",      &CommandHandler::UnitEditSex,    NULL },
        { "occ",      &CommandHandler::UnitEditOcc,    NULL },
        { NULL,       NULL,                            NULL }
    };

    static CommandTable pConf[] =
    {
        { "config",   &CommandHandler::ConfigAddConfig,     NULL },
        { "comment",  &CommandHandler::ConfigAddComment,    NULL },
        { NULL,       NULL,                                 NULL }
    };

    static CommandTable pCommand[] =
    {
        { "help",               NULL,                         pHints },
        { "getconf",            &CommandHandler::GetConfig,   NULL   },
        { "exit",               &CommandHandler::exit,        NULL   },
        { "threads" ,           &CommandHandler::threads,     NULL   },
        { "confs" ,             &CommandHandler::ConfigList,  NULL   },
        { "editunit" ,          NULL,                         pUnit  },
        { "addconf" ,           NULL,                         pConf  },
        { "hide" ,              &CommandHandler::hide,        NULL   },
        { NULL,                 NULL,                         NULL   }
    };

    return pCommand;
}

// This command looks for command from table and executes it if found.
void const CommandHandler::LookupCommand(CommandTable *table, std::string cmd)  {

    config *conf = config::getInstance();
    bool found = false;
    std::vector<std::string> lines;
    std::string arg;
    std::string text;

    // Check that cmd is not empty otherwise we will crash
    if(!cmd.empty()) {

        lines = TheLife::SeparateString(cmd);

        for(int i = 1; i < lines.size(); ++i)
        {
            if(i == 1)
            {
              arg = lines[i];
            }
            else
            {
              arg += " ";
              arg += lines[i];
            }
         }

        if(arg.empty()) arg = cmd;

        // Run throught whole table looking for name of command
        for(int i = 0; table[i].name != NULL; ++i)
        {
            if(table[i].SubCMD != NULL && !strcmp(table[i].name,lines[0].c_str()))
            {
                LookupCommand(table[i].SubCMD, arg);
                TheLife::Debug(Language::Get(DEBUG_LOOKUP_LOOP).c_str(), table, i);
                return;
            }
            
            if(!table[i].Handler)
                continue;

            if(!strcmp(table[i].name,lines[0].c_str()))
            {
              found = true;
              (this->*(table[i].Handler))(arg);
            }
        }
    }

    // If not found report that it was not found and what we typed
    if(!found)
        TheLife::Error(Language::Get(ERROR_WRONG_CMD).c_str(), lines[0].c_str());
} 

void CommandHandler::help(std::string arg) {

    Interface::ConsoleOutput("=== Help ===\n"
                     "Welcome to help. This command is designed to give user some knowledge about:\n"
                     "  * Software itself\n"
                     "  * Commands sotware has\n"
                     "  * Commands usage\n\n");
}

void CommandHandler::exit(std::string arg) {

    TheLife::Application *App = TheLife::Application::getInstance();
    App->SetActive(FALSE);
    App->SetUnload(TRUE);
    TheLife::Debug(Language::Get(DEBUG_CALL_EXIT).c_str());
}

void CommandHandler::about(std::string arg) {

    config *conf = config::getInstance();
    Thread::cThread *th = Thread::cThread::getInstance();
    Interface::ConsoleOutput("=== About ===\n"
                     "This project is written by:\n"
                     "  * Laizerox\n\n"
                     "The purpose of this project:\n"
                     "  * Create limitless life simulator\n"
                     "  * Teach about C/C++ programming\n\n"
                     "Version: %s\n"
                     "Confs loaded: %d\n"
                     "Threads created: %d\n"
                     "=============\n\n", conf->GetVariable("version").c_str(), conf->GetDataSize(), th->ListThread());
}

void CommandHandler::threads(std::string arg) {

    Thread::cThread *th = Thread::cThread::getInstance();

    for(int i = 0; i < th->ListThread(); ++i)
        Interface::ConsoleOutput(Language::Get(CMD_TH_POINTER).c_str(), i, th->GetThread(i));
}

void CommandHandler::ConfigList(std::string arg) {

    config *conf = config::getInstance();

    for(int i = 0; i < conf->GetDataSize(); ++i) {
        Interface::ConsoleOutput(Language::Get(CMD_CONFIG_LIST).c_str(), conf->GetData(i).c_str());
        for(int j = 1; j < conf->GetDataSize(i); ++j) {
            Interface::ConsoleOutput(Language::Get(CMD_CONFIG_LIST_ITEM).c_str(),conf->GetData(i,j).c_str(), i, j, (j == conf->GetDataSize(i)-1) ? "\n\n" : "\n");
        }
    }
}

void CommandHandler::hide(std::string arg) {

    Interface::UI *ui = Interface::UI::getInstance();
    
    int pan;
    std::stringstream argstrm(arg);

    if (!(argstrm>>pan).fail())
        ui->HidePanel(pan);
}

void CommandHandler::UnitEditName(std::string arg) {

    UnitStore *us = UnitStore::getInstance();
    Unit *u = us->ReturnUnit(0);

    u->SetName(arg);
}

void CommandHandler::UnitEditAge(std::string arg) {

    UnitStore *us = UnitStore::getInstance();
    Unit *u = us->ReturnUnit(0);

    int age;
    std::stringstream argstrm(arg);

    if (!(argstrm>>age).fail())
        u->SetAge(age);
}

void CommandHandler::UnitEditSex(std::string arg) {

    UnitStore *us = UnitStore::getInstance();
    Unit *u = us->ReturnUnit(0);

    int sex;
    std::stringstream argstrm(arg);

    if (!(argstrm>>sex).fail())
        u->SetSex(sex);
}
void CommandHandler::UnitEditOcc(std::string arg) {

    UnitStore *us = UnitStore::getInstance();
    Unit *u = us->ReturnUnit(0);

    int occ;
    std::stringstream argstrm(arg);

    if (!(argstrm>>occ).fail())
        u->SetOcc(occ);
}

void CommandHandler::GetConfig(std::string arg) {

    config *conf = config::getInstance();

    if(!conf->GetVariable(arg).empty()) {
        for(int i = 1; i < conf->GetDataSize(conf->GetLine(arg)); ++i)
            Interface::ConsoleOutput(Language::Get(CMD_CONFIG_GET).c_str(), arg.c_str(), i, conf->GetVariable(arg,i).c_str());
    }
    else
        TheLife::Error(Language::Get(ERROR_WRONG_CMD_CONFIG).c_str(), arg.c_str());
}

void CommandHandler::ConfigAddConfig(std::string var) {

    config *conf = config::getInstance();

    std::string path = conf->GetVariable("ldpath");

    if(path.empty()) {
        path = ".";
    }

    std::string file = path;
    file += "/";
    file += "life.config";

    conf->WriteToConfig(file, var);
}

void CommandHandler::ConfigAddComment(std::string com) {

    config *conf = config::getInstance();

    std::string path = conf->GetVariable("ldpath");

    if(path.empty()) {
        path = ".";
    }

    std::string file = path;
    file += "/";
    file += "life.config";

    conf->WriteToConfig(file, "# " + com);
}

// CommandProcess is processing command request
void const CommandHandler::CommandProcess(char *buffer)  {

    config *conf = config::getInstance();
    std::string output = buffer;
    std::string trigger = "/";
    std::string f;

    // Here we separate to load command with "/" from command table or look for config with "-"
    if(output.at(0) == trigger.at(0))
        f = strtok(buffer, "/");

    // Execute one of types if none found report how normal command starts
    if(output.at(0) == trigger.at(0))
    {
        LookupCommand(commands(), f);
    }
    else
    {
        Interface::ConsoleOutput(Language::Get(CMD_PROCESS).c_str());
    }
}
