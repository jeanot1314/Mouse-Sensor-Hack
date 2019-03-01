/*#include <QtGui>
#include "widget.h"

#define MIN_DCX    (0.1)
#define MAX_DCX    (5.0)

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    dcx=MIN_DCX;
    setFixedSize(170, 100);
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    pcx=dcx*2;
    QRect rect = QRect(50-dcx,25-dcx,60+pcx,40+pcx);
    painter.drawText(rect, Qt::AlignCenter,printData);
    painter.drawRect(rect);
    painter.end();

}

void Widget::setPrintData(QString value){
   printData = value;
   dcx=(dcx>MAX_DCX)?MIN_DCX:dcx+MIN_DCX;
}
*/
