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
#ifndef GEARTHREAD_H
#define GEARTHREAD_H

#include <QThread>
#include <QWidget>
#include <QtCore/qmath.h>
#include "../media.h"


#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif


class Thd_gear : public QThread{
    Q_OBJECT

public:
    Thd_gear(QWidget *parent = 0);
    ~Thd_gear();
    void setArguments(int f, double a,int v, Media* m);
    enum Fonction{
        VolumeUp=1,
        VolumeDown=2
               };
protected:
    void run();
private:
    void volUp();
    void volDown();

    double tempsFondue;
    int fonction;
    Media* media;
    double vol;
    double newvol;
    double iteration;
signals:
    void media_stop();
    void setVolume(int);
    void setVolumeSlider(int);
};

#endif // GEARTHREAD_H
