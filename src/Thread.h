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

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

namespace Thread {

        void *Run(void *arg);
        void *Status(void *arg);

        class cThread {

            protected:
                static cThread* Instance;
                cThread() {}

            private:
                std::vector<pthread_t> tList;

            public:
                void AddThread(pthread_t thread) { this->tList.push_back(thread); }
                int ListThread() { return this->tList.size(); }
                virtual pthread_t GetThread(int i) { return tList[i]; }

                static cThread* getInstance();
        };
};

#endif
