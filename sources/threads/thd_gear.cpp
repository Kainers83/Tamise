/*
Copyright 2011 LE GOUIL Kerwan

This file is part of TaMiSe.

TaMiSe is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TaMiSe is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TaMiSe.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "thd_gear.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
Thd_gear::Thd_gear(QWidget *parent):QThread(parent){

}
Thd_gear::~Thd_gear(){
}
/**************************************************************************************************
Acesseurs
**************************************************************************************************/
void Thd_gear::setArguments(int f, double a, int v, Media* m){
    fonction = f;
    tempsFondue=a;
    media=m;
    vol = ((double)v);
}
/**************************************************************************************************
Membres de traitement du thread
**************************************************************************************************/
void Thd_gear::run(){
    switch(fonction){
    case Thd_gear::VolumeUp: volUp(); break;
    case Thd_gear::VolumeDown: volDown();break;
    }
}

void Thd_gear::volUp(){
    newvol = 0;
    double decoupe = (tempsFondue/1000);
    iteration=0;


	//mise a zero du volume pour le fondue
    emit setVolume(0);
	//on monte progressivement le volume
    while(iteration<tempsFondue){
        msleep(decoupe*1000);
        newvol = ((int)((qPow(iteration/tempsFondue,3)*(vol/100))*100));
        emit setVolume(newvol);
		iteration += decoupe;

    }
    emit setVolume(vol);
}
void Thd_gear::volDown(){
    double newvol = 0;
    iteration=tempsFondue;
    double decoupe=tempsFondue/1000;

    while(iteration>=decoupe){
        newvol =(int)((qPow(iteration/tempsFondue,3)*vol/100)*100);
        if(vol<=0)emit setVolume(0);
        else emit setVolume(newvol);
        msleep(decoupe*1000);
        iteration -= decoupe;

    }
    emit media_stop();
    msleep(500);
    emit setVolume(vol);
}
