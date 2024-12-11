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

using namespace std;
using namespace std::filesystem;

class mTheme{
private:
    path Theme;
    bool loadActiveTheme();
    bool searchActiveTheme();
    const map<string, int> tMap= {
                            {"black", 0},
                            {"red",1},
                            {"green",2},
                            {"yellow",3},
                            {"blue",4},
                            {"magenta",5},
                            {"cyan",6},
                            {"white",7},
                            {"default",0}
    };

     const map<string, int> cMap= {
                            {"window_title", 0},
                            {"enabled",1},
                            {"disabled",2},
                            {"info",3},
                            {"title",4},
                            {"frame",5}
    };
                                        //   Tema por defecto
    int colorPair[6][2]={   7,  4,      //  --> NCURSES.h <--
                            7,  4,      // #define COLOR_BLACK	0
                            7,  4,      // #define COLOR_RED	1
                            7,  4,      // #define COLOR_GREEN	2
                            7,  4,      // #define COLOR_YELLOW	3
                            7,  4       // #define COLOR_BLUE	4
                                };      // #define COLOR_MAGENTA	5
                                        // #define COLOR_CYAN	6
                                        // #define COLOR_WHITE	7
    int colorToInt(const string &colorValue);
public:
    mTheme();
    bool chgTheme(const string &_theme);
    int getColorPair(const string &element,int colorIndex=0);

};


