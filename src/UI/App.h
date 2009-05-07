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
namespace TheLife {

        void LoadLife();
        void UnloadLife();
        int KeyHandle(int key, int keycolor);
        void Debug(std::string debug, ...);
        void Error(std::string error, ...);
        std::vector<std::string> SeparateString(std::string c);
        void SigHandler(int sig);

        class Application {

            protected:
                static Application *Instance;
                Application() {}

            private:
                bool quit;
                bool unload;

            public:
                static Application *getInstance();

                // Application Control Functions
                bool Active() { return quit; }
                void SetActive(bool q) { this->quit = q; }
                
                bool Unload() { return unload; }
                void SetUnload(bool q) { this->unload = q; }
        };
};

#endif
