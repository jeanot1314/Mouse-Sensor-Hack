

#include "mythread.h"
#include "view_window.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>



MyThread::MyThread()
{

}

void MyThread::run()
{
    qDebug()<<"---Start thread---";

    //while(1){}
    int cpt_port = 0;
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Number      : " << cpt_port;
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        if(info.portName() == "cu.usbmodem451381"){
            qDebug()<<"Port Teensy Available";

            QSerialPort serial;
            serial.setPort(info);
            serial.setBaudRate(QSerialPort::Baud115200);

            if (serial.open(QIODevice::ReadWrite)){
                //qDebug()<<serial.isOpen();

                qDebug()<<"---Start Serial---";

                int cpt_frame = 0, cpt_frame_nok = 0;
                while(1){
                    QByteArray head, end, data;
                    //extern QByteArray data_colors;
                    //data[1] = 1;//data_colors[1];
                    //setReadBufferSize
                    //serial.flush();
                    qDebug()<<" cpt frame OK : "<<cpt_frame << " cpt frame bad : "<<cpt_frame_nok;
                    //QThread::msleep(200);
                    if(serial.waitForReadyRead(5000)){
                        //data = serial.readLine();
                        data = serial.readAll();

                        if(data[0] == '#' && data.size() == 364)
                        {
                            //qDebug()<<"data red :"<<data;
                            cpt_frame++;
                            emit tab_signal(data);
                            //emit complete();
                        }
                        else{
                            qDebug()<<"BAD FRAME : size = "<<data.size()<<"  Header = "<<data[0];
                            cpt_frame_nok++;
                        }
                    }
                    //if(cpt_frame == 10){
                    //    emit complete();
                    //}
                }
            }
            serial.close();
        }
    }

}
