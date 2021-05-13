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
#ifndef CONDUITEWIDGET_H
#define CONDUITEWIDGET_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QtXml>


#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif

#include "ui_conduitewidget.h"

namespace Ui {
    class ConduiteWidget;
}

class ConduiteWidget : public QWidget, private Ui::ConduiteWidget {
    Q_OBJECT

public:
    explicit ConduiteWidget(QWidget *parent = 0);
    ~ConduiteWidget();
    QStringList getConduite();
    QDomElement getDomConduite();
    void setConduite(QDomElement dom);
    void reset();
public slots:
    void on_conduiteSuiv_clicked();
    void on_conduitePrev_clicked();
    void on_conduiteRAZ_clicked();
    void on_conduiteAdd_clicked();
    void on_conduiteDel_clicked();
    void on_conduiteUp_clicked();
    void on_conduiteDown_clicked();
private:
    QStringListModel* conduiteModel;

    int conduiteLenght;
    int conduiteInt;

};

#endif // CONDUITEWIDGET_H
