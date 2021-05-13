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


A noter que les fonctions:
QSliderCustom::pixelPosToRangeValue(int pos) const
void QSliderCustom::mousePressEvent(QMouseEvent *event)

ont été reprise de la classe SwiftSlider integré a Qt
dans le fichier qt\src\3rdparty\phonon\phonon\swiftslider_p.h
Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

*/

#ifndef QSLIDERCUSTOM_H
#define QSLIDERCUSTOM_H

#include <QSlider>
#include <QWidget>
#include <QStyle>
#include <QStyleOption>
#include <QMouseEvent>
#include <QInputDialog>

class QSliderCustom : public QSlider{

    Q_OBJECT
public:
    explicit QSliderCustom(QWidget *parent = 0);

signals:
    void sliderClicked();

public slots:

private:
    void mousePressEvent(QMouseEvent *event);
    inline int pick(const QPoint &pt) const;
    int pixelPosToRangeValue(int pos) const;
};

#endif // QSLIDERCUSTOM_H
