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

#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include <QList>

#include "configuration.h"
#include "threads/thd_media.h"
#include <FMOD/fmod.h>
#include <FMOD/fmod_errors.h>
#include <QTime>


#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif



#define TRUE 1
#define FALSE 0

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(QObject *parent = 0);
    explicit Media(QString file,QObject *parent = 0);
    ~Media();

    enum State{
        NoState = 0,
        ErrorState = 1,
        PlayingState = 2,
        PausedState = 3,
        StoppedState = 4,
    };

    void  setMedia(QString file);
    QString  getMedia();

    void setPan(float pan);
    float getPan();

    void setVolume(double vol);
    float getVolume();

    void setCurrentTime(int time);
    int currentTime();
    int totalTime();

    void setLoop(bool loop);
    bool isLoop();

    void setMute(bool mute);
    bool isMute();

    bool error(FMOD_RESULT result);
    void checkVersion();

    void updateState(Media::State newState);

    bool isPaused();

    int time;
	
	
    static bool errorS(FMOD_RESULT result);
    static bool getSystem();
    static void createSystem();
    static QStringList getDriverList();
    static void setSystemDriver(int drv_new, QString drv_name);
    static QString getCurrentDriverName();
    static int getCurrentDriverNum();

signals:
    void uState(int);
    void uTime(int);
    void finished();
    void error(QString error);
    void stateChanged(Media::State,Media::State);

public slots:
    void play(float vol,float pan);
    void pause();
    void restart();
    void stop();
    void uTimePos(int);
    void checkLoop();
    void setEndTime(int end);

private:

    void initMedia();

    static FMOD_SYSTEM *system;
    static int instance;

    FMOD_SOUND       *sound;
    FMOD_CHANNEL     *channel;

    unsigned int      version;
    Thd_media *upTimePos;
    Thd_media *upSystem;
    QString mediaFile;

    Media::State oldState;
    Media::State state;

    bool loop;
    int instanceMax;

};

#endif // MEDIA_H
