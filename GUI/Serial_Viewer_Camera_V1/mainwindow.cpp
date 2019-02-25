
//#include <QtGui>
//#include "widget.h"
#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QVBoxLayout>

//#define MAX_SDCX  20

/*MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)*/
MainWindow::MainWindow()
{
    //ui->setupUi(this);
    /*gobject = new Widget(this);
    textMode=1;
    rectMode=1;
    gobject->setPrintData(msgs[textMode]);*/

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(dataOnAir()));
    //timer->start(10);

    barre = new QProgressBar;
    label = new QLabel;
        label->setText("Test de thread");

    ligne = new QFrame;
        ligne->setFrameShape(QFrame::HLine);
        ligne->setFrameShadow(QFrame::Sunken);
    bouton = new QPushButton("Lancer le thread");

    QVBoxLayout *layout = new QVBoxLayout;
        //layout->addWidget(barre);
        layout->addWidget(label);
        layout->addWidget(ligne);
        layout->addWidget(bouton);

    setLayout(layout);
    thread = new MyThread();

    //thread->start();
    //connect(bouton, SIGNAL(clicked()), thread, SLOT(start()));
    connect(thread, SIGNAL(valueChanged(int)), this, SLOT(changerValeur(int)));
    connect(thread, SIGNAL(complete()), this, SLOT(calculTermine()));

    qDebug()<<"---Start---";

    setWindowTitle(tr("Rect Shaking"));
}

/*MainWindow::~MainWindow()
{
    delete ui;
}*/


void MainWindow::changerValeur(int i)
{
    barre->setValue(i);
}

void MainWindow::calculTermine()
{
    label->setText("Calcul Terminé");
}


