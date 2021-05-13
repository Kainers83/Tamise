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
#include "gear.h"
/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/

Gear::Gear(QDomElement element, QWidget *parent){
    setupUi(this);
    setParent(parent);
    gbTitre->setObjectName("NoState");

    media = new Media(this);
    threadVolume = new Thd_gear(this);

    connect(media,SIGNAL(stateChanged (Media::State, Media::State)),this,SLOT(stateChanged ( Media::State, Media::State)));
    connect(media, SIGNAL(uTime(int)),this,SLOT(updateProgressBar(int)));
    connect(media,SIGNAL(finished()),this,SLOT(check_loop()));
    connect(media,SIGNAL(error(QString)),this,SLOT(media_error(QString)));

    connect(volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(_volumeSlider_valueChanged(int)));
    connect(progPlaySeek,SIGNAL(valueChanged(int)),this,SLOT(_progPlaySeek_valueChanged(int)));
    connect(progPlaySeek,SIGNAL(sliderReleased()),this,SLOT(_progPlaySeek_valueChanged()));
    connect(ProgFinSeek,SIGNAL(valueChanged(int)),this,SLOT(_ProgFinSeek_valueChanged(int)));
    connect(volumeSlider,SIGNAL(sliderClicked()),this,SLOT(stopThread()));

    connect(threadVolume,SIGNAL(setVolume(int)),volumeSlider,SLOT(setValue(int)));
    connect(progPlay,SIGNAL(doubleClicked(int)),this,SLOT(ProgressBarDoubleClicked(int)));

    volumeSlider->installEventFilter(this);

    QUuid genId;
    gearId = genId.createUuid().toString();
    setValues(element);
}
Gear::~Gear(){
    threadVolume->terminate();
}
/**************************************************************************************************
Membres de Gestions des parametres du controleur
**************************************************************************************************/

void Gear::setValues(QDomElement element){
    //gbTitre->setObjectName("NoState");

    media->setMedia(element.attribute("fichier"));
    gbTitre->setTitle(element.attribute("nom","John Do"));
    volumeSlider->setValue(element.attribute("volume","50").toInt());
    leNotes->setText(element.attribute("notes",""));
    (element.attribute("loop","0").toInt())?bLoop->setChecked(true):bLoop->setChecked(false);
    (bLoop->isChecked())?media->setLoop(true):media->setLoop(false);

    dsbFondueStart->setValue(element.attribute("startF","0").toDouble());
    (element.attribute("startFChecked","0").toInt())?cbFondueStart->setCheckState(Qt::Checked):cbFondueStart->setCheckState(Qt::Unchecked);

    dsbFondueStop->setValue(element.attribute("endF","0").toDouble());
    (element.attribute("endFChecked","0").toInt())?cbFondueStop->setCheckState(Qt::Checked):cbFondueStop->setCheckState(Qt::Unchecked);

    progPlaySeek->setMaximum(media->totalTime());
    progPlaySeek->setValue(element.attribute("debut").toInt());

    ProgFinSeek->setMaximum(media->totalTime());

    ProgFinSeek->setValue(element.attribute("fin",QString::number(media->totalTime())).toInt());

    bNext->setChecked(element.attribute("next","0").toInt());
}

QDomElement Gear::getValues(int mode){
    QDomDocument doc;
    QDomElement Val = doc.createElement("gear");
    Val.setAttribute("nom",gbTitre->title());
    Val.setAttribute("fichier",media->getMedia());
    Val.setAttribute("notes",leNotes->text());
    Val.setAttribute("volume",volumeSlider->value());
    Val.setAttribute("loop",bLoop->isChecked());
    Val.setAttribute("startFChecked",cbFondueStart->isChecked());
    Val.setAttribute("endFChecked",cbFondueStop->isChecked());
    Val.setAttribute("startF",dsbFondueStart->value());
    Val.setAttribute("endF",dsbFondueStop->value());
    Val.setAttribute("pan",dPan->value());
    Val.setAttribute("debut",progPlaySeek->value());
    Val.setAttribute("fin",ProgFinSeek->value());
    Val.setAttribute("next",bNext->isChecked());

    if(mode == 0){
        Val.setAttribute("total",media->totalTime());
        Val.setAttribute("uuid",gearId);
        Val.setAttribute("pid",pageId);
    }
    return Val;
}

QString Gear::getGearId(){
    return gearId;
}

QString Gear::getPageId(){
    return pageId;
}
void Gear::setPageId(QString pid){
    pageId = pid;
}
QString Gear::convMinSec(int ms){

    int s = ms/1000;
    int mn = s/60;
    ms = ms - (ms/1000)*1000;
    s -= mn*60;

    QString sDiz("");
    QString mnDiz("");
    QString msDiz("");

    if(s<10) sDiz = "0";
    if(mn<10) mnDiz = "0";
    if(ms<100) msDiz = "0";
    if(ms<10) msDiz = "00";

    return mnDiz + QString::number(mn) + ":" + sDiz + QString::number(s) + ":" + msDiz + QString::number(ms);
}
/**************************************************************************************************
Gestion du controleur même
**************************************************************************************************/
void Gear::play(){
    tamdebug("lancement gear Next "+gearId);
    //bPlay->setChecked(true);
    bPlay->click();
    this->setStyleSheet(this->styleSheet());
}
void Gear::setVolume(int vol){
    volumeSlider->setValue(vol);
}
void Gear::on_bPlay_clicked(bool checked){
     if(checked){
        if(threadVolume->isRunning()) threadVolume->terminate();
        if(cbFondueStart->isChecked() && dsbFondueStart->value()!=0){
            threadVolume->setArguments(Thd_gear::VolumeUp,dsbFondueStart->value(),volumeSlider->value(),media);
            media->play(0,((double)dPan->value())/100);

            threadVolume->start();
        }
        else{
            media->play(((float)volumeSlider->value())/100,((float)dPan->value())/100);
        }
    }
    else{
        media->pause();
    }
    this->setStyleSheet(this->styleSheet());
}
void Gear::check_loop(){
    if(bLoop->isChecked()){
        media->setCurrentTime(progPlaySeek->value());
        return;
    }
    else if(bNext->isChecked())
        emit nextGear(this);
    on_bStop_clicked();
}
void Gear::on_bStop_clicked(){

    if(threadVolume->isRunning()) threadVolume->terminate();
    if(cbFondueStop->isChecked() && dsbFondueStop->value()!=0 && !media->isPaused()){
        threadVolume->setArguments(Thd_gear::VolumeDown,dsbFondueStop->value(),volumeSlider->value(),media);
        connect(threadVolume,SIGNAL(media_stop()),this,SLOT(gearFinStop()));
        threadVolume->start();
    }
    else gearFinStop();
}
void Gear::gearFinStop(){
    bPlay->setChecked(false);
    media->stop();
    media->setCurrentTime(progPlaySeek->value());
}
void Gear::on_bLoop_clicked(){
    (bLoop->isChecked())?media->setLoop(true):media->setLoop(false);
}
void Gear::ProgressBarDoubleClicked(int prcnt){
    int setTime = prcnt*media->totalTime()/100;
    if(media->isPaused()) return;
    if(setTime>ProgFinSeek->value()) setTime=ProgFinSeek->value();
    if(setTime<progPlaySeek->value()) setTime=progPlaySeek->value();

    media->setCurrentTime(setTime);
}
void Gear::updateProgressBar(int time){
    int avancement = time*100/media->totalTime();
    progPlay->setValue(avancement);
    progPlay->setFormat(convMinSec(time));
    progPlaySeek->setMaximum(media->totalTime());
    ProgFinSeek->setMaximum(media->totalTime());
    //    progPlaySeek->setValue(time);
}
void Gear::_progPlaySeek_valueChanged(int changed){

    if(progPlaySeek->value()+1000>=ProgFinSeek->value())
            ProgFinSeek->setValue(progPlaySeek->value()+1000);
    if(changed==-1)
        changed = progPlaySeek->value();
    media->setCurrentTime(changed);
    progPlaySeek->setToolTip(convMinSec(changed));
}

void Gear::_ProgFinSeek_valueChanged(int changed){

    if(progPlaySeek->value()+1000>=ProgFinSeek->value())
            progPlaySeek->setValue(ProgFinSeek->value()-1000);

    if(changed==-1)
        changed = ProgFinSeek->value();

    media->setEndTime(changed);
    ProgFinSeek->setToolTip(convMinSec(changed));

}
void Gear::_volumeSlider_valueChanged(int changed){
    media->setVolume(((float)changed)/100);
}
void Gear::on_dPan_valueChanged(){
    media->setPan(((float)dPan->value())/100);
}
void Gear::on_bMute_clicked(){
    (bMute->isChecked())?media->setMute(true):media->setMute(false);
}
void Gear::stopThread(){
    threadVolume->terminate();
}
void Gear::setNext(bool state){
    bNext->setChecked(state);
}
void Gear::setMute(bool state){
    bMute->setChecked(state);
    on_bMute_clicked();
}
/**************************************************************************************************
Membres d"Etats du controleurs
**************************************************************************************************/
void Gear::stateChanged ( Media::State newstate, Media::State oldstate ){

    switch(newstate){
    case Media::NoState:
        gbTitre->setObjectName("NoState");
        break;
    case Media::ErrorState:
        gbTitre->setObjectName("Error");
        this->setEnabled(false);
        break;
    case Media::StoppedState:
        gbTitre->setObjectName("Stopped");
        break;
    case Media::PlayingState:
        gbTitre->setObjectName("Playing");
        break;
    case Media::PausedState:
        gbTitre->setObjectName("Paused");
        break;
    }
    //emit updateCSS();
    //QEvent::Polish
    this->setStyleSheet(this->styleSheet());
}
void Gear::media_error(QString err){
    emit error(err);
}
bool Gear::isValid(){
    return (gbTitre->objectName()=="Error")?false:true;
}
