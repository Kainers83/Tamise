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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <QFontDatabase>
#include <QTextEdit>
#include <QTime>
#include <QDate>
#include <QStringList>
#include <QSettings>
#include <QProgressDialog>
#include <QTimer>
#include <QtNetwork>
#include <QtXml/QtXml>
#include <QInputDialog>

#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif


#include "ui_mainwindow.h"

#include "configuration.h"
#include "gear.h"
#include "pages.h"
#include "widget/QWidgetPage.h"
#include "widget/QProgressLoad.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, private Ui::MainWindowDesign{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum enumInfos{
        Erreur = -1,
        Infos_Debut = 0,
        Infos_Chargement = 1,
    };  
    enum SaveMode{
        Liste = 0,
        Projet = 1
    };

public slots:
    void on_actionA_Propos_triggered();
    void on_actionAjouter_triggered();
    void on_actionCacher_Panneau_triggered();
    void on_actionCharger_triggered();
    void on_actionConduite_Scenique_triggered();
    void on_actionCompteur_triggered();
    void on_actionPages_triggered();
    void on_actionEnregistrer_triggered();
    void on_actionEnregistrer_Liste_triggered();
    void on_actionEnregister_Projet_triggered();
    void on_actionInformations_triggered();
    void on_actionNouvelle_triggered();

    void on_actionOptions_triggered();

    void on_actionPlein_Ecran_toggled(bool checked);

    void on_optionsStyleCombo_activated(QString text);
    void on_optionGearsMax_valueChanged(int value);
    void on_optionOngletsMax_valueChanged(int value);
    void on_optionDefaultWin_toggled(bool value);
    void on_optionLastWin_toggled(bool value);
    void on_optionFS_toggled(bool value);
    void on_optionCustomWin_toggled(bool value);
    void on_optionCustWinDimX_valueChanged(int value);
    void on_optionCustWinDimY_valueChanged(int value);
    void on_optionCustWinPosX_valueChanged(int value);
    void on_optionCustWinPosY_valueChanged(int value);
    void on_optionCustWinButton_clicked();
    void on_compteurSet_clicked();
    void on_compteurReset_clicked();
    void on_compteurModeCo_toggled();
    void on_compteurModeCh_toggled();
    void on_compteurplus30s_clicked();
    void on_compteurmoins30s_clicked();
    void updateChronoLCD(int time=-1);

    void on_actionUpdate_triggered();
    void updateReadData();
    void nextGear(Gear *gear);

    void on_actionNextYes_triggered();
    void on_actionNextNo_triggered();

    void setPageVolume(int vol);

    void on_action0_triggered();
    void on_action25_triggered();
    void on_action50_triggered();
    void on_action75_triggered();
    void on_action100_triggered();
	
    void on_menuDriver_Audio_triggered();
    void on_menubar_triggered();

    void loadfile(QString file="");

    Gear* createGear(QString file, QWidgetPage *page);
    Gear* createGear(QDomElement element);
    Gear* createGear(QDomElement element, QWidgetPage *page, int position=-1);

    QWidgetPage* createPage(QString nom="", int position=-1);
    QWidgetPage* createPage(QDomElement element, int position=-1);
    bool deletePage(QWidgetPage *page);
    bool deletePages();

    void changeDriver(QAction *action);

    void error(QString err);
    void updateInfosConsole(enumInfos num,QString text = "");
    void setCSS(QString nom = "");
    void updateCSS();
    void compteurChronoStart();
    void compteurChronoUpdate();

signals:
    void infos(enumInfos,QString="");


private:
    //membres
    void Enregistrer(SaveMode mode, QString sauvegarde = "", QString pfile ="", QString ppath="");
    bool eventFilter(QObject *obj, QEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    bool isProject;
    int gearCount;
    QString ProjectName;
    QString ProjectPath;
    QTimer* compteurInterval;
    int compteurMax;
    int compteurSeconds;
    bool compteurState;

    //Instances IHM
    GearArrange *arrange;
    QList<QAction*> gearActionDeleteList;
    QList<QAction*> gearActionEditList;
    QTextEdit *toolBoxInfos;
    QActionGroup *driversActions;
    QRect DefaultWin;
    QRect LastWin;
    QRect CustWin;

    //Instances Données
    QString softName;
    QString softVersion;
    QString softAuteur;
    QString softCompil;
    QString softSite;
    QString extension;
    QStringList listFilter;
    QStringList drv_list;

    QString session;

    QString appFolderName;
    QString appFileIni;
    QString appFolderPref;
    QString appFolderStyle;
    QString appFolderStyleCurrent;
    QString appFolderSession;
    QString appFolderMedias;


    QNetworkAccessManager *qnm;
    QNetworkReply *qnr;


};
#endif // MAINWINDOW_H
