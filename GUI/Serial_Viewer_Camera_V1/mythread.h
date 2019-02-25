#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>


class MyThread : public QThread
{
     Q_OBJECT

public:
        MyThread();

public slots :
        void run();

signals :
        void valueChanged(int i);
        void complete();
        //void tab_signal(QList<QString> &);
        void tab_signal(QByteArray data_c);
public :
        int i;
        QByteArray data;
};


#endif // MYTHREAD_H
