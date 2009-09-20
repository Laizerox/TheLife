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

#ifndef APP_H
#define APP_H

#include <string>
#include <vector>
#include <ctime>

#include "../Singleton.h"

#define sTheLife TheLife::Application::getInstance()

namespace TheLife {

        void LoadLife();
        void UnloadLife();
        int KeyHandle(int key, int keycolor);
        void Debug(std::string debug, ...);
        void Error(std::string error, ...);
        std::vector<std::string> Explode(std::string delimiter, std::string str, int n = 0, std::string filter = "");
        std::string Implode(std::string limiter, std::vector<std::string> vec, int n = 0);
        void SigHandler(int sig);
        long long millisec_elapsed(struct timespec diff);
        struct timespec diff_timespec(struct timespec start, struct timespec end);
        std::vector<std::string> DirExtension(std::string dir, std::string rexp);
        void CharStr(std::string ch, std::string& str);
        bool CharCheck(std::string ch, std::string str, int n = 0);

        class Application : public Singleton<Application> {

            private:
                bool quit;
                bool unload;
                std::string m_Language;

            public:
                Application() {}

                // Application Control Functions
                bool Active() { return quit; }
                void SetActive(bool q) { this->quit = q; }
                
                bool Unload() { return unload; }
                void SetUnload(bool q) { this->unload = q; }

                std::string ActiveLanguage() { return m_Language; }
                void SetActiveLanguage(std::string lng) { this->m_Language = lng; }
        };
};

#endif
