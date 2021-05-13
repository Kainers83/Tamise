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
#ifndef QPROGRESSLOAD_H
#define QPROGRESSLOAD_H

#ifdef QT_NO_DEBUG
    #define tam tamdebug(msg)
#else
    #include <QDebug>
    #define tam tamdebug(msg)    qDebug(qPrintable(msg));
#endif

#include "ui_QProgressLoad.h"

class QProgressLoad : public QWidget, private Ui::QProgressLoadDesign{

    Q_OBJECT

public:
    QProgressLoad(QWidget *parent);
    ~QProgressLoad();

    void setPageMax(int max);
    void setGearMax(int max);
    void setPageValue(int val);
    void setGearValue(int val);
    void setPageLabelText(QString name);
    void setGearLabelText(QString name);


};




#endif
