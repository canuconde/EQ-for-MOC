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

#pragma once

#include <ncurses.h>
#include <string>
#include <sstream>
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
    update_window(local_win,local_eqSet);
    wrefresh(local_win);
return local_win;
}

void update_window(WINDOW *local_win, eqSet &local_eqSet, string exband,float value){
    int width, height;
    getmaxyx(local_win,height,width);
    string db;
    stringstream strValue;
    strValue << fixed << setprecision(2) << value;

    db=" [ ";
    db+=strValue.str();
    db+=" dB ] ";

    wmove(local_win,height-3,width*1/10);
    wprintw(local_win,"EQ preset: ");
    wattron(local_win, COLOR_PAIR(1) | A_BOLD );
    wprintw(local_win,string("<"+local_eqSet.name+">").data());
    wattroff(local_win, COLOR_PAIR(1) | A_BOLD );

    wmove(local_win,height-2,width*1/10);
    wprintw(local_win,exband.data());
    wattron(local_win, COLOR_PAIR(1) | A_BOLD );
    wprintw(local_win,db.data());
    wattroff(local_win, COLOR_PAIR(1) | A_BOLD );

    wmove(local_win,height-3,width*7/10);
    wattron(local_win, A_BOLD );
    wprintw(local_win,"Tab: ");
    wattroff(local_win,  A_BOLD );
    wattron(local_win, A_ITALIC );
    wprintw(local_win,string("Switch EQ").data());
    wattroff(local_win, A_ITALIC);

    wmove(local_win,height-2,width*7/10);
    wattron(local_win, A_BOLD );
    wprintw(local_win,"S: ");
    wattroff(local_win,  A_BOLD );
    wattron(local_win, A_ITALIC );
    wprintw(local_win,string("Save").data());
    wattroff(local_win, A_ITALIC);

    wattron(local_win, A_BOLD );
    wprintw(local_win," Q: ");
    wattroff(local_win,  A_BOLD );
    wattron(local_win, A_ITALIC );
    wprintw(local_win,string("Exit").data());
    wattroff(local_win, A_ITALIC);

    if(local_eqSet.isactive==true){
        wattron(local_win, COLOR_PAIR(2) | A_BOLD );
        mvwprintw(local_win,height-2,width*5/10,string("[ACTIVE]").data());
        wattroff(local_win, COLOR_PAIR(2) | A_BOLD );
    }else{
        wattron(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM );
        mvwprintw(local_win,height-2,width*5/10,string("[UNACTIVE]").data());
        wattroff(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM );
    }
    if(local_eqSet.unsaved==true){
        wattron(local_win, COLOR_PAIR(2) | A_BOLD );
        mvwprintw(local_win,height-3,width*5/10,string("[UNSAVED]").data());
        wattroff(local_win, COLOR_PAIR(2) | A_BOLD );
    }else{
        wattron(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
        mvwprintw(local_win,height-3,width*5/10,string("[SAVED]").data());
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
//  mvwhline(local_win,2, 1, 0,3);
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
}

void destroy_win(WINDOW *local_win){
    wrefresh(local_win);
    delwin(local_win);
}