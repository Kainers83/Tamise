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

#ifndef THD_MEDIA_H
#define THD_MEDIA_H

#include <QThread>
#include <FMOD/fmod.hpp>


#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif


namespace ThdMedia_Fonctions{
    enum FONCTIONS{
        UPDATE_SYSTEM = 0,
        UPDATE_TIME_POSITION = 1,
    };
}
class Thd_media : public QThread
{
    Q_OBJECT
public:


    explicit Thd_media(ThdMedia_Fonctions::FONCTIONS fct,QObject *parent = 0);
    ~Thd_media();
    void setChannel(FMOD_CHANNEL *c);
    void setSystem(FMOD_SYSTEM *s);
    void updateTimePosition();
    void run();

signals:
    void timePosition(int);
    void media_finished();

public slots:
    void setSoundEnd(unsigned int end);

private:
    bool a;
    FMOD_CHANNEL *channel;
    FMOD_SYSTEM *system;
    unsigned int position;
    unsigned int total;
    unsigned int endTime;

    ThdMedia_Fonctions::FONCTIONS role;

};

#endif // THD_MEDIA_H
