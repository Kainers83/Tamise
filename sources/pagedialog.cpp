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
#include "pagedialog.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
PageDialog::PageDialog(QWidget *parent) : QWidget(parent){

    setupUi(this);
}

PageDialog::~PageDialog(){

}
/**************************************************************************************************
Acesseurs
**************************************************************************************************/
QDomElement PageDialog::getValues(){
    QDomDocument doc;
    QDomElement tab = doc.createElement("tab");
    tab.setAttribute("nom",pageNom->text());
    tab.setAttribute("max",maxG->value());
    tab.setAttribute("R",colorR->value());
    tab.setAttribute("G",colorG->value());
    tab.setAttribute("B",colorB->value());

    return tab;

}
void PageDialog::setValues(QDomElement a){
    pageNom->setText(a.attribute("nom"));
    maxG->setValue(a.attribute("max").toInt());
    colorR->setValue(a.attribute("R").toInt());
    colorG->setValue(a.attribute("G").toInt());
    colorB->setValue(a.attribute("B").toInt());
}

/**************************************************************************************************
Slots
**************************************************************************************************/

void PageDialog::on_reset_clicked(){
    pageNom->setText("");
    maxG->setValue(16);
    colorR->setValue(0);
    colorG->setValue(0);
    colorB->setValue(0);
}
void PageDialog::on_add_clicked(){
    if(pageNom->text().isEmpty()){
       QMessageBox::warning(this,"Nom de l'Onglet vide","L'Onglet n'a pas encore de titre.",QMessageBox::Ok);
        return;
    }
    emit creatPageButton(getValues());
}
void PageDialog::on_pageList_activated(int index){
    setValues(VPtr<QWidgetPage>::varToPtr(pageList->currentData())->getValues());
}
void PageDialog::on_edit_clicked(){
    if(pageList->count()<=0)
        return;
    if(pageNom->text().isEmpty()){
       QMessageBox::warning(this,"Nom de l'Onglet vide","L'Onglet n'a pas encore de titre.",QMessageBox::Ok);
        return;
    }
    QWidgetPage *page = VPtr<QWidgetPage>::varToPtr(pageList->currentData());
    page->setValues(getValues());
    emit changePageName(page,pageNom->text());

}
void PageDialog::on_del_clicked(){
    emit delPageButton(VPtr<QWidgetPage>::varToPtr(pageList->currentData()));
    pageList->removeItem(pageList->currentIndex());
}
void PageDialog::addPageToComboBox(QWidgetPage *page,int position){
    if(position>-1)
        pageList->insertItem(position,page->getValues().attribute("nom"),VPtr<QWidgetPage>::ptrToVar(page));
    else
        pageList->addItem(page->getValues().attribute("nom"),VPtr<QWidgetPage>::ptrToVar(page));
}
void PageDialog::removePageFromComboBox(QWidgetPage *page){
    pageList->removeItem(pageList->findData(VPtr<QWidgetPage>::ptrToVar(page)));

}
void PageDialog::updateComboBoxPageListe(QList<QWidgetPage*> list){
    pageList->clear();
    for(int i=0;i<list.length();i++){
        pageList->addItem(list.at(i)->getValues().attribute("nom"),VPtr<QWidgetPage>::ptrToVar(list.at(i)));
    }
}
