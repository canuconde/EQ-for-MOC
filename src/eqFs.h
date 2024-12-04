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
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include "eqSet.h"

#define THEMESDIR "/usr/share/moc/themes/"
#define THEMESDIRLOCAL "/usr/local/share/moc/themes/"
#define MOCCONFIGFILE ".moc/config"


using namespace std;
using namespace std::filesystem;

bool moc_search_active_theme(path &themefile){
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
        themefile=value;
        return true;
    }
    return false;
}

bool moc_load_active_theme(path &theme){
    string key, eq, value1, value2;
    path configDir=THEMESDIR;
    configDir=configDir/theme;
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
    }
    configFile.close();
    // if(eq=="=" && value.size()>0){
    //     themefile=value;
    //     return true;
    // }
    return false;
}

bool eq_search_files(vector<eqSet> &files, const string &_path){
    string line;
    string active;
    string range, value;
    string defaultfile=path(_path)/"EQ_4_MOC";
    if(!exists(path(defaultfile))){
        eqSet defaultset(defaultfile);
        files.push_back(defaultset);
        files.back().save();
    }
    cout << path(_path).parent_path()/"equalizer";
    if(exists(path(_path).parent_path()/"equalizer")){

        fstream tmpFile(path(_path).parent_path()/"equalizer");
        if(tmpFile.is_open()){
                while(!tmpFile.eof()){
                    tmpFile >> line >> value;
                    if(line=="Preset:") active=value;
                }
            tmpFile.close();
        }
    }

    if(!exists(path(_path))){
        return false;
    }
eqSet *tmpeqSet2;
    for (directory_entry const& it : directory_iterator(_path)){
	                if (it.is_regular_file()){
                        tmpeqSet2 = new eqSet(it.path().string());

                        files.push_back(*tmpeqSet2);

                        fstream tmpFile(it.path());

                        if(!tmpFile.is_open()){

                            files.pop_back();
                            continue;
                        }
                        int i=0;
                        while(!tmpFile.eof()){
                            tmpFile >> line;
                            if(i==11) break; //SI EL ARCHIVO TIENE MAS DE 10BANDAS DEJAMOS DE LEER
                            // SINO PROVOCAMOS CRASH AL HACER PUSH_BACK
                            if(line[0]=='#' || line=="EQSET") {
                                getline(tmpFile,line,'\n');
                                continue;
                            }
                            if(line[0]=='0') {
                                tmpFile >> range;
                                files.back().setbandvalue(i,stof(range));
                                files.back().unsaved=false;
                                i++;
                                getline(tmpFile,line,'\n');
                                continue;
                            }
                            if(it.path().filename().string()==active) files.back().isactive=true;
                            tmpFile >> range >> value;
                            files.back().setbandvalue(i,stof(value));
                            cout << i<<"\t" <<line <<"\t" <<range<<"\t" <<value << endl;
                            files.back().unsaved=false;
                            i++;
                        }
                        tmpFile.close();

                    }

    }
    // files.push_back(defaultset);

    return true;
}

