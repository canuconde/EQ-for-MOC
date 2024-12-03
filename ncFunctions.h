#pragma once

#include <ncurses.h>
#include <string>
#include "eqSet.h"

using namespace std;

WINDOW *create_eqbar(int height, int width, int starty, int startx, const string &label, float value);
WINDOW *create_mainwin(int height, int width, int starty, int startx, eqSet &local_eqSet);
void update_control(WINDOW *local_win, float value);
void update_window(WINDOW *local_win, eqSet &local_eqSet, string exband="", float value=0);
void clean_control(WINDOW *local_win);
void clean_window(WINDOW *local_win);
void destroy_win(WINDOW *local_win);

WINDOW *create_eqbar(int height, int width, int starty, int startx,const string &label, float value){
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    wbkgd(local_win, COLOR_PAIR(6));
    wattron(local_win,COLOR_PAIR(4) | A_BOLD );
    mvwprintw(local_win,1,1,label.data());
    wattroff(local_win,COLOR_PAIR(4) | A_BOLD );
    update_control(local_win,value);
    box(local_win, 0, 0);
  //  mvwhline(local_win,2, 1, 0,3); // Linea en y=2 x=0 de tipo 0 (defecto) y longitud 3
    wrefresh(local_win);
return local_win;
}

WINDOW *create_mainwin(int height, int width, int starty, int startx,eqSet &local_eqSet){
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
     wbkgd(local_win, COLOR_PAIR(6));
    box(local_win, 0, 0);
    string title = string("EQ for MOC").data();
    wmove(local_win,0,width/2-(title.size()+2)/2);
    wprintw(local_win,"|");
    wattron(local_win, COLOR_PAIR(1) | A_BOLD );
    wprintw(local_win,title.data());
    wattroff(local_win, COLOR_PAIR(1) | A_BOLD );
    wprintw(local_win,"|");
    /*  NO BORRAR ESTO
     * wattrset(local_win,COLOR_PAIR(1)); //Pinta todo menos el espacio vacio
    for(int i=0;i<height-2;i++){ // Pintamos el espacio vacio
        mvwchgat(local_win,i+1,1,width-2, A_NORMAL,1,NULL);
    }
    */
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
