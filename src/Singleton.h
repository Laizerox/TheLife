/* 
 * Copyright (C) 2008 TheLife
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

#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton
{
    public:
        static T* getInstance() {
            if(instance == NULL) instance = new T();
            return instance;
         }

        static void destroy() {
            delete instance;
            instance = NULL;
        }

    protected:
        static T* instance;
        Singleton() {}
        virtual ~Singleton() {}

    private:
        Singleton(const Singleton<T>&);
};

template<typename T> T* Singleton<T>::instance = NULL;
#endif
