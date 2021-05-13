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
#include "pages.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
GearArrange::GearArrange(GearArrange::Mode m,QWidget *parent) :QTabWidget(parent){
    //mettre en place une gestion correcte
setMovable(false);
connect(this,SIGNAL(tabBarDoubleClicked(int)),this,SLOT(on_tabBarDoubleClicked(int)));

}
GearArrange::~GearArrange(){

}

/**************************************************************************************************
Membres de gestion des Ajouts/Suppression des Gears et des Tab
**************************************************************************************************/
void GearArrange::addGear(Gear* gear, int position){
     QHBoxLayout *tp_lyt = static_cast<QHBoxLayout*>(pages.last()->layout());

    if(position<0 || position>tp_lyt->count()-1)
        tp_lyt->insertWidget(tp_lyt->count()-1,gear);
    else
        tp_lyt->insertWidget(position,gear);

}
void GearArrange::addGear(Gear* gear, QWidgetPage *page, int position){

     QHBoxLayout *tp_lyt = static_cast<QHBoxLayout*>(page->layout());

    if(position<0 || position>tp_lyt->count()-1)
        tp_lyt->insertWidget(tp_lyt->count()-1,gear);
    else
        tp_lyt->insertWidget(position,gear);
}
void GearArrange::removeGearFromTab(Gear* gear){
    for(int i=0; i<pages.length();i++){
        pages.at(i)->layout()->removeWidget(gear);
    }

}
QWidgetPage* GearArrange::newPage(QDomElement a){

    QString titre = a.attribute("nom");

    QScrollArea *area = new QScrollArea(this);
    QWidgetPage *page = new QWidgetPage(titre,this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    QSpacerItem *item = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout->addSpacerItem(item);

    addTab(area,titre);

    area->setWidget(page);
    area->setWidgetResizable(true);

    page->setValues(a);


    layout->setMargin(0);
    layout->setSpacing(0);

    page->setObjectName("Page");

    page->setLayout(layout);

    pages<<page;

    return page;
}

void GearArrange::deletePage(QWidgetPage *page){
    for(int i=0; i<pages.length();i++){
        if(page == pages.value(i)){
            removeTab(i);
            pages.value(i)->deleteLater();
            pages.removeAt(i);
        }
    }
}

QList<QWidgetPage*> GearArrange::getPageListe(){
    return pages;
}
QWidgetPage* GearArrange::getCurrentWidgetPage(){
    return pages.at(currentIndex());
}
void GearArrange::changePageName(QWidgetPage* page,QString titre){
    setTabText(pages.indexOf(page),titre);
}
void GearArrange::on_tabBarDoubleClicked(int index){
    QString newname = QInputDialog::getText(this, "Modifier le nom de l'onglet",
                                            "Nom de l'onglet:", QLineEdit::Normal,tabText(index));
    if(newname.isEmpty()){
       QMessageBox::warning(this,"Nom de l'Onglet vide","L'Onglet n'a pas encore de titre.",QMessageBox::Ok);
        return;

    }
        setTabText(index,newname);
        pages.at(index)->setNom(newname);

}
int GearArrange::gearCount(){
    int gearCount = 0;
    for(int i=0;i<pages.length();i++){
        gearCount = gearCount + pages.at(i)->getGears().length();
    }
    return gearCount;
}
