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
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include "eqSet.hpp"
#include "eqPresets.hpp"


using namespace std;
using namespace std::filesystem;

eqPresets::eqPresets(const string &setsPath){
    eqSetsDirectory = path(setsPath);
    searchActive();
    createDefaultFile();
    loadEqFiles();
}

bool eqPresets::isOn(){
    return eqIsOn;
}

string eqPresets::getActive(){
    return activeEq;
}

bool eqPresets::createDefaultFile(){
    string defaultfile=eqSetsDirectory/"EQ_4_MOC";
    if(!exists(path(defaultfile))){
        eqSet defaultset(defaultfile);
        set.push_back(defaultset);
        set.back().save();
        return true;
    }
    return false;
}

bool eqPresets::searchActive(){
    string line, value;
    if(exists(eqSetsDirectory.parent_path()/"equalizer")){
        fstream tmpFile(eqSetsDirectory.parent_path()/"equalizer");
        if(tmpFile.is_open()){
                while(!tmpFile.eof()){
                    tmpFile >> line >> value;
                    if(line=="Active:") eqIsOn=true;
                    if(line=="Preset:") activeEq=value;
                }
            tmpFile.close();
            return true;
        }
    }
    return false;
}

bool eqPresets::loadEqFiles(){
    string line, range, value;
    if(!exists(eqSetsDirectory)){
        return false;
    }
   for (directory_entry const& it : directory_iterator(eqSetsDirectory)){
            if (it.is_regular_file()){
                eqSet tmpeqSet2(it.path().string());
                set.push_back(tmpeqSet2);
                fstream tmpFile(it.path());
                if(!tmpFile.is_open()){
                    set.pop_back();
                    continue;
                }
                int i=0;
                while(!tmpFile.eof()){
                    tmpFile >> line;
                    if(i==11) break; //SI EL ARCHIVO TIENE MAS DE 10BANDAS DEJAMOS DE LEER
                    if(line[0]=='#' || line=="EQSET") {
                        getline(tmpFile,line,'\n');
                        continue;
                    }
                    if(line[0]=='0') {
                        tmpFile >> range;
                        set.back().setbandvalue(i,stof(range));
                        set.back().unsaved=false;
                        i++;
                        getline(tmpFile,line,'\n');
                        continue;
                    }
                    if(it.path().filename().string()==activeEq) set.back().isactive=true;
                    tmpFile >> range >> value;
                    set.back().setbandvalue(i,stof(value));
                    set.back().unsaved=false;
                    i++;
                }
                tmpFile.close();
            }
    }
    return true;
}


