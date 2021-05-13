#include "widget/QProgressLoad.h"

QProgressLoad::QProgressLoad(QWidget *parent): QWidget(parent){
    setupUi(this);
    setWindowFlags(Qt::Tool);
}
QProgressLoad::~QProgressLoad(){

}

void QProgressLoad::setPageMax(int max){
    page->setMaximum(max);
}
void QProgressLoad::setGearMax(int max){
    gear->setMaximum(max);
}
void QProgressLoad::setPageValue(int val){
    page->setValue(val);
}
void QProgressLoad::setGearValue(int val){
    gear->setValue(val);
}
void QProgressLoad::setPageLabelText(QString name){
    label->setText("Chargement de : "+name);
}
void QProgressLoad::setGearLabelText(QString name){
    label_2->setText("Chargement de : "+name);
}
