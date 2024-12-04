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
#include <string>
#include <vector>
#include <filesystem>


#define THEMESDIR "/usr/share/moc/themes/"
#define THEMESDIRLOCAL "/usr/local/share/moc/themes/"
#define MOCCONFIGFILE ".moc/config"

using namespace std;
using namespace std::filesystem;


class mTheme{
private:
    path Theme;
    bool loadActiveTheme();
    bool searchActiveTheme();
                        //  Tema por defecto
                        // De NCURSES.h
                        // #define COLOR_BLACK	0
                        // #define COLOR_RED	1
                        // #define COLOR_GREEN	2
                        // #define COLOR_YELLOW	3
                        // #define COLOR_BLUE	4
                        // #define COLOR_MAGENTA	5
                        // #define COLOR_CYAN	6
                        // #define COLOR_WHITE	7
    int colorPair[6][2]={   7,  4,
                            7,  4,
                            7,  4,
                            7,  4,
                            7,  4,
                            7,  4   };
    int colorToInt(const string &colorValue);
public:
    mTheme();
    int getColorPair(const string &element,int colorIndex);

};


mTheme::mTheme(){
    if(searchActiveTheme()){
        loadActiveTheme();
    }
}
        // # FOREGOUND_COLOR and BACKGROUND_COLOR can have one of the following values:
        // # black, red, green, yellow, blue, magenta, cyan, white, default (can be
        // # transparent), grey (not standard, but works)

int mTheme::colorToInt(const string &colorValue){
    if(colorValue=="black"){
        return 0;
    }else  if(colorValue=="black"){
        return 0;
    }else  if(colorValue=="red"){
        return 1;
    }else  if(colorValue=="green"){
        return 2;
    }else  if(colorValue=="yellow"){
        return 3;
    }else  if(colorValue=="blue"){
        return 4;
    }else  if(colorValue=="magenta"){
        return 5;
    }else  if(colorValue=="cyan"){
        return 6;
    }else   if(colorValue=="white"){
        return 7;
    }else  if(colorValue=="default"){
        return 0;
    }
    return -1;
}

bool mTheme::loadActiveTheme(){
    string key, eq, value1, value2;
    path configDir=THEMESDIR;
    configDir=configDir/Theme;
    if(!exists(path(configDir))) return false;
    ifstream configFile(configDir);
    if (!configFile.is_open()) return false;
    while(!configFile.eof()){
        configFile >> key;
        if(key[0]=='#'){
            getline(configFile,key,'\n');
            continue;
        }
        configFile >> eq >> value1 >> value2 ;
        getline(configFile,eq,'\n');
        cout << key << "\t" << value1<< "\t" << value2 <<endl;

        if(key=="window_title"){
                colorPair[0][0]=colorToInt(value1);
                colorPair[0][1]=colorToInt(value2);
        }else if(key=="enabled"){
                colorPair[1][0]=colorToInt(value1);
                colorPair[1][1]=colorToInt(value2);
        }else if(key=="disabled"){
                colorPair[2][0]=colorToInt(value1);
                colorPair[2][1]=colorToInt(value2);
        }else if(key=="info"){
                colorPair[3][0]=colorToInt(value1);
                colorPair[3][1]=colorToInt(value2);
        }else if(key=="title"){
                colorPair[4][0]=colorToInt(value1);
                colorPair[4][1]=colorToInt(value2);
        }else if(key=="frame"){
                colorPair[5][0]=colorToInt(value1);
                colorPair[5][1]=colorToInt(value2);
        }


    }
    configFile.close();
    return false;
}

bool mTheme::searchActiveTheme(){
    string key, eq, value;
    path configDir=getenv("HOME");
    configDir=configDir/MOCCONFIGFILE;
    if(!exists(path(configDir))) return false;
    fstream configFile(configDir);
    if (!configFile.is_open()) return false;
    while(!configFile.eof()){
        configFile >> key;
        if(key!="Theme"){
            getline(configFile,key,'\n');
            continue;
        }
        configFile >> eq >> value;
        break;
    }
    configFile.close();
    if(eq=="=" && value.size()>0){
        Theme=value;
        return true;
    }
    return false;
}
int mTheme::getColorPair(const string &element,int colorIndex=0){
    if(element=="window_title") return colorPair[0][colorIndex];
    if(element=="enabled") return colorPair[1][colorIndex];
    if(element=="disabled") return colorPair[2][colorIndex];
    if(element=="info") return colorPair[3][colorIndex];
    if(element=="title") return colorPair[4][colorIndex];
    if(element=="frame") return colorPair[5][colorIndex];
    return -1;
}
