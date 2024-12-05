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

#define APP_NAME "eq4moc - EQ for MOC"
#define APP_VERSION 0.9

#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "eqFs.h"
#include "eqSet.h"
#include "ncFunctions.h"
#include "mocTheme.h"

using namespace std;

int main(int argc, char *argv[])
{
    // COSAS DE MOCP //
    string moc_dir = ".moc/eqsets";
    char userinput;
    int selectedeq=0;
    vector<eqSet> eqSets;
    string user_dir=getenv("HOME");
    mTheme moc_theme;
    // COSAS DE NCURSES //
    WINDOW *main_win, *control[10];
    int startx, starty, width, height;
    int col_startx, col_starty, col_width, col_height;
    int ch;
    int selected=0;
    const string band_freq[10]={"60 ","170","310","600","1 K","3 K","6 K","12K","14K","16K"};
    const string ex_band_freq[10]={"60 Hz","170 Hz","310 Hz","600 Hz","1 kHz","3 kHz","6 kHz","12 kHz","14 kHz","16 kHz"};

    if(argc>0) //solo si tenemos argumentos
	for(int i=1; i < argc ; i++)
	{
			if(string(argv[i])=="-v" || string(argv[i])=="--version"){
				cout << "          This is : " << APP_NAME << "\n"
                     << "          Version : " << APP_VERSION << "\n"
                     << "        Copyright : (C) 2024 Sebastián Bergara\n"
                     << "          License : GNU General Public License, version 3 or later\n"
                << endl;
				return 0;
			}else{
				return 1;
			}
	}
    /*
     * Inicializamos las cositas de Ncurses
     * TODO: agregar compatibilidad con temas
     *       y con los temas de MOC
     */
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    if(has_colors() == FALSE)
    {
        endwin();
        cout << "Your terminal does not support color\n" << endl;
        return 1;
    }
    start_color();
    init_pair(1, moc_theme.getColorPair("window_title"), moc_theme.getColorPair("window_title",1)); // window_title
    init_pair(2, moc_theme.getColorPair("enabled"), moc_theme.getColorPair("enabled",1)); // enabled
    init_pair(3, moc_theme.getColorPair("disabled"), moc_theme.getColorPair("disabled",1));  // disabled
    init_pair(4, moc_theme.getColorPair("info"), moc_theme.getColorPair("info",1)); // info
    init_pair(5, moc_theme.getColorPair("title"), moc_theme.getColorPair("title",1)); // title
    init_pair(6, moc_theme.getColorPair("frame"), moc_theme.getColorPair("frame",1)); // lineas y fondo
    /*
     * Comprobaciones de MOC
     */
    if (!setlocale(LC_ALL, ""))
		cout << "Could not set locale!" << endl;
    if(user_dir==""){
        cout<<"Could not determine user's home directory!"<<endl;
        return 1;
    }
    moc_dir=path(user_dir)/path(moc_dir);
    if(!exists(path(moc_dir))){
        create_directories(moc_dir);
        // cout<<"No se encontro el directorio de configuracion de moc: \t" << path(moc_dir) <<endl;
        // char q;
        // cout << "¿Desea crear el directorio? (Y/N)" <<endl;
        // cin >> q;
        // if (q=='Y' || q=='y'){
        //     create_directories(moc_dir);
        // }else{
        //     return 1;
        // }
    }
    eq_search_files(eqSets,moc_dir);
    for(int i=0;i<eqSets.size(); i++){
        if(eqSets[i].isactive==true){
            selectedeq=i;
        }
    }
    /*
     * Mostramos un mensaje que no es visible
     * mainwin lo tapa. Queda aca como recordatorio
     * tenemos que mostrar un mensaje cuando el terminal no tiene
     * espacio suficiente para mostrar las ventas de ncurses
     */
    //printw("Pulse F1 para salir");
    refresh();
    /* Creamos las ventanas
     * maiwin es la ventana principal
     * y creamos 10 ventanas mas, una por cada control
     */
    height = LINES;
    width = COLS;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;
    main_win = create_mainwin(height, width, starty, startx,eqSets[selectedeq]);
    col_height=height-5;
    col_width=5;
    col_starty=2;
    col_startx=width*1/10; //la decima parte del disply
    for(int i=0;i<10;i++){
        control[i] = create_eqbar(col_height, col_width, col_starty , col_startx*i+col_startx/2,
                                  band_freq[i], eqSets[selectedeq].getbandvalue(i+1) );
     }

    clean_control(control[selected]);
    wattron(control[selected], A_BLINK  | A_REVERSE );
    update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
    wattroff(control[selected], A_BLINK | A_REVERSE  );
    clean_window(main_win);
    update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));

    float test=0;
    while((ch = getch()) != KEY_F(1))
    {
        switch(ch)
        {
            case KEY_LEFT:
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                if(selected==0) selected=9; else selected--;
            break;
            case KEY_RIGHT:
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                if(selected==9) selected=0; else selected++;

            break;
            case KEY_UP:
                test=eqSets[selectedeq].getbandvalue(selected+1);
                if(test<20)  test+=0.5; else test=20;
                eqSets[selectedeq].setbandvalue(selected+1,test);
            break;
            case KEY_DOWN:
                test=eqSets[selectedeq].getbandvalue(selected+1);
                if(test>-20) test-=0.5; else test=-20;
                eqSets[selectedeq].setbandvalue(selected+1,test);
            break;
            case 's':
            case 'S':
                eqSets[selectedeq].save();
            break;
            case KEY_RESIZE:
                //system("echo 'e' | mocp &"); <-- TODO: NO borrar esto!!! lo necesitamos para refrescar moc
                for(int i=0;i<10;i++){
                    destroy_win(control[i]);

                }
                destroy_win(main_win);
                height = LINES;
                width = COLS;
                starty = (LINES - height) / 2;
                startx = (COLS - width) / 2;
                main_win = create_mainwin(height, width, starty, startx,eqSets[selectedeq]);
                col_height=height-5;
                col_width=5;
                col_starty=2;
                col_startx=width*1/10; //la decima parte del display
                for(int i=0;i<10;i++){
                  //  destroy_win(control[i]); <-- TODO v. seg.
                    control[i] = create_eqbar(col_height, col_width, col_starty , col_startx*i+col_startx/2,
                                            band_freq[i], eqSets[selectedeq].getbandvalue(i+1) );
                }
            break;
            case '\t':
                selectedeq++;
                if(selectedeq == eqSets.size()) selectedeq=0;
                for(int i=0;i<10;i++){
                    clean_control(control[i]);
                    update_control(control[i],eqSets[selectedeq].getbandvalue(i+1));
                }
                use_default_colors();
            break;
            case 'q':
            case 'Q':
                endwin();
                return 0;

            break;
        }
                clean_control(control[selected]);
                wattron(control[selected], A_BLINK | A_REVERSE );
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                wattroff(control[selected], A_BLINK | A_REVERSE  );
                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
    }
    endwin();
    return 0;
}