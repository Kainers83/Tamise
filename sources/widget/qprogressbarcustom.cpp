#include "widget/qprogressbarcustom.h"

QProgressBarCustom::QProgressBarCustom(QWidget *parent) : QProgressBar(parent)
{

}

void QProgressBarCustom::mouseDoubleClickEvent( QMouseEvent *e )
{
    if ( e->button() == Qt::LeftButton )
    {
        int val = e->pos().x()*100/this->size().width();
        emit doubleClicked(val);

    }
}
void QProgressBarCustom::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        int val = e->pos().x()*100/this->size().width();
        emit doubleClicked(val);
    }

}

