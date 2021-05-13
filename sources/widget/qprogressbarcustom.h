#ifndef QPROGRESSBARCUSTOM_H
#define QPROGRESSBARCUSTOM_H

#include <QObject>
#include <QWidget>
#include <QProgressBar>
#include <QInputDialog>
#include <QMouseEvent>
#include <QDebug>

class QProgressBarCustom : public QProgressBar
{
    Q_OBJECT

public:
    QProgressBarCustom(QWidget *parent);

signals:
    void doubleClicked(int);

public slots:

protected:
    void mouseDoubleClickEvent( QMouseEvent *e );
    void  mousePressEvent(QMouseEvent *e);
};

#endif // QPROGRESSBARCUSTOM_H
