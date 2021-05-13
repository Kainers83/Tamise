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
#include "conduitewidget.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
ConduiteWidget::ConduiteWidget(QWidget *parent) : QWidget(parent){

    setupUi(this);
    conduiteModel = new QStringListModel(this);
    conduiteLenght=0;
    conduiteInt=0;

    conduiteListView->setModel(conduiteModel);
}

ConduiteWidget::~ConduiteWidget(){

}
/**************************************************************************************************
Acesseurs
**************************************************************************************************/
QStringList ConduiteWidget::getConduite(){
    return conduiteModel->stringList();
}

QDomElement ConduiteWidget::getDomConduite(){

    QStringList list = conduiteModel->stringList();
    QDomDocument doc;
    QDomElement conduite = doc.createElement("conduite");
    for(int i=0;i<list.length();i++){
        QDomElement scene = doc.createElement("scene");
        QDomText text = doc.createTextNode(list.at(i));
        scene.appendChild(text);
        conduite.appendChild(scene);
    }
return conduite;
}
void ConduiteWidget::setConduite(QDomElement dom){

    QDomNode n = dom.firstChild();
    QStringList liste;
    while(!n.isNull()){
        liste << n.toElement().text();
        n = n.nextSibling();
    }

    conduiteModel->setStringList(liste);
    conduiteLenght = liste.length();
}
/**************************************************************************************************
Slots
**************************************************************************************************/
void ConduiteWidget::on_conduiteSuiv_clicked(){
    if(conduiteInt<conduiteLenght-1){
        conduiteLCD->display(conduiteLCD->intValue() +1);
        conduiteListView->setCurrentIndex(conduiteModel->index(conduiteLCD->intValue()));
        conduiteInt++;
    }

}
void ConduiteWidget::on_conduitePrev_clicked(){
    if(conduiteInt>0){
        conduiteLCD->display(conduiteLCD->intValue() -1);
        conduiteListView->setCurrentIndex(conduiteModel->index(conduiteLCD->intValue()));
        conduiteInt--;
    }
}
void ConduiteWidget::on_conduiteRAZ_clicked(){
    conduiteLCD->display(0);
    conduiteListView->setCurrentIndex(conduiteModel->index(0));
    conduiteInt = 0;
}
void ConduiteWidget::on_conduiteAdd_clicked(){
    int newRow = conduiteListView->currentIndex().row() +1;
    const QVariant v = "Nouvelle Scene " + QString().number(conduiteLenght+1);
    conduiteModel->insertRow(newRow);
    conduiteListView->setCurrentIndex(conduiteModel->index(newRow));
    conduiteModel->setData(conduiteModel->index(newRow),v);
    conduiteLenght++;
}
void ConduiteWidget::on_conduiteDel_clicked(){
    conduiteModel->removeRow(conduiteListView->currentIndex().row());
    conduiteLenght--;
}
void ConduiteWidget::on_conduiteUp_clicked(){
    /* à Implementer*/
}
void ConduiteWidget::on_conduiteDown_clicked(){
    /* à Implementer*/
}
void ConduiteWidget::reset(){
    conduiteModel->removeRows(0,conduiteModel->rowCount());
    conduiteLenght = 0;
    conduiteInt = 0;
    conduiteLCD->display(0);
}
