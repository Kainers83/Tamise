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
#include "geardialog.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
GearDialog::GearDialog(QWidget *parent): QWidget(parent){
    setupUi(this);
    setWindowFlags(Qt::Dialog);
    on_Reset_clicked();
    edit->setEnabled(false);
    del->setEnabled(false);
}
GearDialog::~GearDialog(){
}
/**************************************************************************************************
Membres de Gestions des parametres du modificateur de controleurs
**************************************************************************************************/
QDomElement GearDialog::getValues(){
    QDomDocument doc("gear");
    QDomElement Val = doc.createElement("gear");
    Val.setAttribute("nom",titre->text());
    Val.setAttribute("fichier",chemin->text());
    Val.setAttribute("notes",notes->toPlainText().replace("\n","<br />"));
    Val.setAttribute("volume",volume->value());
    Val.setAttribute("loop",boucle->isChecked());
    Val.setAttribute("startF",fondueStart->value());
    Val.setAttribute("endF",fondueStop->value());
    Val.setAttribute("startFChecked",fondueStartCheck->isChecked());
    Val.setAttribute("endFChecked",fondueStopCheck->isChecked());

    int time = QTime(0,0).msecsTo(debut->time());
    Val.setAttribute("debut",time);

    int time2 = QTime(0,0).msecsTo(fin->time());
    if(time2>time+1000)
        Val.setAttribute("fin",time2);
    else
        Val.setAttribute("fin",totalms);

    return Val;
}
void GearDialog::setValues(QDomElement a){

    titre->setText(a.attribute("nom","John Doe"));
    chemin->setText(a.attribute("fichier"));
    notes->setText(a.attribute("notes",""));
    volume->setValue(a.attribute("volume","100").toInt());
    boucle->setChecked(a.attribute("loop","0").toInt());
    fondueStart->setValue(a.attribute("startF","1").toInt());
    fondueStop->setValue(a.attribute("endF","1").toInt());
    fondueStartCheck->setChecked(a.attribute("startFChecked").toInt());
    fondueStopCheck->setChecked(a.attribute("endFChecked").toInt());
    totalms = a.attribute("total").toInt();

    debut->setTime(QTime(0,0).addMSecs(a.attribute("debut").toInt()));
    debut->setMaximumTime(QTime(0,0).addMSecs(a.attribute("total").toInt()-1000));

    fin->setTime(QTime(0,0).addMSecs(a.attribute("fin").toInt()));
    fin->setMaximumTime(QTime(0,0).addMSecs(a.attribute("total").toInt()));
}

void GearDialog::on_Reset_clicked(){
    titre->setText("");
    chemin->setText("");
    notes->setText("");
    volume->setValue(100);
    boucle->setChecked(false);
    fondueStart->setValue(1);
    fondueStop->setValue(1);
    fondueStartCheck->setChecked(false);
    fondueStopCheck->setChecked(false);
    debut->setTime(QTime(0,0));
    fin->setTime(QTime(0,0));

    edit->setEnabled(false);
    del->setEnabled(false);

}
/**************************************************************************************************
Gestion des actions du widget
**************************************************************************************************/
void GearDialog::on_debut_timeChanged(){
    if(debut->time() > fin->time())
        debut->setTime(fin->time());

}
void GearDialog::on_add_clicked(){
    //faire les tests pour verifeir le chmein du fichier et le formats :D
    if(!QFile::exists(chemin->text())){
        QMessageBox::warning(this,"Fichier Introuvable","Le fichier "+chemin->text()+"est introuvable");
        return;
    }
    if(!chemin->text().contains(QRegExp(".[mp3|wav|wma|ogg|flac]$",Qt::CaseInsensitive))){
        QMessageBox::warning(this,"Format non supporté par Tamise","Le fichier "+chemin->text()+"est introuvable");
        return;
    }
    if(pageListePos->count()>0 && gearListePos->count()>0)
        emit creaGearButton(getValues(),VPtr<QWidgetPage>::varToPtr(pageListePos->currentData()),(gearListePos->currentIndex()+((apres->currentText()=="Après")?1:0)));
    else if(pageListePos->count()>0)
        emit creaGearButton(getValues(),VPtr<QWidgetPage>::varToPtr(pageListePos->currentData()),0);
    else
        emit creaGearButton(getValues());

}
void GearDialog::on_toolButton_clicked(){
    QDir::setCurrent (QDir::homePath());
    chemin->setText(QFileDialog::getOpenFileName());
    if(!chemin->text().contains(QRegExp(".[mp3|wav|wma|ogg|flac]$"))){
        QMessageBox::warning(this,"Format non supporté par Tamise","Le fichier "+chemin->text()+" ne possede pas un format de fichier compatible avec TaMiSe");
    }
}
void GearDialog::on_gearListe_activated(int index){
    if(gearListe->count()==0){
        edit->setEnabled(false);
        del->setEnabled(false);
        return;
    }
    this->setValues(VPtr<Gear>::varToPtr(gearListe->itemData(gearListe->currentIndex()))->getValues());
    edit->setEnabled(true);
    del->setEnabled(true);
}
void GearDialog::addGearToComboBox(Gear *gear,int position){
    if(position==-1){
        gearListe->addItem(gear->getValues().attribute("nom"),VPtr<Gear>::ptrToVar(gear));
    }
    else{
        gearListe->insertItem(position,gear->getValues().attribute("nom"),VPtr<Gear>::ptrToVar(gear));
    }
    gearListe->setCurrentIndex(gearListe->count()-1);
}
void GearDialog::removeFromComboBox(Gear *gear){
    gearListe->removeItem(gearListe->findData(VPtr<Gear>::ptrToVar(gear)));
}
void GearDialog::clear(){
    gearListe->clear();
    on_Reset_clicked();
}
void GearDialog::on_edit_clicked(){
    VPtr<Gear>::varToPtr(gearListe->currentData())->setValues(this->getValues());
}
void GearDialog::on_del_clicked(){
    QWidgetPage *tempP = VPtr<QWidgetPage>::varToPtr(pageListe->currentData());
    Gear *tempG = VPtr<Gear>::varToPtr(gearListe->currentData());

    tempP->layout()->removeWidget(tempG);
    tempP->removeFromGearList(tempG);

    tempG->deleteLater();

    on_pageListe_activated();
    on_pageListePos_activated();

    on_gearListe_activated();
}
void GearDialog::addPageToComboBox(QWidgetPage *page,int position){
    if(position<0){
        pageListe->addItem(page->getValues().attribute("nom"),VPtr<QWidgetPage>::ptrToVar(page));
        pageListePos->addItem(page->getValues().attribute("nom"),VPtr<QWidgetPage>::ptrToVar(page));
    }
     else{
        pageListe->insertItem(position,page->getValues().attribute("nom"),VPtr<QWidgetPage>::ptrToVar(page));
        pageListePos->insertItem(position,page->getValues().attribute("nom"),VPtr<QWidgetPage>::ptrToVar(page));
    }
    pageListe->setCurrentIndex(pageListe->count()-1);
    on_pageListe_activated(pageListe->currentIndex());
}
void GearDialog::setCurrentIndex(int index){
//    pageListe->setCurrentIndex(pageListe->count()-1);
    on_pageListe_activated(pageListe->currentIndex());
    pageListePos->setCurrentIndex(pageListe->currentIndex());
    on_pageListePos_activated(pageListePos->currentIndex());
    on_gearListe_activated();
}
void GearDialog::on_pageListe_activated(int index){
    gearListe->clear();

    if(pageListe->count() == 0)
        return;

    QList<Gear*> liste = VPtr<QWidgetPage>::varToPtr(pageListe->itemData(pageListe->currentIndex()))->getGears();
    for(int i=0;i<liste.length();i++){
        gearListe->addItem(liste.at(i)->getValues().attribute("nom"),VPtr<Gear>::ptrToVar(liste.at(i)));
    }
    pageListePos->setCurrentIndex(pageListe->currentIndex());
    on_pageListePos_activated(pageListePos->currentIndex());
}
void GearDialog::on_pageListePos_activated(int index){
    gearListePos->clear();

    if(pageListePos->count()==0)
        return;

    QList<Gear*> liste = VPtr<QWidgetPage>::varToPtr(pageListePos->currentData())->getGears();
    for(int i=0;i<liste.length();i++)
        gearListePos->addItem(liste.at(i)->getValues().attribute("nom"),VPtr<Gear>::ptrToVar(liste.at(i)));
    gearListePos->setCurrentIndex(gearListePos->count()-1);
}
void GearDialog::removePageFromComboBox(QWidgetPage *page){
    pageListe->removeItem(pageListe->findData(VPtr<QWidgetPage>::ptrToVar(page)));
    pageListePos->removeItem(pageListePos->findData(VPtr<QWidgetPage>::ptrToVar(page)));
    on_pageListe_activated();
    on_pageListePos_activated();

}
void GearDialog::updateComboBoxPageListe(QList<QWidgetPage*> list){
    pageListe->clear();
    pageListePos->clear();
    for(int i=0;i<list.length();i++){
        QString pageNom = list.at(i)->getValues().attribute("nom");
        QVariant pageVar = VPtr<QWidgetPage>::ptrToVar(list.at(i));
        pageListe->addItem(pageNom,pageVar);
        pageListePos->addItem(pageNom,pageVar);
    }


}
