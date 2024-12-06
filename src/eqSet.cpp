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
#include <filesystem>
#include <fstream>
#include "eqSet.hpp"

using namespace std;
using namespace std::filesystem;

eqSet::eqSet(const string &file_path){
    moc_dir=path(file_path).remove_filename();
    name=path(file_path).filename();
}

bool eqSet::save(){
    string tmp_filename;
    tmp_filename = moc_dir;
    tmp_filename.append(name);
    ofstream eqset_file(tmp_filename);
    if(!eqset_file.is_open()) return false;
    eqset_file << "#This file was generated automatically by some eq app for moc." <<endl;
    eqset_file << "EQSET" <<endl;
    eqset_file << band[0].bandfreq << "\t" << setprecision (2) << fixed << band[0].bandwidth << endl;
    for(int i=1; i<11; i++){
        eqset_file << band[i].bandfreq <<"\t" << setprecision (2) << fixed << band[i].bandwidth<<"\t"<< setprecision (2) << fixed <<band[i].bandamp<<endl;
    }
    eqset_file.close();
    unsaved=false;
    return true;
}

bool eqSet::save_as(const string  &new_filename){
    name=new_filename;
    if(!save()) return false;
    return true;
}
bool eqSet::setbandvalue(const int &bandnumber, const float &value){
    band[bandnumber].bandamp=value;
    unsaved=true;
    return true;
}
float eqSet::getbandvalue(const int &bandnumber){
    return band[bandnumber].bandamp;
}
