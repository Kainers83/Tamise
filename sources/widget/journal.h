#ifndef JOURNAL_H
#define JOURNAL_H

#include <QTextEdit>


#ifdef QT_NO_DEBUG
    #define  tamdebug(msg)
#else
    #include <QDebug>
    #define  tamdebug(msg)    qDebug(qPrintable(msg));
#endif

class Journal : public QTextEdit
{
    Q_OBJECT
public:
    explicit Journal(QWidget *parent = 0);

signals:

public slots:
    void critical(int code, QString text = "");
    void error(int code, QString text = "");
    void warning(int code, QString text = "");
    void info(int code, QString text = "");
};

#endif // JOURNAL_H
