
#include "view_window.h"


view_window::view_window()
{
    //https://openclassrooms.com/forum/sujet/qt-setcentralwidget-not-declared-53278
    //https://openclassrooms.com/forum/sujet/qwidget-setlayout-attempting-to-set-qlayout-90230
    //https://openclassrooms.com/forum/sujet/qthread-ou-pas-41322

 //   QImage image("/Users/chabadao/img.png");
   /* QPainter painter(&image);
    QPen pen;
    pen.setWidth(500);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint(5,5);
    painter.end();*/

    //p = new QPainter;
   // p->drawRect(200, 100, 100, 100);

    size = 0;
    int k;
    for(k=0;k<364;k++){
        data_colors[k]=0;
    }

    barre = new QProgressBar;
    label = new QLabel;
        label->setText("CAM");
      //  label->setPixmap(QPixmap::fromImage(image));


    ligne = new QFrame;
        ligne->setFrameShape(QFrame::HLine);
        ligne->setFrameShadow(QFrame::Sunken);
    bouton = new QPushButton("Start Serial Read");

    QVBoxLayout *layout = new QVBoxLayout;
        //layout->addWidget(pen);
        layout->addWidget(bouton);
        //layout->addWidget(barre);
        layout->addWidget(label);
        layout->addWidget(ligne);
    resize(600, 400);
    setWindowTitle(tr("Camera Viewer"));

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);

    thread = new MyThread();

    thread->start();
    //connect(bouton, SIGNAL(clicked()), thread, SLOT(start()));
    connect(thread, SIGNAL(valueChanged(int)), this, SLOT(changerValeur(int)));
    connect(thread, SIGNAL(complete()), this, SLOT(calculTermine()));
    //connect(thread, SIGNAL(complete()), this, SLOT(calculTermine()));
    //connect(thread, SIGNAL(gotAlbums(QList<QString> &)), this, SLOT(AlbumsReceived(QList<QString> &)));
    connect(thread, SIGNAL(tab_signal(QByteArray)), this, SLOT(tab_update(QByteArray)));

    qDebug()<<"---Start---";


}

/*MainWindow::~MainWindow()
{
    delete ui;
}*/

void view_window::mousePressEvent(QMouseEvent *event)
{
    //m_points.append(event->pos());

    //update();
}

void view_window::paintEvent(QPaintEvent *event)
{
    int i,j, cpt_good_pixel=0;

    QPainter painter(this);
    QPen pen;
    pen.setWidth(14);

    int min=int(data_colors[1]), max=int(data_colors[1]), moy,tmpmoy=int(data_colors[1]);

    for(i=2;i<361;i++){
        if(int(data_colors[1+i]) > max){
            max = int(data_colors[1+i]);
        }
        if((min > int(data_colors[1+i])) && int(data_colors[1+i]) > 0){
            min = int(data_colors[1+i]);
        }
        tmpmoy += int(data_colors[1+i]);
    }
    moy = tmpmoy/361;
    int number1 = 256/(max - min);
    qDebug()<<"min "<<min<<"  max "<<max<<"  moy "<<moy<<"  fact "<<number1;

    //qDebug()<<data_colors;
    for(i=0;i<19;i++){
        for(j=0;j<19;j++){
            if(int(data_colors[1+j+19*i])>0){
                pen.setColor(QColor( (int(data_colors[1+j+19*i])-min)*number1, 0, (int(data_colors[1+j+19*i])-min)*number1));

                //qDebug()<<">>"<<int(data_colors[1+j+19*i])<<"   >>"<<(int(data_colors[1+j+19*i])-min)*number1;
                painter.setPen(pen);
                painter.drawPoint(170+15*(19-i),80+15*j);
                cpt_good_pixel++;
            }
            else{
                pen.setColor(QColor(0xFF,0xFF,0xFF));
                painter.setPen(pen);
                painter.drawPoint(170+15*(19-i),80+15*j);
            }
        }
    }
    qDebug()<<"Number of valid pixel : "<<cpt_good_pixel;
    size++;

    foreach (QPoint point, m_points) { painter.drawPoint(point); }
}

void view_window::tab_update(QByteArray data_col){

    //qDebug()<<"Data RX"<<data_col;
    //std::memcpy((void*)(&data_colors), (void*)(&data_col), 364);
    //data_col.setRawData(data_colors, 365);
    //data_col.setRawData()
    data_colors = data_col.mid(0);
    //qDebug()<<"Data RX"<<data_colors;
    update();
}

void view_window::changerValeur(int i)
{
    barre->setValue(i);
}

void view_window::calculTermine()
{
    //label->setText("Calcul Terminé");
    update();
}

