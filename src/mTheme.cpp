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
#include <map>

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
    return tMap.at(colorValue);
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
        if(cMap.count(key)){
            colorPair[cMap.at(key)][0]=colorToInt(value1);
            colorPair[cMap.at(key)][1]=colorToInt(value2);
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
