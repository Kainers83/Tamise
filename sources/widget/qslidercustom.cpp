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
#include "qslidercustom.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
QSliderCustom::QSliderCustom(QWidget *parent){
    setParent(parent);
}
inline int QSliderCustom::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}
/**************************************************************************************************
Membres de traitement du clic sur le QSLider
**************************************************************************************************/
int QSliderCustom::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

void QSliderCustom::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const QPoint center = sliderRect.center() - sliderRect.topLeft();
        // to take half of the slider off for the setSliderPosition call we use the center - topLeft

        if (!sliderRect.contains(event->pos())) {
            event->accept();

            setSliderPosition(pixelPosToRangeValue(pick(event->pos() - center)));
            triggerAction(SliderMove);
            setRepeatAction(SliderNoAction);
        } else {
            QSlider::mousePressEvent(event);
        }
    }
    else if(event->button() == Qt::RightButton){
        setValue(QInputDialog::getInt(this,"","",value(),minimum(),maximum(),1));
    }
    else {
        QSlider::mousePressEvent(event);
    }
    emit sliderClicked();
}
