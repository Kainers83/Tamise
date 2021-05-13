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

#ifndef GEARARRANGE_H
#define GEARARRANGE_H

#include <QWidget>
#include <QTabWidget>
#include <QScrollArea>
#include <QList>
#include <QUuid>
#include <QInputDialog>
#include <QMessageBox>


#include "configuration.h"
#include "gear.h"
#include "widget/QWidgetPage.h"

#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif


class GearArrange : public QTabWidget
{
    Q_OBJECT
public:
    enum Mode{
        Ligne = 0,
        Onglet = 1
    };

    explicit GearArrange(GearArrange::Mode m,QWidget *parent = 0);
    ~GearArrange();

    void addGear(Gear* gear,int position = -1);
    void addGear(Gear* gear, QWidgetPage *page=NULL, int position = -1);
    void removeGearFromTab(Gear *gear);

    QWidgetPage *newPage(QDomElement a);
    void deletePage(QWidgetPage *page);

    QList<QWidgetPage*> getPageListe();
    QWidgetPage* getCurrentWidgetPage();
    int gearCount();


signals:

public slots:
    void changePageName(QWidgetPage* page,QString titre);
    void on_tabBarDoubleClicked(int index);

private:

    GearArrange::Mode mode;
    QList<Gear*> gearList;
    QList<QWidgetPage*> pages;

};

#endif // GEARARRANGE_H
