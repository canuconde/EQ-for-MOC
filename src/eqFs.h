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

using namespace std;
using namespace std::filesystem;

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
   for (directory_entry const& it : directory_iterator(_path)){
	                if (it.is_regular_file()){
                        eqSet tmpeqSet2(it.path().string());
                        files.push_back(tmpeqSet2);
                        fstream tmpFile(it.path());
                        if(!tmpFile.is_open()){
                            files.pop_back();
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
                                files.back().setbandvalue(i,stof(range));
                                files.back().unsaved=false;
                                i++;
                                getline(tmpFile,line,'\n');
                                continue;
                            }
                            if(it.path().filename().string()==active) files.back().isactive=true;
                            tmpFile >> range >> value;
                            files.back().setbandvalue(i,stof(value));
                            files.back().unsaved=false;
                            i++;
                        }
                        tmpFile.close();
                    }
    }
    return true;
}

