#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QSettings>

#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif


class Configuration : public QObject {
private:
    static QString appFileIni;
public:

    static void setIniFile(QString file);
    template<typename type> static type load(QString section,QString para,type defaut){
        QSettings config(appFileIni, QSettings::IniFormat);
        config.beginGroup(section);
        if(!config.value(para).isValid())
            Configuration::save(section,para,defaut);
        type value = config.value(para).value<type>();
        config.endGroup();
        return value;
    }
    template<typename type> static void save(QString section,QString para,type value){
        QSettings config(appFileIni, QSettings::IniFormat);
        config.beginGroup(section);
        config.setValue(para,value);
        config.endGroup();
    }
};
template <class T> class VPtr
{
public:
    static T* varToPtr(QVariant v)
    {
    return  (T *) v.value<void *>();
    }

    static QVariant ptrToVar(T* ptr)
    {
    return qVariantFromValue((void *) ptr);
    }
};
#endif // CONFIG_H
