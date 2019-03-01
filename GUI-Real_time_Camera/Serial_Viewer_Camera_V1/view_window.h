#ifndef VIEW_WINDOW_H
#define VIEW_WINDOW_H


#include <QFrame>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QMainWindow>
#include <QPaintDevice>
#include <QDebug>
#include "mythread.h"
#include <QVBoxLayout>
#include <QtGui>
#include <algorithm>


class view_window : public QMainWindow
{   
    Q_OBJECT

public:
    view_window();


public slots :
        void changerValeur(int i);
        void calculTermine();
        void tab_update(QByteArray data_col);

protected:
        void mousePressEvent(QMouseEvent *event);
        void paintEvent(QPaintEvent *event);

private:
    QPainter *p;
    QPaintDevice *square;
    QProgressBar *barre;
    QLabel *label;
    QFrame *ligne;
    QPushButton *bouton;
    QList<QPoint> m_points;
    MyThread *thread;
    int size;

public:
    QByteArray data_colors;

};

#endif // VIEW_WINDOW_H
