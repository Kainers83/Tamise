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
#include "media.h"


FMOD_SYSTEM* Media::system = NULL;
int Media::instance = 0;

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
Media::Media(QObject *parent) : QObject(parent){
    initMedia();
    sound = NULL;
    instance++;

}
Media::Media(QString file,QObject *parent): QObject(parent){
    sound = NULL;
    initMedia();
    setMedia(file);
    instance++;
}
Media::~Media(){
    if(upTimePos->isRunning()) upTimePos->terminate();
    if(upSystem->isRunning()) upSystem->terminate();
    FMOD_Sound_Release(sound);

    instance--;
}
void Media::initMedia(){
    channel = 0;
    time=0;

    //creation des threads
    upTimePos = new Thd_media(ThdMedia_Fonctions::UPDATE_TIME_POSITION,this);
    upSystem = new Thd_media(ThdMedia_Fonctions::UPDATE_SYSTEM,this);

    connect(upTimePos,SIGNAL(timePosition(int)),this,SLOT(uTimePos(int)));
    connect(upTimePos,SIGNAL(media_finished()),this,SLOT(checkLoop()));
    upSystem->start();


}
/**************************************************************************************************
    Création et Gestion du System Media
**************************************************************************************************/

bool Media::getSystem(){
    if(system == NULL)
        return false;
    else
        return true;
}
void Media::createSystem(){
    if(system==NULL){
        if (errorS(FMOD_System_Create(&system)))
            return;
        int channelMax = Configuration::load("FMOD","channel_max",(int) 1000);
        int drv_num = Configuration::load("FMOD","drvnum",0);
        QString drv_name = Configuration::load("FMOD","drv_name",getDriverList().at(drv_num));

        Media::setSystemDriver(drv_num,drv_name);
        if(errorS(FMOD_System_SetSoftwareChannels(system,channelMax)))
            return;
        if(errorS(FMOD_System_Init(system, channelMax, FMOD_INIT_NORMAL, NULL)))
            return;
    }
}
/**************************************************************************************************
    Gestion du peripherique de sortie Audio
**************************************************************************************************/
//To Do: Crée une gestion des erreurs de FMOD
QStringList Media::getDriverList(){//Mettre la gestion des erreurs.
    int drv_num = 0;
    char drv_nom[255];
    QStringList drv_lst;

    FMOD_System_GetNumDrivers(system, &drv_num);
    for(int i=0;i<drv_num;i++){
        FMOD_System_GetDriverInfo(system,i,drv_nom,255,NULL);
        drv_lst << QString::fromLatin1(drv_nom);
    }
    return drv_lst;
}
void Media::setSystemDriver(int drv_new, QString drv_name){
    QStringList drv_liste = Media::getDriverList();
    int drv_number = drv_liste.length();

    if((drv_new>=0 and drv_new < drv_number) and drv_name == drv_liste.at(drv_new)){
        FMOD_System_SetDriver(system,drv_new);
        Configuration::save("FMOD","drv_num",drv_new);
        Configuration::save("FMOD","drv_name",drv_liste.at(drv_new));
    }

}
QString Media::getCurrentDriverName(){
    int drv = 0;
    char drv_name[255];
    FMOD_System_GetDriver(system,&drv);
    FMOD_System_GetDriverInfo(system,drv,drv_name,255,NULL);
    QString driver = QString::fromLatin1(drv_name);
    return driver;
}

 int Media::getCurrentDriverNum(){
    int drv = -1;
    FMOD_System_GetDriver(system,&drv);
    return drv;
}
/**************************************************************************************************
    Acceseurs de Modifications du media
**************************************************************************************************/

void Media::setMedia(QString file){

    if(sound != NULL){
        FMOD_Sound_Release(sound);
    }
    if(error(FMOD_System_CreateStream(system, file.toLatin1(), FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound)))
        return;

    if(error(FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, TRUE, &channel)))
        return;

    upTimePos->setChannel(channel);
    upTimePos->start();

    mediaFile = file;
    loop = false;
}
void Media::setPan(float pan){
    if(pan>=-1.0 && pan<=1.0)
        FMOD_Channel_SetPan(channel,pan);
}
void Media::setVolume(double vol){
    FMOD_Channel_SetVolume(channel,vol);
}
void Media::setMute(bool mute){
    (mute)?FMOD_Channel_SetMute(channel,TRUE):FMOD_Channel_SetMute(channel,FALSE);
}
void Media::setCurrentTime(int t){
    FMOD_Channel_SetPosition(channel,t,FMOD_TIMEUNIT_MS);
}
int Media::currentTime(){
    unsigned int pos;
    FMOD_Channel_GetPosition(channel,&pos,FMOD_TIMEUNIT_MS);
    return pos;
}
void Media::setLoop(bool l){
    loop = l;
}
/**************************************************************************************************
Acceseurs de Valeurs du media
**************************************************************************************************/
QString Media::getMedia(){
    return mediaFile;
}
float Media::getPan(){
    float pan;
    FMOD_Channel_GetPan(channel,&pan);
    return pan;
}
float Media::getVolume(){
    float vol;
    FMOD_Channel_GetVolume(channel,&vol);
    return vol;
}
/**************************************************************************************************
Acesseurs d'etats du media
**************************************************************************************************/
int Media::totalTime(){
    unsigned int length;
    FMOD_Sound_GetLength(sound,&length,FMOD_TIMEUNIT_MS);
    return length;
}
bool Media::isMute(){
    int mute;
    FMOD_Channel_GetMute(channel,&mute);
    return mute;
}
bool Media::isLoop(){
    return loop;
}
bool Media::isPaused(){
    FMOD_BOOL p;
    FMOD_Channel_GetPaused(channel,&p);
    return (p==TRUE)? true:false;
}
/**************************************************************************************************
 SLOTS
 **************************************************************************************************/
void Media::uTimePos(int ms){
    emit uTime(ms);
}
void Media::play(float vol,float pan){

    FMOD_BOOL paused = FALSE;
    FMOD_Channel_GetPaused(channel,&paused);
    FMOD_Channel_SetPaused(channel,!paused);

    setMute(false);
    setVolume(vol);
    setPan(pan);
    updateState(Media::PlayingState);

    //upTimePos->setChannel(channel);
    //upTimePos->start();

}
void Media::stop(){
    FMOD_Channel_SetPaused(channel,TRUE);
    FMOD_Channel_SetPosition(channel,time,FMOD_TIMEUNIT_MS);
    updateState(Media::StoppedState);

}
void Media::pause(){
    FMOD_RESULT result = FMOD_Channel_SetPaused(channel,TRUE);
    error(result);
    updateState(Media::PausedState);
}
void Media::restart(){
    FMOD_Channel_SetPosition(channel,time,FMOD_TIMEUNIT_MS);
}
void Media::checkLoop(){
    if(!loop){
        stop();
    }
    emit finished();
}

void Media::setEndTime(int end){
    upTimePos->setSoundEnd(end);
}

/**************************************************************************************************
 Check erreurs
 **************************************************************************************************/
bool Media::error(FMOD_RESULT result){
    if (result != FMOD_OK){
        emit error("Instance:"+QString::number(instance)+" FMOD error!: " + QString::number(result)+" - " + QString::fromLatin1(FMOD_ErrorString(result)));
        updateState(Media::ErrorState);
        return true;
    }
    return false;
}
bool Media::errorS(FMOD_RESULT result){
    if (result != FMOD_OK){
        //emit errorS("FMOD error!: " + QString::number(result)+" - " + QString::fromLatin1(FMOD_ErrorString(result)));
        return true;
    }
    return false;
}
void Media::checkVersion(){
    if(error(FMOD_System_GetVersion(system, &version)));
    if (version < FMOD_VERSION){
        emit error("Error!  You are using an old version of FMOD " + QString::number(version) + ".  This program requires " + QString::number(FMOD_VERSION));
        updateState(Media::ErrorState);
    }
}
void Media::updateState(Media::State newState){
    oldState = state;
    state = newState;
    emit stateChanged(newState,oldState);
}
