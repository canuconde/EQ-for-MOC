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

#define PACKAGE_NAME "eq4moc - EQ for MOC"
#define PACKAGE_VERSION "1.0"
#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "eqFs.h"
#include "eqSet.hpp"
#include "ncFunctions.h"
#include "mTheme.hpp"

using namespace std;

int main(int argc, char *argv[])
{

    string moc_dir = ".moc/eqsets";
    int selectedeq=0, selected=0;
    vector<eqSet> eqSets;
    string user_dir=getenv("HOME");
    mTheme moc_theme;



    WINDOW *main_win, *control[10], *help;
    int ch;
    const string band_freq[10]={"60 ","170","310","600","1 K","3 K","6 K","12K","14K","16K"};
    const string ex_band_freq[10]={"60 Hz","170 Hz","310 Hz","600 Hz","1 kHz","3 kHz","6 kHz","12 kHz","14 kHz","16 kHz"};

    for(int i=1; i < argc ; i++)
	{
			if(string(argv[i])=="-v" || string(argv[i])=="--version"){
				cout << "\n"
                     << "          This is : " << PACKAGE_NAME << "\n"
                     << "          Version : " << PACKAGE_VERSION << "\n"
                     << "        Copyright : (C) 2024 Sebastián Bergara\n"
                     << "          License : GNU General Public License, version 3 or later\n"
                << endl;
				return 0;
			}else if(string(argv[i])=="-t" || string(argv[i])=="-T" || string(argv[i])=="--theme"){
                if(!argv[i+1]) return 1;
                if(!moc_theme.chgTheme(argv[i+1])){
                        cout << "\t Theme file not found." <<endl;
                        return 1;
                }

            }
	}
	// if (!setlocale(LC_ALL, ""))
	// 	cout << "Could not set locale!" << endl;
    if(user_dir==""){
        cout<<"Could not determine user's home directory!"<<endl;
        return 1;
    }
    moc_dir=path(user_dir)/path(moc_dir);
    if(!exists(path(moc_dir))){
        create_directories(moc_dir);
       }
    eq_search_files(eqSets,moc_dir);
    for(unsigned int i=0;i<eqSets.size(); i++){
        if(eqSets[i].isactive==true){
            selectedeq=i;
        }
    }
    eqSets.shrink_to_fit();

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    refresh();
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

    main_win = create_mainwin(LINES, COLS, 0, 0,eqSets[selectedeq]);

    for(int i=0;i<10;i++){
        control[i] = create_eqbar(LINES-5, 5, 2, COLS*1/10*i+COLS*1/25,
                                  band_freq[i], eqSets[selectedeq].getbandvalue(i+1) );
     }

    clean_control(control[selected]);
    wattron(control[selected], A_BLINK  | A_REVERSE );
    update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
    wattroff(control[selected], A_BLINK | A_REVERSE  );
    clean_window(main_win);
    update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
    help = newwin(LINES/2,COLS/2,LINES/4,COLS/4);

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
            case 'h':
            case 'H':
                    box(help,0,0);
                    wrefresh(help);
            break;
            case 'e':
            case 'E':
                    system("echo 'eq' | mocp ; clear"); // <-- TODO: NO borrar esto!!! lo necesitamos para refrescar moc
                    cbreak();
                    noecho();
                    curs_set(0);
                    keypad(stdscr, TRUE);
                    werase(stdscr);
                    wrefresh(stdscr);
                    destroy_win(main_win);
                    main_win = create_mainwin(LINES, COLS, 0, 0,eqSets[selectedeq]);
                    for(int i=0;i<10;i++){
                    destroy_win(control[i]);
                        control[i] = create_eqbar(LINES-5, 5, 2, COLS*1/10*i+COLS*1/25,
                                           band_freq[i], eqSets[selectedeq].getbandvalue(i+1) );
                    }
            break;
            case KEY_RESIZE:
                // TODO Usar wresize() y mvwin()
                werase(stdscr);
                wrefresh(stdscr);
                destroy_win(main_win);
                main_win = create_mainwin(LINES, COLS, 0, 0,eqSets[selectedeq]);
                for(int i=0;i<10;i++){
                    destroy_win(control[i]);
                     control[i] = create_eqbar(LINES-5, 5, 2, COLS*1/10*i+COLS*1/25,
                                           band_freq[i], eqSets[selectedeq].getbandvalue(i+1) );
                }

            break;
            case '\t':
            case 'k':
                selectedeq++;
                if(selectedeq == eqSets.size()) selectedeq=0;
                for(int i=0;i<10;i++){
                    clean_control(control[i]);
                    update_control(control[i],eqSets[selectedeq].getbandvalue(i+1));
                }
                use_default_colors();
            break;
            case 'K':
                selectedeq--;
                if(selectedeq < 0) selectedeq=eqSets.size()-1;
                for(int i=0;i<10;i++){
                    clean_control(control[i]);
                    update_control(control[i],eqSets[selectedeq].getbandvalue(i+1));
                }
            break;
            case 'q':
            case 'Q':
                for(int i=0;i<10;i++){
                    delwin(control[i]);

                }
                delwin(main_win);
                refresh();
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
