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
#ifndef GEARDIALOG_H
#define GEARDIALOG_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QStringListModel>

#include "ui_geardialog.h"
#include "gear.h"
#include "widget/QWidgetPage.h"
#include "configuration.h"

#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif

namespace Ui {
    class GearDialog;
}

class GearDialog : public QWidget, private Ui::GearDialogDesign{

    Q_OBJECT

public:
    explicit GearDialog(QWidget *parent = 0);
    void setValues(QDomElement a);
    QDomElement getValues();
    ~GearDialog();
    void addGearToComboBox(Gear *gear, int position=-1);
    void removeFromComboBox(Gear *gear);
    void addPageToComboBox(QWidgetPage *page, int position=-1);
    void removePageFromComboBox(QWidgetPage *page);

    void clear();
    void updateComboBoxPageListe(QList<QWidgetPage*> liste);
    void updateComboBoxGearListe(QList<Gear*> liste);

    void setCurrentIndex(int index = -1);
private:
    Ui::GearDialog *ui;
    int totalms;
private slots:
    void on_add_clicked();
    void on_Reset_clicked();
    void on_toolButton_clicked();
    void on_gearListe_activated(int index=-1);
    void on_edit_clicked();
    void on_del_clicked();
    void on_debut_timeChanged();
    void on_pageListe_activated(int index=-1);
    void on_pageListePos_activated(int index=-1);


signals:
    void creaGearButton(QDomElement,QWidgetPage*,int);
    void creaGearButton(QDomElement);

    void delGearButton(Gear*,QWidgetPage*);
};

#endif // GEARDIALOG_H
