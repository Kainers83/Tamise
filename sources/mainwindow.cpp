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
#include "mainwindow.h"

/**************************************************************************************************
Constructeurs et destructeurs
**************************************************************************************************/
MainWindow::MainWindow(QWidget *parent){

    /*
     Version: w.x.y
         w != w-1 : Version de la release
         x != 0   : Ajout Fonctionnalitée
         y != 0   : Correction Bug/Ajout Mineurs
    */

    //Données diverses du Logiciel
    softName = "TaMiSe";
    softVersion = "0.5.6";
#ifndef QT_NO_DEBUG
    softVersion += " Debug";
#endif
    softCompil = QString::fromLocal8Bit(__DATE__) + " à " + QString::fromLocal8Bit(__TIME__);
    softAuteur = "LE GOUIL Kerwan";
    softSite = "http://tamise.naeo.fr";

    qnr=NULL;
    qnm=NULL;

    //Creation des Dossiers de l'Applications
    appFolderName = "." + softName.toLower();
    appFolderPref = QDir::homePath() + "/" + appFolderName;
    appFileIni = appFolderPref + "/" + "config.ini" ;

    Configuration::setIniFile(appFileIni);
    appFolderStyle = "styles";
    appFolderSession = "listes";
    appFolderMedias = "medias";

    QDir::home().mkdir(appFolderName);
    QDir::setCurrent(appFolderPref);
    QDir::current().mkdir(appFolderMedias);
    QDir::current().mkdir(appFolderSession);
    QDir::current().mkdir(appFolderStyle);

    appFolderStyle = appFolderPref + "/" + appFolderStyle;
    appFolderSession = appFolderPref + "/" + appFolderSession;
    appFolderMedias = appFolderPref + "/" + appFolderMedias;

    extension = ".tms";
    listFilter << "*.ogg" << "*.mp3" << "*.wav" << "*.wma" << "*.flac";

    //Creation et Gestion de la fenetre principale
    this->setupUi(this);
    this->setParent(parent);
    this->installEventFilter(this);
    this->setWindowTitle(softName + " - Version " + softVersion);
    this->setWindowIcon(QIcon(":/img/icon.png"));
    Raccourcis->hide();

   //creation du system audio à l'avance pour recuperer la lsite des drivers audio.
    if(Media::getSystem() == NULL){
       Media::createSystem();
    }
    drv_list = Media::getDriverList();
    driversActions = new QActionGroup(this);
    driversActions->setExclusive(true);
    connect(driversActions,SIGNAL(triggered(QAction*)),this,SLOT(changeDriver(QAction*)));

	//Reajustage du Menu pour checker le bon Periph.
    on_menubar_triggered();

    //Chargement des dimensions de la fenetre Principale.
    DefaultWin = QRect(100,100,430,240);
    QString geometrymode = Configuration::load("CONFIGURATIONS","geometryMode",QString("default"));
    CustWin = Configuration::load("CONFIGURATIONS","geometryCustomXY",DefaultWin);
    LastWin = Configuration::load("CONFIGURATIONS","geometryLastXY",DefaultWin);

    optionCustWinDimX->setValue(CustWin.width());
    optionCustWinDimY->setValue(CustWin.height());
    optionCustWinPosX->setValue(CustWin.x());
    optionCustWinPosY->setValue(CustWin.y());

    if(geometrymode == "fullscreen"){
        optionFS->setChecked(true);
        actionPlein_Ecran->trigger();
    }
    else if(geometrymode == "custom"){
        optionCustomWin->setChecked(true);
        this->setGeometry(CustWin);
    }
    else if (geometrymode == "lastwin"){
        optionLastWin->setChecked(true);
        this->setGeometry(LastWin);
    }
    else{
        this->setGeometry(QRect(100,100,430,240));
    }

    qApp->setApplicationName(softName);
    qApp->setOrganizationName(softAuteur);
    qApp->setApplicationVersion(softVersion + "|" + softCompil);
    QFontDatabase::addApplicationFont(":/fonts/logo.ttf");

    //Initialisation des Instances.
    toolBoxInfos = new QTextEdit(this);
    arrange = new GearArrange(GearArrange::Onglet, this);

    //Configuration des Widgets de la fenetre principale
    TitreSoft->setText("<b>" + softName + "</b> " + softVersion);
    TitreSoft->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    TitreSoft->setTextFormat(Qt::RichText);
    TitreSoft->setFont(QFont("1942 report"));
    arrange->hide();
    toolBox->hide();
    toolBox->setMaximumWidth(400);

    optionsStyleCombo->addItem("Tamise");
    optionsStyleCombo->addItem("Système");
    optionsStyleCombo->addItem("Noir");

    optionsStyleCombo->addItems(QDir(appFolderStyle).entryList(QDir::NoDot|QDir::NoDotDot|QDir::Dirs));
    {
        QString appStyle = Configuration::load("CONFIGURATIONS","style",QString("black"));
        for(int i=0;i<optionsStyleCombo->count();i++){
            if(optionsStyleCombo->itemText(i) == appStyle)
                optionsStyleCombo->setCurrentIndex(i);
        }
        on_optionsStyleCombo_activated(appStyle);
    }
    layout1->addWidget(arrange);

    //Conception de la toolbox
    toolBox->addItem(toolBoxInfos,"Console d'Infos");

    //Initialisation première du Compteur Chronometre
    compteurSeconds = Configuration::load("CONFIGURATIONS","tempsChrono",3600);
    if(Configuration::load("CONFIGURATIONS","compteurModeChrono",false))
        compteurModeCh->setChecked(true);
    compteurMax=compteurSeconds;
    compteurProgBar->setMaximum(compteurSeconds);
    compteurProgBar->setValue(compteurSeconds);
    updateChronoLCD(compteurSeconds);
    compteurState = false;
    compteurInterval = new QTimer(this);
    compteurInterval->setInterval(1000);

    //Connection des Signaux et Slots

    connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(this,SIGNAL(infos(enumInfos,QString)),this,SLOT(updateInfosConsole(enumInfos,QString)));

    connect(toolBoxControleurs,SIGNAL(creaGearButton(QDomElement,QWidgetPage*,int)),this,SLOT(createGear(QDomElement,QWidgetPage*,int)));
    connect(toolBoxControleurs,SIGNAL(creaGearButton(QDomElement)),this,SLOT(createGear(QDomElement)));

    connect(toolBoxPage,SIGNAL(creatPageButton(QDomElement)),this,SLOT(createPage(QDomElement)));
    connect(toolBoxPage,SIGNAL(delPageButton(QWidgetPage*)),this,SLOT(deletePage(QWidgetPage*)));

    connect(toolBoxPage,SIGNAL(changePageName(QWidgetPage*,QString)),arrange,SLOT(changePageName(QWidgetPage*,QString)));

    connect(compteurInterval, SIGNAL(timeout()), this, SLOT(compteurChronoUpdate()));
    connect(compteurStart,SIGNAL(clicked()),this, SLOT(compteurChronoStart()));

    emit infos(MainWindow::Infos_Debut);

    if(QCoreApplication::arguments().count()>1){
        QProgressLoad dialog(this);
        dialog.setPageMax(QCoreApplication::arguments().count()-1);

        for(int i=1;i<QCoreApplication::arguments().count();i++){
//            on_actionCharger_triggered(QCoreApplication::arguments().at(i));


            QString url = QCoreApplication::arguments().at(i);
            QFileInfo infos(url);
            if(infos.isDir()){
                dialog.setPageLabelText(url);
                dialog.setPageValue(i);
                dialog.show();
                QWidgetPage *temp = createPage(infos.completeBaseName());
                QDirIterator dirIterator(infos.filePath(), listFilter ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
                QStringList liste;
                while(dirIterator.hasNext()){
                    liste << dirIterator.next();
                }
                dialog.setGearMax(liste.length());
                for(int j=0;j<liste.length();j++){
                    dialog.setGearLabelText(liste.at(j));
                    dialog.setGearValue(j);
                    if(!createGear(liste.at(j),temp))
                        return;
                    QCoreApplication::processEvents();
                }
            }
            else if(infos.isFile() &&  listFilter.contains("*."+infos.suffix())){
                if(!arrange->getPageListe().isEmpty()){
                   if(!createGear(url,arrange->getCurrentWidgetPage()))
                       return;
                }
                else
                    if(!createGear(url,createPage()))
                        return;
            }
            else if(infos.isFile() && infos.suffix().contains("tms")){
                  loadfile(url);
            }



        }
    }
}
MainWindow::~MainWindow(){
    if(!this->isFullScreen()){
        Configuration::save("CONFIGURATIONS","geometryLastXY",this->geometry());
    }
    if(!session.isEmpty())
        Configuration::save("CONFIGURATIONS","LastSession",this->session);


}
/**************************************************************************************************
    Gestion des Gears même
**************************************************************************************************/
void MainWindow::nextGear(Gear *gear){
    for(int i=0;i<arrange->getPageListe().length();i++){
        int index = arrange->getPageListe().at(i)->getGears().indexOf(gear);
        if(index>=0 && index < arrange->getPageListe().at(i)->getGears().length()-1)
            arrange->getPageListe().at(i)->getGears().at(index+1)->play();
        else if(index == arrange->getPageListe().at(i)->getGears().length())
            arrange->getPageListe().at(0)->getGears().at(index+1)->play();

    }
}
Gear* MainWindow::createGear(QString file,QWidgetPage *page){
        QFileInfo infos(file);
        QDomDocument doc;
        QDomElement gear = doc.createElement("gear");
        gear.setAttribute("fichier",file);
        gear.setAttribute("nom",infos.completeBaseName());
        return createGear(gear,page);
}
Gear* MainWindow::createGear(QDomElement element){

    if(arrange->getPageListe().isEmpty() && element.attribute("pid").isEmpty()){
        QDomDocument doc;
        QDomElement a = doc.createElement("tab");
        a.setAttribute("nom","ONGLET "+QString::number(arrange->getPageListe().length()+1));
        createPage(a);
        return createGear(element, arrange->getPageListe().last());
    }
    else{
       return createGear(element, arrange->getPageListe().at(arrange->currentIndex()));
    }

    return NULL;
}
Gear* MainWindow::createGear(QDomElement element,QWidgetPage *page, int position){
    //Pour le moment, limitation technique dut au fait que je peux pas aller au dela de 384*2 Threads... Voir si j'ai pas moyen de supprimer les Thread ou de les reunir en un seul?
    //if(gearList.length()>384)
    //    return;
    if(arrange->gearCount()>=380){
        QMessageBox::warning(this,
                             "Nombre de controleur maximum atteint",
                             "Impossible de rajouter un controleur à l'onglet selectionné:<br /><b>"+page->getValues().attribute("nom")+"</b><br /><br />"+
                             "Nombre de Controleur Maximum atteint pour le logiciel"
                             );
        return NULL;
    }
    gearCount++;
    if(arrange->getPageListe().isEmpty() && element.attribute("pid").isEmpty())
        createPage("ONGLET "+QString::number(arrange->getPageListe().length()+1));

    if(page->getValues().attribute("max").toInt() >0 && page->getGears().length() == page->getValues().attribute("max").toInt()){
        QMessageBox::warning(this,
                             "Nombre de controleur maximum atteint",
                             "Impossible de rajouter un controleur à l'onglet selectionné:<br /><b>"+page->getValues().attribute("nom")+"</b><br /><br />"+
                             "Nombre de Controleur Maximum atteint pour cet onglet ("+page->getValues().attribute("max")+").<br /><br />"+
                             "Pour pourvoir rajouter un controleur, modifier la valeur maximum dans les proprietés de l'onglet"
                             );
        return NULL;

    }

    TitreSoft->hide();
    arrange->show();

    Gear *temp = new Gear(element);

    arrange->addGear(temp,page,position);
    page->addToGearList(temp,position);
    toolBoxControleurs->setCurrentIndex();
    connect(temp,SIGNAL(error(QString)),this,SLOT(error(QString)));
    connect(temp,SIGNAL(nextGear(Gear*)),this, SLOT(nextGear(Gear*)));

    emit infos(Infos_Chargement,element.attribute("nom"));

    temp->setStyleSheet(qApp->styleSheet());
    return temp;
}
QWidgetPage* MainWindow::createPage(QString nom, int position){
    QDomDocument doc;
    QDomElement tab = doc.createElement("tab");
    tab.setAttribute("nom",(nom.isEmpty())?"ONGLET "+QString::number(arrange->getPageListe().length()+1):nom);
    return createPage(tab,position);
}
QWidgetPage *MainWindow::createPage(QDomElement element, int position){
    TitreSoft->hide();
    arrange->show();
    //if(!this->isFullScreen())
        //this->setGeometry(Configuration::load("CONFIGURATIONS","geometry",QRect(100,100,430,240)));
    arrange->newPage(element);
    //toolBoxPage->addPageToComboBox(arrange->getPageListe().last(),position);
    //toolBoxControleurs->addPageToComboBox(arrange->getPageListe().last(),position);
    toolBoxControleurs->updateComboBoxPageListe(arrange->getPageListe());
    toolBoxPage->updateComboBoxPageListe(arrange->getPageListe());
    return arrange->getPageListe().last();
}
bool MainWindow::deletePage(QWidgetPage *page){
    arrange->deletePage(page);
    toolBoxControleurs->removePageFromComboBox(page);
    if(arrange->getPageListe().isEmpty()){
        on_actionNouvelle_triggered();
    }
}
bool MainWindow::deletePages(){
    //CA DOIT PAS ETRE MIEUX
    while(!arrange->getPageListe().isEmpty()){
        deletePage(arrange->getPageListe().last());
    }
}
/**************************************************************************************************
    Acceseurs de Gestion Console (Save+New+Load)
**************************************************************************************************/

void MainWindow::on_actionCharger_triggered(){
  loadfile();
}
void MainWindow::on_actionNouvelle_triggered(){
    if(arrange->getPageListe().length()>0){
        int ok = QMessageBox::question(this,"Creation d'une nouvelle Session?",
                                       "Crée une nouvelle session<br /> effacera toutes les données actuelle",
                                       QMessageBox::Ok|QMessageBox::Cancel);
        if(ok == QMessageBox::Cancel)
            return;
        deletePages();
        toolBoxControleurs->clear();
    }
    session = "";
    ProjectName = "";
    ProjectPath = "";
    setWindowTitle(softName + " - Version: " + softVersion);
    toolBoxConduite->reset();
    toolBox->hide();
    arrange->hide();
    TitreSoft->show();
}
/**************************************************************************************************
Enregistrement et chargement
**************************************************************************************************/
void MainWindow::on_actionEnregistrer_triggered(){
    if(ProjectName!="")
        Enregistrer(MainWindow::Projet,session,ProjectName,ProjectPath);
    else
        Enregistrer(MainWindow::Liste,session);
}
void MainWindow::on_actionEnregister_Projet_triggered(){
    Enregistrer(MainWindow::Projet);
}
void MainWindow::on_actionEnregistrer_Liste_triggered(){
    Enregistrer(MainWindow::Liste);
}
void MainWindow::Enregistrer(SaveMode mode, QString sauvegarde,QString pfile,QString ppath){
    QDir::setCurrent(appFolderSession);
    QString ProjetFile = pfile;
    QString ProjetDir = ppath;

     if(mode==MainWindow::Projet){
         if(sauvegarde.isEmpty()){
             if(ProjetFile.isEmpty())
                ProjetFile = QInputDialog::getText(this, "Sous quel nom voulez vous enregistrer votre projet?",
                                                         "Nom du Projet:", QLineEdit::Normal);
             if(ProjetFile.isEmpty())
                 return;
             if(ProjetDir.isEmpty())
                ProjetDir = QFileDialog::getExistingDirectory(this, "Selectionner le dossier où sera enregistré le projet "+ProjetFile+" (un nouveau sous dossier sera crée)",
                                                             appFolderSession,
                                                             QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontResolveSymlinks);
             if(ProjetDir.isEmpty())
                 return;
             sauvegarde = ProjetDir+"/"+ProjetFile+"/"+ProjetFile+".tms";
            if(QFile::exists(sauvegarde)){
                 int ok;
                 ok = QMessageBox::question(this,"Le projet existe déjà","le projet existe déjà. Le remplacer?",QMessageBox::Ok|QMessageBox::Cancel);
                 if(ok == QMessageBox::Cancel)
                     return;
             }
            QDir dir(ProjetDir);
            dir.mkdir(ProjetFile);
            dir.cd(ProjetFile);
            QDir::setCurrent(dir.path());
         }
     }
     else if(mode==MainWindow::Liste){
        if(sauvegarde.isEmpty()){
            sauvegarde = QFileDialog::getSaveFileName();
        }
        if(sauvegarde.isEmpty())
            return;
     }
     if(!sauvegarde.contains(QRegExp(extension+"$")))
         sauvegarde.append(extension);
    QFile fichier(sauvegarde);
    if (fichier.open(QFile::WriteOnly)) {

        QDomDocument doc;

        QDomElement tamise = doc.createElement("tamise");
            QDomElement gears = doc.createElement("gears");
            QDomElement conduite = doc.createElement("conduite");


        QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.insertBefore(xmlNode, doc.firstChild());

        doc.appendChild(tamise);
            tamise.appendChild(gears);
            tamise.appendChild(conduite);

        for(int i=0;i<arrange->getPageListe().length();i++){
            QDomElement tab = arrange->getPageListe().at(i)->getValues(1);
            gears.appendChild(tab);
            if(mode==MainWindow::Projet)
                gears.setAttribute("projet",ProjetFile);
            for(int j=0;j < arrange->getPageListe().at(i)->getGears().length();j++){

                QDomElement gearval = arrange->getPageListe().at(i)->getGears().at(j)->getValues(1);
                tab.appendChild(gearval);

                if(mode==MainWindow::Projet){
                  QString filePath = gearval.attribute("fichier");
                  QFileInfo file(filePath);
                  if(!QFile::exists(QDir::currentPath()+"/"+file.fileName())){
                      QFile::copy(filePath,QDir::currentPath()+"/"+file.fileName());
                  }
                  gearval.setAttribute("fichier",file.fileName());
                }
            }
        }

        conduite.appendChild(toolBoxConduite->getDomConduite());

        QTextStream out(&fichier);
        doc.save(out,2);
        fichier.close();
        session = sauvegarde;
        setWindowTitle(softName + " - Version: " + softVersion + " (" + session + ")");
        ProjectName = ProjetFile;
        ProjectPath = ProjetDir;
        toolBoxControleurs->setCurrentIndex();
    }
}
void MainWindow::loadfile(QString file){
    QDir::setCurrent(appFolderSession);
    QString nomFichier;

    if(file.isEmpty()) nomFichier = QFileDialog::getOpenFileName(this, "Choisir un Projet Tamise");
    else nomFichier = file;

    QRegExp regExp(extension+"$");
    if(nomFichier.isEmpty())
        return;
    if(!nomFichier.contains(regExp)){
        QMessageBox::warning(this,
                             "Erreur sur le Type de Fichier",
                             "Le type de fichier choisi n'a pas la bonne extension: '"+extension+"'"
                             );
        return;
    }
    QFile fichier(nomFichier);
    QFileInfo infos(nomFichier);
    QDir::setCurrent(infos.path());

    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QDomDocument dom("tamise");

    QString str = fichier.readAll();
    fichier.close();

    if(!dom.setContent(str,false)){
        QMessageBox::warning(this,
                             "Erreur lors de la lecture du fichier XML",
                             "Le Fichier n'est pas un document XML valide ou est corrompue."
                             );
        return;
    }

    on_actionNouvelle_triggered();

    QDomElement tamise = dom.documentElement();
    QDomNode nodeTamise = tamise.firstChild();

    QProgressLoad dialog(this);
    dialog.show();

    while(!nodeTamise.isNull()){
        QDomElement e1 = nodeTamise.toElement();

        if(!e1.tagName().compare("gears")){
            int actuelpage=0;
            dialog.setPageValue(0);
            ProjectName = e1.attribute("projet");
            if(!ProjectName.isEmpty()){
                QDir dir(infos.path());
                dir.cd("../");
                ProjectPath = dir.path();
            }

            QDomNode n2 = e1.firstChild();
            int pageMax=0;
            while(!n2.isNull()){
                pageMax++;
                n2 = n2.nextSibling();
            }
            dialog.setPageMax(pageMax);

            n2 = e1.firstChild();

            while(!n2.isNull()){
                QDomElement e2 = n2.toElement();
                createPage(e2);
                dialog.setPageLabelText(e2.attribute("nom"));
                dialog.setPageValue(actuelpage++);
                int actuelgear=0;

                QDomNode n3 = e2.firstChild();
                int gearMax=0;
                while(!n3.isNull()){
                    gearMax++;
                    n3=n3.nextSibling();
                }
                dialog.setGearMax(gearMax);

                n3 = e2.firstChild();
                while(!n3.isNull()){
                    QDomElement e3 = n3.toElement();
                    dialog.setGearLabelText(e3.attribute("nom"));
                    if(!ProjectName.isEmpty()){
                        e3.setAttribute("fichier",ProjectPath+"/"+ProjectName+"/"+e3.attribute("fichier"));
                    }
                    if(!createGear(e3,arrange->getPageListe().last())) return;

                    QCoreApplication::processEvents();
                    dialog.setGearValue(actuelgear++);

                    n3=n3.nextSibling();

                }
                QCoreApplication::processEvents();
                n2 = n2.nextSibling();
            }
        }
        else if (!e1.tagName().compare("conduite")){
            QDomNode n2 = e1.firstChild();
            QDomElement e2 = n2.toElement();
            toolBoxConduite->setConduite(e2);
        }
        nodeTamise = nodeTamise.nextSibling();
    }
    session = nomFichier;
    setWindowTitle(softName + " - Version: " + softVersion + " (" + session + ")");
    updateCSS();

}
/**************************************************************************************************
    AutoConnections Slots
**************************************************************************************************/
void MainWindow::on_actionPlein_Ecran_toggled(bool checked){
    (checked)?this->showFullScreen():this->showNormal();
}
void MainWindow::on_actionInformations_triggered(){
    toolBox->show();
    toolBox->setCurrentWidget(toolBoxInfos);
}
void MainWindow::on_actionAjouter_triggered(){
    toolBox->show();
    toolBox->setCurrentWidget(toolBoxControleurs);
}
void MainWindow::on_actionCompteur_triggered(){
    toolBox->show();
    toolBox->setCurrentWidget(toolBoxCompteur);
}
void MainWindow::on_actionConduite_Scenique_triggered(){
    toolBox->show();
    toolBox->setCurrentWidget(toolBoxConduite);
}
void MainWindow::on_actionOptions_triggered(){
    toolBox->show();
    toolBox->setCurrentWidget(toolBoxOptions);
}
void MainWindow::on_actionPages_triggered(){
    toolBox->show();
    toolBox->setCurrentWidget(toolBoxPage);
}
void MainWindow::on_actionUpdate_triggered(){

    if (softVersion.contains(QRegExp("Debug",Qt::CaseInsensitive))){
        QMessageBox::information(0,"Mise à Jour",
                                 "Tamise Version Debug<br />Impossible de mettre à jour cette version.",
                                 QMessageBox::Ok);
        return;
    }
    else{

        if(qnm==NULL) qnm = new QNetworkAccessManager();
        QUrl url(softSite + "/update");
        QNetworkRequest requete(url);

        qnr = qnm->get(requete);
        connect(qnr, SIGNAL(finished()), this, SLOT(updateReadData()));
    }
}
void MainWindow::updateReadData(){

  QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
  QString serveurVersion = r->readAll();
    if(r->error()!= QNetworkReply::NoError){
        QMessageBox::information(0,"Mise à Jour",
                                     "Erreur durant le processus de verification de mise à jour<br /><b>Erreur HTTP " + r->errorString() + "</b>",
                                     QMessageBox::Ok);
    }
    else if(serveurVersion!=softVersion){
        QMessageBox::information(0,"Mise à Jour",
                                 "Une nouvelle Version de Tamise est disponible: <b>" + serveurVersion+"</b><br />"
                                 +"Version actuelle: "+softVersion+"</b><br />"
                                 +"Vous pouvez la telecharger à l'adresse:<br /><a href="+softSite+"/"+serveurVersion+"/setup_TaMiSe_"+serveurVersion+".exe>"+softSite+"/"+serveurVersion+"/setup_TaMiSe_"+serveurVersion+".exe</a>",
                                 QMessageBox::Ok);
    }

    else{
        QMessageBox::information(0,"Mise à Jour",
                                 "Tamise est actuellement à jour",
                                 QMessageBox::Ok);
    }
}
void MainWindow::on_actionCacher_Panneau_triggered(){
    (toolBox->isHidden())?toolBox->show():toolBox->hide();
}
//AUTO CONNECT DES OPTIONS
void MainWindow::on_optionsStyleCombo_activated(QString text){
    if(text==optionsStyleCombo->itemText(0)){
        setCSS(":/css/tamise.css");
        return;
    }
    if(text==optionsStyleCombo->itemText(1)){
        setCSS(":/css/style.css");
        return;
    }
    if(text==optionsStyleCombo->itemText(2)){
        setCSS(":/css/black.css");
        return;

    }

    appFolderStyleCurrent = appFolderStyle + "/" + text;
    QDir::setCurrent (appFolderStyleCurrent);
    setCSS(appFolderStyleCurrent + "/style.css");

    Configuration::save("CONFIGURATIONS","style",text);
    Configuration::save("CONFIGURATIONS","ongletsMax","16");
    Configuration::save("CONFIGURATIONS","gpom","16");

}
void MainWindow::on_optionDefaultWin_toggled(bool value){
    Configuration::save("CONFIGURATIONS","geometryMode","default");
}
void MainWindow::on_optionFS_toggled(bool value){
    Configuration::save("CONFIGURATIONS","geometryMode","fullscreen");
}
void MainWindow::on_optionLastWin_toggled(bool value){
    Configuration::save("CONFIGURATIONS","geometryMode","lastwin");
}
void MainWindow::on_optionCustomWin_toggled(bool value){
    Configuration::save("CONFIGURATIONS","geometryMode","custom");
}
void MainWindow::on_optionCustWinButton_clicked(){
    optionCustWinDimX->setValue(this->geometry().width());
    optionCustWinDimY->setValue(this->geometry().height());
    optionCustWinPosX->setValue(this->geometry().x());
    optionCustWinPosY->setValue(this->geometry().y());
    Configuration::save("CONFIGURATIONS","geometryCustomXY",CustWin);
}
void MainWindow::on_optionOngletsMax_valueChanged(int value){
    Configuration::save("CONFIGURATIONS","ongletsMax",value);
}
void MainWindow::on_optionGearsMax_valueChanged(int value){
    Configuration::save("CONFIGURATIONS","GearsMax",value);
}
void MainWindow::on_optionCustWinDimX_valueChanged(int value){
    CustWin.setWidth(value);
    Configuration::save("CONFIGURATIONS","geometryCustomXY",CustWin);
}
void MainWindow::on_optionCustWinDimY_valueChanged(int value){
    CustWin.setHeight(value);
    Configuration::save("CONFIGURATIONS","geometryCustomXY",CustWin);
}
void MainWindow::on_optionCustWinPosX_valueChanged(int value){
    CustWin.setX(value);
    Configuration::save("CONFIGURATIONS","geometryCustomXY",CustWin);
}
void MainWindow::on_optionCustWinPosY_valueChanged(int value){
    CustWin.setY(value);
    Configuration::save("CONFIGURATIONS","geometryCustomXY",CustWin);

}
void MainWindow::on_actionNextYes_triggered(){
    foreach(QWidgetPage* page,arrange->getPageListe()){
        foreach(Gear* gear,page->getGears()){
            gear->setNext(true);
        }
    }
}
void MainWindow::on_actionNextNo_triggered(){
    foreach(QWidgetPage* page,arrange->getPageListe()){
        foreach(Gear* gear,page->getGears()){
            gear->setNext(false);
        }
    }
}
void MainWindow::setPageVolume(int vol){
    foreach(QWidgetPage* page,arrange->getPageListe()){
        foreach(Gear* gear,page->getGears()){
            gear->setVolume(vol);
        }
    }
}
void MainWindow::on_action0_triggered(){
    setPageVolume(0);
}
void MainWindow::on_action25_triggered(){
    setPageVolume(25);
}
void MainWindow::on_action50_triggered(){
    setPageVolume(50);
}
void MainWindow::on_action75_triggered(){
    setPageVolume(75);
}
void MainWindow::on_action100_triggered(){
    setPageVolume(100);
}
void MainWindow::on_menubar_triggered(){

    foreach(QAction *action, driversActions->actions()){
        menuDriver_Audio->removeAction(action);
        driversActions->removeAction(action);
    }
    int drv_actuel = Media::getCurrentDriverNum();
    QStringList driverList =  Media::getDriverList();

    for(int i=0;i<driverList.length();i++){
        QVariant v = i ;
        QAction *action = driversActions->addAction(driverList.at(i));
        action->setData(v);
        action->setCheckable(true);
        if(drv_actuel == i){
            action->setChecked(true);
        }
    }
    menuDriver_Audio->addActions(driversActions->actions());
}
void MainWindow::on_menuDriver_Audio_triggered(){

}
/**************************************************************************************************
    Autres
**************************************************************************************************/
void MainWindow::changeDriver(QAction *action){
    int drv_new = action->data().toInt();
    Media::setSystemDriver(drv_new, action->iconText());
}
void MainWindow::setCSS(QString nom){
    QDir::setCurrent(appFolderStyleCurrent);
    QFile fichier;
    fichier.setFileName(nom);
    if(!fichier.open(QIODevice::Text | QIODevice::ReadOnly)){
        emit error("Erreur lors de l'ouverture du fichier CSS ");
        return;
    }
    QString file = fichier.readAll();
    fichier.close();

    qApp->setStyleSheet(file);
    for(int i=0; i<arrange->getPageListe().length();i++){
        for(int j=0;j<arrange->getPageListe().at(i)->getGears().length();j++)
            arrange->getPageListe().at(i)->getGears().at(j)->setStyleSheet(file);
    }
}
void MainWindow::updateCSS(){
    QDir::setCurrent(appFolderStyleCurrent);
    qApp->setStyleSheet(qApp->styleSheet());
}
void MainWindow::on_actionA_Propos_triggered(){
    QMessageBox::about(this, "A propos",
                       QString("<table>")
                       +QString("<tr><td align=center style='font-size:20pt;font-family:1942 report;'><b>"+softName+"</b></td><td></td></tr>")
                       +QString("<tr><td></td><td></td></tr>")
                       +QString("<tr><td><b>Version: </b></td><td> ") + softVersion +" - "+ softCompil + QString("</td></tr>")
                       +QString("<tr><td><b>API graphique:</b></td><td><a href=\"http://www.qt.io/\">QT Gui © 2015 The Qt Company")
                       +QString("<tr><td><b>API sonore:</b></td><td><a href=\"http://fmod.org\">FMODex © Firelight Technologies</a></td></tr>")
                       +QString("<tr><td><b>Icone: </b></td><td><a href=http://www.iconarchive.com/show/tulliana-2-icons-by-umut-pulat/k-mix-icon.html>Iconarchive</a></td></tr>")
                       +QString("<tr><td><b>Police:</b></td><td>1942 report <a href=http://megafonts.free.fr>MegaFonts</a></td></tr>")
                       +QString("<tr><td><b>License:</b></td><td><a href=http://www.gnu.org/licenses/gpl-3.0.html>GPL 3.0</a></td></tr>")
                       +QString("<tr><td><b>Copyright:</b></td><td> 2011-2015 ") + softAuteur + QString("</td></tr>")
                       +QString("<tr><td><b>Site:</b></td><td><i><a href="+ softSite +">Site</a></i></td></tr>")
                       +QString("</table>")
                       );
}
void MainWindow::error(QString err){
    updateInfosConsole(MainWindow::Erreur,err);
}
void MainWindow::updateInfosConsole(enumInfos err,QString text){
    QString temp /* = QDate::currentDate().toString("dd/MM/yyyy")+" " + QTime::currentTime().toString("HH:mm:ss") +":" */;
    switch(err){
    case MainWindow::Erreur:
        temp += text;
        break;
    case MainWindow::Infos_Debut:
        temp += "<b>Ouverture de "+softName +" version "+softVersion +"<b/>";
        break;
    case MainWindow::Infos_Chargement:
        temp += "<b>Chargement de :</b>" + text.replace("<br />"," ");
        break;
    }
    toolBoxInfos->append(temp);
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch(keyEvent->key()){
        case Qt::Key_F1:
            on_actionNouvelle_triggered();
            break;
        case Qt::Key_F2:
              loadfile();
            break;
        case Qt::Key_F3:
            on_actionEnregistrer_triggered();
            break;
        case Qt::Key_F4:
            on_actionEnregistrer_Liste_triggered();
            break;
        case Qt::Key_F5:
            toolBox->show();
            toolBox->setCurrentWidget(toolBoxOptions);
            break;
        case Qt::Key_F6:
            toolBox->show();
            toolBox->setCurrentWidget(toolBoxCompteur);
            break;
        case Qt::Key_F7:
            toolBox->show();
            toolBox->setCurrentWidget(toolBoxConduite);
            break;
        case Qt::Key_F8:
            toolBox->show();
            toolBox->setCurrentWidget(toolBoxPage);
            break;
        case Qt::Key_F9:
            toolBox->show();
            toolBox->setCurrentWidget(toolBoxControleurs);
            break;
        case Qt::Key_F10:
            toolBox->show();
            toolBox->setCurrentWidget(toolBoxInfos);
        case Qt::Key_ScrollLock:
            if(actionBarre_de_Menu->isChecked()){
               actionBarre_de_Menu->setChecked(false);
                menubar->show();
            }
             else{
                menubar->hide();
                actionBarre_de_Menu->setChecked(true);
            }
            return true;
            break;
        case Qt::Key_F11:
            actionPlein_Ecran->trigger();
            break;
        case Qt::Key_F12:
            actionCacher_Panneau->trigger();
            return true;
            break;
        case /*Qt::Key_1*/ Qt::Key_Ampersand:
            arrange->setCurrentIndex(0);
            return true;
            break;
        case /*Qt::Key_2*/ Qt::Key_Eacute:
            arrange->setCurrentIndex(1);
            return true;
            break;
        case /*Qt::Key_3*/ Qt::Key_QuoteDbl:
            arrange->setCurrentIndex(2);
            return true;
            break;
        case /*Qt::Key_4*/ Qt::Key_Apostrophe:
            arrange->setCurrentIndex(3);
            return true;
            break;
        case /*Qt::Key_5*/ Qt::Key_ParenLeft:
            arrange->setCurrentIndex(4);
            return true;
            break;
        case /*Qt::Key_6*/ Qt::Key_hyphen:
            arrange->setCurrentIndex(5);
            return true;
            break;
        case /*Qt::Key_7*/ Qt::Key_Egrave:
            arrange->setCurrentIndex(6);
            return true;
            break;
        case /*Qt::Key_8*/ Qt::Key_Underscore:
            arrange->setCurrentIndex(7);
            return true;
            break;
        case /*Qt::Key_9*/ Qt::Key_Ccedilla:
            arrange->setCurrentIndex(8);
            return true;
            break;
        case /*Qt::Key_0*/ Qt::Key_acute:
            arrange->setCurrentIndex(9);
            return true;
            break;
        case Qt::Key_Alt + Qt::Key_1:
            arrange->setCurrentIndex(10);
            return true;
            break;
        case Qt::Key_A:
            if(arrange->getCurrentWidgetPage()->getGears().length()>0)
                arrange->getCurrentWidgetPage()->getGears().at(0)->play();
            break;
        case Qt::Key_Z:
            if(arrange->getCurrentWidgetPage()->getGears().length()>1)
                arrange->getCurrentWidgetPage()->getGears().at(1)->play();
            break;
        case Qt::Key_E:
            if(arrange->getCurrentWidgetPage()->getGears().length()>2)
                arrange->getCurrentWidgetPage()->getGears().at(2)->play();
            break;
        case Qt::Key_R:
            if(arrange->getCurrentWidgetPage()->getGears().length()>3)
                arrange->getCurrentWidgetPage()->getGears().at(3)->play();
            break;
        case Qt::Key_T:
            if(arrange->getCurrentWidgetPage()->getGears().length()>4)
                arrange->getCurrentWidgetPage()->getGears().at(4)->play();
            break;
        case Qt::Key_Y:
            if(arrange->getCurrentWidgetPage()->getGears().length()>5)
                arrange->getCurrentWidgetPage()->getGears().at(5)->play();
            break;
        case Qt::Key_U:
            if(arrange->getCurrentWidgetPage()->getGears().length()>6)
                arrange->getCurrentWidgetPage()->getGears().at(6)->play();
            break;
        case Qt::Key_I:
            if(arrange->getCurrentWidgetPage()->getGears().length()>7)
                arrange->getCurrentWidgetPage()->getGears().at(7)->play();
            break;
        case Qt::Key_O:
            if(arrange->getCurrentWidgetPage()->getGears().length()>8)
                arrange->getCurrentWidgetPage()->getGears().at(8)->play();
            break;
        case Qt::Key_P:
            if(arrange->getCurrentWidgetPage()->getGears().length()>9)
                arrange->getCurrentWidgetPage()->getGears().at(9)->play();
            break;

        }
    }
    return QMainWindow::eventFilter(obj, event);
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
    //if (event->mimeData()->hasFormat("text/plain")) // petit filtre
         event->acceptProposedAction();
     }
}
void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
         QList<QUrl> urlList = event->mimeData()->urls();
         QProgressLoad dialog(this);
         dialog.setPageMax(urlList.size());
         for (int i = 0; i < urlList.size() && i < 32; ++i) {
             QString url = urlList.at(i).path().remove(0,1);
             QFileInfo infos(url);
             if(infos.isDir()){
                 dialog.setPageLabelText(url);
                 dialog.setPageValue(i);
                 dialog.show();
                 QWidgetPage *temp = createPage(infos.completeBaseName());
                 QDirIterator dirIterator(infos.filePath(), listFilter ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
                 QStringList liste;
                 while(dirIterator.hasNext()){
                     liste << dirIterator.next();
                 }
                 dialog.setGearMax(liste.length());
                 for(int j=0;j<liste.length();j++){
                     dialog.setGearLabelText(liste.at(j));
                     dialog.setGearValue(j);
                     if(!createGear(liste.at(j),temp))
                         return;
                     QCoreApplication::processEvents();
                 }
             }
             else if(infos.isFile() &&  listFilter.contains("*."+infos.suffix())){
                 if(!arrange->getPageListe().isEmpty()){
                    if(!createGear(url,arrange->getCurrentWidgetPage()))
                        return;
                 }
                 else
                     if(!createGear(url,createPage()))
                         return;
             }
             else if(infos.isFile() && infos.suffix().contains("tms")){
                   loadfile(url);
             }
         }
     }


     event->acceptProposedAction();
}

//membres de gestion du Chrono/Compteurs
void MainWindow::on_compteurSet_clicked(){
    compteurSeconds = QTime(0,0,0).secsTo(compteurTime->time());
    compteurMax=compteurSeconds;
    Configuration::save("CONFIGURATIONS","tempsChrono",compteurSeconds);

    compteurProgBar->setMaximum(compteurMax);
    compteurProgBar->setValue(compteurSeconds);

    compteurStart->setEnabled(true);
    compteurStart->setText("Demarrer");
    compteurState = false;
    updateChronoLCD(compteurSeconds);
}
void MainWindow::on_compteurReset_clicked(){
    compteurState = false;
    updateChronoLCD(compteurSeconds);
}
void MainWindow::compteurChronoStart(){
    if(!compteurState){
        compteurProgBar->setMaximum(compteurMax);
        compteurProgBar->setValue(compteurSeconds);
        compteurState=true;
        compteurInterval->start();
        compteurStart->setText("Pause");
    }
    else{
        compteurState = false;
        compteurStart->setText("Demarrer");
    }
}
void MainWindow::compteurChronoUpdate(){
    if(compteurState){
        if(compteurSeconds<=0 && compteurModeCo->isChecked()){
            //compteurStart->setEnabled(false);
            compteurH->display("PE");
            compteurM->display("RD");
            compteurS->display("UE");
           // compteurState = false;
            return;
        }
        compteurSeconds--;
        compteurProgBar->setValue(compteurSeconds);
        updateChronoLCD(compteurSeconds);
        compteurInterval->start();
    }
}
void MainWindow::updateChronoLCD(int time){

    if(compteurModeCh->isChecked())
        time=compteurMax-time;

    int h = time/3600;
    int m = time/60 - h*60;
    int s = time - h*3600 -m*60;

    compteurH->display(((h<10)?"0":"")+QString::number(h));
    compteurM->display(((m<10)?"0":"")+QString::number(m));
    compteurS->display(((s<10)?"0":"")+QString::number(s));
}
void MainWindow::on_compteurModeCo_toggled(){
    compteurChronoUpdate();
    Configuration::save("CONFIGURATIONS","compteurModeChrono",false);
}
void MainWindow::on_compteurModeCh_toggled(){
    compteurChronoUpdate();
    Configuration::save("CONFIGURATIONS","compteurModeChrono",true);
}
void MainWindow::on_compteurplus30s_clicked(){
    compteurSeconds+=30;
    compteurChronoUpdate();
}
void MainWindow::on_compteurmoins30s_clicked(){
    compteurSeconds-=30;
    compteurChronoUpdate();
}
