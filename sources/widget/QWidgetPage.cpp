#include "QWidgetPage.h"

QWidgetPage::QWidgetPage(QString titre, QWidget *parent) :  QWidget(parent){
    QUuid genId;
    pageId = genId.createUuid().toString();
    nom = titre;
    max=10;
}
QWidgetPage::~QWidgetPage(){


}
QString QWidgetPage::getPageId(){
    return pageId;
}
QDomElement QWidgetPage::getValues(int mode){
    QDomDocument doc;
    QDomElement tab = doc.createElement("tab");
    tab.setAttribute("nom",nom);
    tab.setAttribute("max",max);

    return tab;
}
void QWidgetPage::setValues(QDomElement a){
    nom = a.attribute("nom");
    max = a.attribute("max","0").toInt();
    setObjectName(nom);
}
void QWidgetPage::setNom(QString name){
    if(!name.isEmpty())
        nom=name;
}
void QWidgetPage::addToGearList(Gear *gear, int position){
    if(position>-1)
        gearList.insert(position,gear);
    else
        gearList << gear;
}
void QWidgetPage::removeFromGearList(Gear *gear){
    gearList.removeOne(gear);
}

QList<Gear*> QWidgetPage::getGears(){
    return gearList;
}
