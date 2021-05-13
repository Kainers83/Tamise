#ifndef QWIDGETPAGE_H
#define QWIDGETPAGE_H

#include <QWidget>
#include <QtXml>
#include <QUuid>
#include <QList>


#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif


#include "gear.h"


class QWidgetPage : public QWidget{
    Q_OBJECT

public:
    explicit QWidgetPage(QString titre, QWidget *parent=0);
    ~QWidgetPage();
    QString getPageId();
    QDomElement getValues(int mode = 0);
    void setValues(QDomElement a);
    void setNom(QString nom);

    void addToGearList(Gear *gear, int position=-1);
    void removeFromGearList(Gear *gear);
    QList<Gear*> getGears();



private:
    QString pageId;
    QString nom;
    int max;
    QList<Gear*> gearList;
};

#endif
