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
#include <fstream>
#include "eqFs.h"
#include "eqSet.h"
#include "mocTheme.h"
#define THEMESDIR "/usr/share/moc/themes/"
#define THEMESDIRLOCAL "/usr/local/share/moc/themes/"
#define MOCCONFIGFILE ".moc/config"
using namespace std;
using namespace std::filesystem;

int main(){
    string moc_dir = ".moc/eqsets";
    char userinput;
    int selectedeq=0;
    vector<eqSet> eqSets;
    string user_dir=getenv("HOME");
    if(user_dir==""){
        cout<<"No se pudo encontrar el directorio HOME del usuario acutal\n¿La variable de entorno no es accesible?"<<endl;
        return 1;
    }
    moc_dir=path(user_dir)/path(moc_dir);
    if(!exists(path(moc_dir))){
        cout<<"No se encontro el directorio de configuracion de moc: \t" << path(moc_dir) <<endl;
        char q;
        cout << "¿Desea crear el directorio? (S/N)" <<endl;
        cin >> q;
        if (q=='S' || q=='s'){
            create_directories(moc_dir);
        }else{
            return 1;
        }
    }



    eq_search_files(eqSets,moc_dir);
    cout << "se encontraron: "<<endl;
    cout << eqSets.size() <<endl;
    for(eqSet &set : eqSets){
        if(set.isactive==true){
            cout << set.name << endl;
            for(int i=0;i<11;i++){
                cout << set.getbandvalue(i) << "\t";
                }
                cout <<endl;
        }
    }

    path theme;
    moc_search_active_theme(theme);
    cout << theme.string() << endl;
    moc_load_active_theme(theme);
    // while(userinput!='q'){
    //     switch(userinput){
    //         case    'a':
    //                     selectedeq++;
    //                     if(selectedeq == eqSets.size()) selectedeq=0;
    //             break;
    //         case    's':
    //                     selectedeq--;
    //                     if(selectedeq < 0)  selectedeq=eqSets.size()-1;
    //             break;
    //         default:
    //             break;
    //     }
    //     cout << eqSets[selectedeq].name << endl;
    //     cout << selectedeq << endl;
    //     for(int i=0;i<11;i++){
    //         cout << eqSets[selectedeq].getbandvalue(i) << "\t";
    //         }
    //     cout <<endl;
    //     cin >> userinput;
    //     system("clear");
    // }

    mTheme moc_theme;
    cout << moc_theme.getColorPair("window_title") << "\n" << moc_theme.getColorPair("window_title",1) << endl ;

    return 0;
}
