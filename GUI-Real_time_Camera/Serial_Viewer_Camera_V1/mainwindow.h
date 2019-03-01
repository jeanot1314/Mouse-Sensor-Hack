#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QtGui>
#include <QFrame>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QMainWindow>
//#include <QWidget>
//#include "widget.h"
#include "mythread.h"


/*namespace Ui {
class MainWindow;
}*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    //explicit MainWindow(QWidget *parent = nullptr);
    //~MainWindow();


public slots :
        void changerValeur(int i);
        void calculTermine();

private:
    //Ui::MainWindow *ui;
    //Widget *gobject;
    //const QString msgs[2] = {"Hello","World"};
    //int textMode;
    //int rectMode;
    QProgressBar *barre;
    QLabel *label;
    QFrame *ligne;
    QPushButton *bouton;

    MyThread *thread;
};

#endif // MAINWINDOW_H


