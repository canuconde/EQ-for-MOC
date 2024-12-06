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

class eqSet{
private:
    struct band_type{
        int bandfreq;
        float bandwidth;
        float bandamp;
    };
    float amp[3]={0,0.00,0.00};
    band_type band[11]={0,0.00,0.00,
                        60,1.46,-0.00,
                        170,1.42,-0.00,
                        310,1.42,-0.00,
                        600,1.42,-0.00,
                        1000,1.42,-0.00,
                        3000,1.44,-0.00,
                        6000,1.50,-0.00,
                        12000,1.76,-0.00,
                        14000,1.90,-0.00,
                        16000,2.08,-0.00};

public:
    eqSet(const string &file_path);
    string name;
    path moc_dir;
    bool unsaved=false;
    bool isactive=false;
    bool setbandvalue(const int &bandnumber,const float &value);
    float getbandvalue(const int &bandnumber);
    bool save();
    bool save_as(const string &new_filename);
};
