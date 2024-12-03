#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "eqFs.h"
#include "eqSet.h"
#include "ncFunctions.h"

using namespace std;


int main(int argc, char *argv[])
{
    // COSAS DE MOCP //
    string moc_dir = ".moc/eqsets";
    char userinput;
    int selectedeq=0;
    vector<eqSet> eqSets;
    string user_dir=getenv("HOME");
    // COSAS DE NCURSES //
    WINDOW *main_win, *control[10];
    int startx, starty, width, height;
    int col_startx, col_starty, col_width, col_height;
    int ch;
    int selected=0;
    const string band_freq[10]={"60 ","170","310","600","1 K","3 K","6 K","12K","14K","16K"};
    const string ex_band_freq[10]={"60 Hz","170 Hz","310 Hz","600 Hz","1 kHz","3 kHz","6 kHz","12 kHz","14 kHz","16 kHz"};
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
        printf("Tu terminal no soporta color\n");
        return 1;
    }
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // window_title
    init_pair(2, COLOR_BLUE, COLOR_BLACK);  // enabled
    init_pair(3, COLOR_WHITE, COLOR_BLACK); // disabled
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // info
    init_pair(5, COLOR_CYAN, COLOR_BLACK); // title
    init_pair(6, COLOR_WHITE, COLOR_BLACK); // lineas y fondo
    /*
     * Comprobaciones de MOC
     */
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
    //init_pair(1, COLOR_WHITE, COLOR_BLUE);
    //attron(COLOR_PAIR(1));
   printw("Pulse F1 para salir");
    //attroff(COLOR_PAIR(1));
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
        control[i] = create_eqbar(col_height, col_width, col_starty , col_startx*i+col_width/2,
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
                if(test<19.5)  test++;
                eqSets[selectedeq].setbandvalue(selected+1,test);
            break;
            case KEY_DOWN:
                test=eqSets[selectedeq].getbandvalue(selected+1);
                if(test>-20) test--;
                eqSets[selectedeq].setbandvalue(selected+1,test);
            break;
            case 's':
                eqSets[selectedeq].save();
            break;
            case KEY_RESIZE:
                //system("echo 'e' | mocp &"); <-- NO borrar esto!!!
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
                    control[i] = create_eqbar(col_height, col_width, col_starty , col_startx*i+col_width/2,
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

