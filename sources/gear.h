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
#ifndef GEAR_H
#define GEAR_H

#include <QWidget>
#include <QtXml>
#include <QUuid>

#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif

#include "ui_gear.h"
#include "threads/thd_gear.h"

#include "configuration.h"

#include "media.h"



class Gear : public QWidget, private Ui::GearDesign {

    Q_OBJECT

public:
    Gear(QDomElement, QWidget *parent = 0);
    ~Gear();
    bool isValid();
    QDomElement getValues(int mode = 0);

public slots:
    void setValues(QDomElement);
    void gearFinStop();
    void updateProgressBar(int time);
    QString convMinSec(int ms);
    void stateChanged ( Media::State newstate, Media::State oldstate );
    QString getGearId();
    QString getPageId();
    void setPageId(QString pid);
    void play();
    void setNext(bool state);
    void setMute(bool state);
    void setVolume(int vol);


private slots:
    void on_bStop_clicked();
    void on_bPlay_clicked(bool checked);
    void on_bLoop_clicked();
    void _volumeSlider_valueChanged(int changed);
    void _progPlaySeek_valueChanged(int changed = -1);
    void _ProgFinSeek_valueChanged(int changed = -1);
    void on_dPan_valueChanged();
    void stopThread();
    void on_bMute_clicked();
    void media_error(QString err);
    void check_loop();
    void ProgressBarDoubleClicked(int prcnt);


private:

    Media *media;
    Thd_gear* threadVolume;
    QString sep;
    QString gearId;
    QString pageId;

signals:
    void updateCSS();
    void error(QString);
    void nextGear(Gear*);
};

namespace Parametres {
    enum a{
        Titre = 0,
        Chemin_Fichier = 1,
        Note = 2,
        Volume = 3,
        Looped_checked = 4,
        Fondue_debut = 5,
        Fondue_fin = 6,
        Fondue_debut_checked = 7,
        Fondue_fin_checked = 8,
        Debut_Piste = 9,
        Fin_Piste = 10
    };
}

#endif // GEAR_H
