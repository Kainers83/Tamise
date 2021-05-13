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
#ifndef PAGEDIALOG_H
#define PAGEDIALOG_H

#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QList>
#include <QtXml>


#include "widget/QWidgetPage.h"
#include "configuration.h"

#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif

#include "ui_pagedialog.h"



namespace Ui {
    class PageDialog;
}

class PageDialog : public QWidget, private Ui::PageDialogDesign {
    Q_OBJECT

public:
    explicit PageDialog(QWidget *parent = 0);
    ~PageDialog();
    void updateComboBoxPageListe(QList<QWidgetPage *> liste);

    void addPageToComboBox(QWidgetPage *page,int position);
    void removePageFromComboBox(QWidgetPage *page);

public slots:
    QDomElement getValues();
    void setValues(QDomElement a);
    void on_add_clicked();
    void on_pageList_activated(int index);
    void on_reset_clicked();
    void on_edit_clicked();
    void on_del_clicked();

signals:
    void creatPageButton(QDomElement);
    void editPageButton(QString, QDomElement);
    void delPageButton(QWidgetPage*);
    void changePageName(QWidgetPage*,QString);
private:



};

#endif // PAGEDIALOG_H
