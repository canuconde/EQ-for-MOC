#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>
#include "eqSet.h"
using namespace std;
using namespace std::filesystem;

bool eq_search_files(vector<eqSet> &files, const string &_path){
    string line;
    string active;
    string range, value;
    string defaultfile=path(_path)/"MOC_Def_EQ";
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

