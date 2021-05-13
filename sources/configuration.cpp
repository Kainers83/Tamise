#include "configuration.h"

QString Configuration::appFileIni;

void Configuration::setIniFile(QString file){
    if(appFileIni.isEmpty())
        appFileIni = file;
}
