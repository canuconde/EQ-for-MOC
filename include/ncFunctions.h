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

using namespace std;
using namespace std::filesystem;

WINDOW *create_eqbar(int height, int width, int starty, int startx, const string &label, float value);

WINDOW *create_mainwin(int height, int width, int starty, int startx, eqSet &local_eqSet);

WINDOW *create_saveaswin(int height, int width, int starty, int startx);

void update_control(WINDOW *local_win, float value);

void update_window(WINDOW *local_win, eqSet &local_eqSet, string exband="", float value=0);

void clean_control(WINDOW *local_win);

void clean_window(WINDOW *local_win);

void destroy_win(WINDOW *local_win);
