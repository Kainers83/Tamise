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
#include "thd_media.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
Thd_media::Thd_media(ThdMedia_Fonctions::FONCTIONS fct, QObject *parent) : QThread(parent){
    channel = NULL;
    role = fct;
    a = true;
}
Thd_media::~Thd_media(){
    a=false;
}
/**************************************************************************************************
Accesseurs
**************************************************************************************************/
void Thd_media::setChannel(FMOD_CHANNEL *c){
    channel=c;
}
void Thd_media::setSystem(FMOD_SYSTEM *s){
    system = s;
}
void Thd_media::setSoundEnd(unsigned int end){
    endTime = end;
}
/**************************************************************************************************
Fonctions de traitement du thread
**************************************************************************************************/
void Thd_media::run(){
    switch(role){
    case ThdMedia_Fonctions::UPDATE_SYSTEM:
        while(a){
            FMOD_System_Update(system);
            msleep(10);
        }
        break;
    case ThdMedia_Fonctions::UPDATE_TIME_POSITION :
        updateTimePosition();
        break;
    }
}
void Thd_media::updateTimePosition(){
    position = 0;
    total = 0;
    FMOD_SOUND *sound;
    FMOD_Channel_GetCurrentSound(channel,&sound);
    FMOD_Sound_GetLength(sound,&total,FMOD_TIMEUNIT_MS);

    if(channel != NULL){
        while(a){
            FMOD_Channel_GetPosition(channel,&position,FMOD_TIMEUNIT_MS);
            emit timePosition(position);

            if(position >= endTime-200){
                emit media_finished();
            }
            msleep(10);
        }
    }
}
