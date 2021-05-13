#-------------------------------------------------
#
# Project created by QtCreator 2011-03-17T15:10:56
#
#-------------------------------------------------
#
#Copyright 2011 LE GOUIL Kerwan
#
#This file is part of TaMiSe.
#
#TaMiSe is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#TaMiSe is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with TaMiSe.  If not, see <http://www.gnu.org/licenses/>.
#

QT += core gui network widgets xml

TARGET = Tamise
TEMPLATE = app
INCLUDEPATH += -L$$PWD/FMOD_4/lib

win32{
    RC_FILE = res/rc_file.rc
    LIBS += C:/Qt/5.15.2/mingw81_32/lib/libfmodex.a #-lfmodex
}
unix{
    LIBS += -lfmodex-4.34.01
}

SOURCES += main.cpp\
        mainwindow.cpp \
    gear.cpp \
    conduitewidget.cpp \
    geardialog.cpp \
    media.cpp \
    threads/thd_media.cpp \
    threads/thd_gear.cpp \
    widget/qslidercustom.cpp \
    pages.cpp \
    configuration.cpp \
    widget/journal.cpp \
    pagedialog.cpp \
    widget/QWidgetPage.cpp \
    widget/QProgressLoad.cpp \
    widget/qprogressbarcustom.cpp

HEADERS  += mainwindow.h \
    gear.h \
    conduitewidget.h \
    geardialog.h \
    media.h \
    threads/thd_media.h \
    threads/thd_gear.h \
    widget/qslidercustom.h \
    pages.h \
    configuration.h \
    widget/journal.h \
    pagedialog.h \
    widget/QWidgetPage.h \
    widget/QProgressLoad.h \
    widget/qprogressbarcustom.h

FORMS    += \
    gear.ui \
    conduitewidget.ui \
    mainwindow.ui \
    geardialog.ui \
    pagedialog.ui \
    widget/QProgressLoad.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    res/style.css \
    res/rc_file.rc \
    res/tamise.css

DISTFILES += \
    res/button.png
