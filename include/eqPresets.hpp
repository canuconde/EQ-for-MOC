//                     eq4moc - EQ for MOC
//     Copyright (C) 2024 Sebastián Bergara <canu.conde@gmail.com>
//
// This file is part of eq4moc (EQ 4 MOC).
//
// eq4moc (EQ 4 MOC) is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// eq4moc (EQ 4 MOC) is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with eq4moc (EQ 4 MOC).
// If not, see <https://www.gnu.org/licenses/>.
#pragma once
#include "eqSet.hpp"

using namespace std;
using namespace std::filesystem;

class eqPresets {
private:
    string activeEq;
    bool eqIsOn;
    path eqSetsDirectory;
    bool loadEqFiles();
    bool createDefaultFile();
    bool searchActive();
public:
    eqPresets(const string &path);
    bool isOn();
    string getActive();
    vector<eqSet> set;
};
