#include <iostream>
#include <vector>
#include <fstream>
#include "eqFs.h"
#include "eqSet.h"

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

    while(userinput!='q'){
        switch(userinput){
            case    'a':
                        selectedeq++;
                        if(selectedeq == eqSets.size()) selectedeq=0;
                break;
            case    's':
                        selectedeq--;
                        if(selectedeq < 0)  selectedeq=eqSets.size()-1;
                break;
            default:
                break;
        }
        cout << eqSets[selectedeq].name << endl;
        cout << selectedeq << endl;
        for(int i=0;i<11;i++){
            cout << eqSets[selectedeq].getbandvalue(i) << "\t";
            }
        cout <<endl;
        cin >> userinput;
        system("clear");
    }



    return 0;
}
