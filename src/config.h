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

#ifndef CONFIG_H
#define CONFIG_H

class config {

    protected:
        static config*  Instance;
        config() {}

    private:
        std::vector<std::vector <std::string> > data;

    public:
        static config* getInstance();

        // Conf reading
        virtual std::string GetVariable(std::string look, int pos = 1);
        virtual int GetDataSize(int slot = -1) { return (slot > -1) ? data[slot].size() : data.size(); }
        virtual std::string GetData(int i, int j = 0)  { return data[i][j]; }
        virtual int GetLine(std::string look);

        // Conf loading&saving
        void LoadConf(std::string file, config *conf);
        void CreateDefaultConfig(std::string file, config *conf);
        void EditConfig(std::string file, std::string name);
        bool WriteToConfig(std::string file, std::string variable);
};

#endif
