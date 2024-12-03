#include <ncurses.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "eqFs.h"
#include "eqSet.h"

using namespace std;
WINDOW *create_eqbar(int height, int width, int starty, int startx, const string &label, float value);
WINDOW *create_mainwin(int height, int width, int starty, int startx, eqSet &local_eqSet);
void update_control(WINDOW *local_win, float value);
void update_window(WINDOW *local_win, eqSet &local_eqSet, string exband="", float value=0);
void clean_control(WINDOW *local_win);
void clean_window(WINDOW *local_win);
void destroy_win(WINDOW *local_win);
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
    col_height=height*6/8;
    col_width=5;
    col_starty=height*1/8;
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
                clean_control(control[selected]);
                wattron(control[selected], A_BLINK | A_REVERSE );
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                wattroff(control[selected], A_BLINK | A_REVERSE  );

                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
            break;
            case KEY_RIGHT:
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                if(selected==9) selected=0; else selected++;
                clean_control(control[selected]);
                wattron(control[selected], A_BLINK | A_REVERSE  );
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                wattroff(control[selected], A_BLINK | A_REVERSE );

                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
            break;
            case KEY_UP:
                test=eqSets[selectedeq].getbandvalue(selected+1);
                if(test<19.5)  test++;
                eqSets[selectedeq].setbandvalue(selected+1,test);
                clean_control(control[selected]);
                wattron(control[selected], A_BLINK | A_REVERSE );
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                wattroff(control[selected], A_BLINK | A_REVERSE  );
                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
            break;
            case KEY_DOWN:
                test=eqSets[selectedeq].getbandvalue(selected+1);
                if(test>-20) test--;
                eqSets[selectedeq].setbandvalue(selected+1,test);
                clean_control(control[selected]);
                wattron(control[selected], A_BLINK  | A_REVERSE );
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                wattroff(control[selected], A_BLINK | A_REVERSE  );
                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
            break;
            case 's':
                eqSets[selectedeq].save();
                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq]);
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
                    control[i] = create_eqbar(col_height, col_width, col_starty , col_startx*i+col_width/2,
                                            band_freq[i], eqSets[selectedeq].getbandvalue(i+1) );
                }


                 clean_control(control[selected]);
                wattron(control[selected], A_BLINK  | A_REVERSE );
                update_control(control[selected],eqSets[selectedeq].getbandvalue(selected+1));
                wattroff(control[selected], A_BLINK | A_REVERSE  );
                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
            break;
            case 'k':
                selectedeq++;
                if(selectedeq == eqSets.size()) selectedeq=0;

                //update_window(main_win,eqSets[selectedeq]);
                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
                for(int i=0;i<10;i++){
                    clean_control(control[i]);
                    update_control(control[i],eqSets[selectedeq].getbandvalue(i+1));
                }
            break;
            case 'K':
                selectedeq--;
                if(selectedeq < 0)  selectedeq=eqSets.size()-1;
                clean_window(main_win);
                update_window(main_win,eqSets[selectedeq],ex_band_freq[selected],eqSets[selectedeq].getbandvalue(selected+1));
                for(int i=0;i<10;i++){
                    clean_control(control[i]);
                    update_control(control[i],eqSets[selectedeq].getbandvalue(i+1));
                }

            break;
        }

    }
    endwin();
    return 0;
}

WINDOW *create_eqbar(int height, int width, int starty, int startx,const string &label, float value){
    WINDOW *local_win;
 //   init_pair(1, COLOR_WHITE, COLOR_BLUE);
    local_win = newwin(height, width, starty, startx);
    wattron(local_win,COLOR_PAIR(4) | A_BOLD );
    mvwprintw(local_win,1,1,label.data());
    wattroff(local_win,COLOR_PAIR(4) | A_BOLD );

    value=value+20; // Value es un nº entre -20 y 20
    int percent;
    percent = static_cast<int>(value*100/40);
    clean_control(local_win); // Usamos clean en este caso para pintar
   // wattron(local_win,COLOR_PAIR(1) |  A_BLINK | A_STANDOUT );
    wattron(local_win,COLOR_PAIR(5) | A_BOLD);
    for(int i=0;i<percent*(height-4)/100;i++){
        mvwaddch(local_win,height-i-3,1,ACS_CKBOARD);
        mvwaddch(local_win,height-i-3,2,ACS_CKBOARD);
        mvwaddch(local_win,height-i-3,3,ACS_CKBOARD);
    }
    wattroff(local_win,COLOR_PAIR(5) | A_BOLD);
    // wattroff(local_win,COLOR_PAIR(1)|  A_BLINK | A_STANDOUT);
    wattron(local_win,COLOR_PAIR(4) | A_BOLD);
    mvwprintw(local_win,height-2,1,"%d%%",percent);
    wattroff(local_win,COLOR_PAIR(4) | A_BOLD);

    box(local_win, 0, 0);           /* 0, 0 pone los caracteres por
                  * defecto para las lineas vertical*/
  //  mvwhline(local_win,2, 1, 0,3); // Linea en y=2 x=0 de tipo 0 (defecto) y longitud 3
    wrefresh(local_win);            /* Muestra la caja                    */
return local_win;
}

WINDOW *create_mainwin(int height, int width, int starty, int startx,eqSet &local_eqSet){
    WINDOW *local_win;
  //  init_pair(1, COLOR_WHITE, COLOR_BLUE);
    local_win = newwin(height, width, starty, startx);

    box(local_win, 0, 0);
     string title = string("EQ for MOC").data();

    wmove(local_win,0,width/2-(title.size()+2)/2);
    wprintw(local_win,"|");
    wattron(local_win, COLOR_PAIR(1) | A_BOLD );
    wprintw(local_win,title.data());
    wattroff(local_win, COLOR_PAIR(1) | A_BOLD );
    wprintw(local_win,"|");
    //wattrset(local_win,COLOR_PAIR(1)); //Pinta todo menos el espacio vacio



   // mvwprintw(local_win,height-2,width*3/5,string("[Active] [Enable] [Unsaved]").data());
    for(int i=0;i<height-2;i++){ // Pintamos el espacio vacio
      //  mvwchgat(local_win,i+1,1,width-2, A_NORMAL,1,NULL);
    }
    update_window(local_win,local_eqSet);
    wrefresh(local_win);
return local_win;
}
void update_window(WINDOW *local_win, eqSet &local_eqSet, string exband,float value){
    int width, height;
    getmaxyx(local_win,height,width);
    string db;

        db=" [ ";
        db+=to_string(value);
        db=db.substr(0, db.find(".")+3);
        db+="dB ] ";

        wmove(local_win,height-3,width*1/10);
        wprintw(local_win,"EQ presset: ");
        wattron(local_win, COLOR_PAIR(1) | A_BOLD );
        wprintw(local_win,string("<"+local_eqSet.name+">").data());
        wattroff(local_win, COLOR_PAIR(1) | A_BOLD );

        wmove(local_win,height-2,width*1/10);
        wprintw(local_win,exband.data());
        wattron(local_win, COLOR_PAIR(1) | A_BOLD );
        wprintw(local_win,db.data());
        wattroff(local_win, COLOR_PAIR(1) | A_BOLD );
    if(local_eqSet.isactive==true){
        wattron(local_win, COLOR_PAIR(2) | A_BOLD );
        mvwprintw(local_win,height-2,width*6/10,string("[ACTIVE]").data());
        wattroff(local_win, COLOR_PAIR(2) | A_BOLD );
    }else{
        wattron(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM );
        mvwprintw(local_win,height-2,width*6/10,string("[UNACTIVE]").data());
        wattroff(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM );
    }
    if(local_eqSet.unsaved==true){
        wattron(local_win, COLOR_PAIR(2) | A_BOLD );
        mvwprintw(local_win,height-3,width*6/10,string("[UNSAVED]").data());
        wattroff(local_win, COLOR_PAIR(2) | A_BOLD );
    }else{
        wattron(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
        mvwprintw(local_win,height-3,width*6/10,string("[SAVED]").data());
        wattroff(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
    }

    wrefresh(local_win);
}
void clean_window(WINDOW *local_win){
    int width, height;
    getmaxyx(local_win,height,width);
        for(int j=1;j<width-1;j++){
          mvwprintw(local_win,height-2,j," ");
          mvwprintw(local_win,height-3,j," ");
        }

   // wrefresh(local_win);
}
void update_control(WINDOW *local_win, float value){
    value=value+20; // Value es un nº entre -20 y 20
    int percent;
    int width, height;
    getmaxyx(local_win,height,width);
    percent = static_cast<int>(value*100/40);

    wattron(local_win,COLOR_PAIR(5) | A_BOLD);
    for(int i=0;i<percent*(height-4)/100;i++){
          mvwaddch(local_win,height-i-3,1,ACS_CKBOARD);
          mvwaddch(local_win,height-i-3,2,ACS_CKBOARD);
          mvwaddch(local_win,height-i-3,3,ACS_CKBOARD);
      }
    wattroff(local_win,COLOR_PAIR(5) | A_BOLD);
    if(percent==100) percent=99;
    wattron(local_win,COLOR_PAIR(4) | A_BOLD);
    mvwprintw(local_win,height-2,1,"%d%%",percent);
    wattroff(local_win,COLOR_PAIR(4) | A_BOLD);
   // mvwhline(local_win,2, 1, 0,3);
    wrefresh(local_win);
}
void clean_control(WINDOW *local_win){
    int width, height;
    getmaxyx(local_win,height,width);
    for(int i=0;i<height-3;i++){
          mvwprintw(local_win,height-i-2,1," ");
          mvwprintw(local_win,height-i-2,2," ");
          mvwprintw(local_win,height-i-2,3," ");
      }
    //wrefresh(local_win);
}
void destroy_win(WINDOW *local_win){
  wrefresh(local_win);
  delwin(local_win);
}
