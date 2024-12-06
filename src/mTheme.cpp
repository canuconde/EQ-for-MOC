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

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "mTheme.hpp"

#define THEMESDIR "/usr/share/moc/themes/"
#define THEMESDIRLOCAL "/usr/local/share/moc/themes/"
#define THEMESDIRUSER ".moc/themes/"
#define MOCCONFIGFILE ".moc/config"

using namespace std;
using namespace std::filesystem;

mTheme::mTheme(){
    if(searchActiveTheme()){
        loadActiveTheme();
    }
}

bool mTheme::chgTheme(const string &_theme){
    Theme=_theme;
    if(!loadActiveTheme()) return false;
     return true;

}

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
    path configDir=getenv("HOME");
    if(exists(path(THEMESDIR/Theme))) {
                configDir=THEMESDIR;
    }else if(exists(path(configDir/THEMESDIRUSER/Theme))) {
                configDir=configDir/THEMESDIRUSER;
    }else{
        return false;
    }
    configDir=configDir/Theme;
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
    return true;
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

int mTheme::getColorPair(const string &element,int colorIndex){
    if(element=="window_title") return colorPair[0][colorIndex];
    if(element=="enabled") return colorPair[1][colorIndex];
    if(element=="disabled") return colorPair[2][colorIndex];
    if(element=="info") return colorPair[3][colorIndex];
    if(element=="title") return colorPair[4][colorIndex];
    if(element=="frame") return colorPair[5][colorIndex];
    return -1;
}
