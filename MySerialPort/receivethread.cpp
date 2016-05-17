#include "receivethread.h"
#include <QDebug>

ReceiveThread::ReceiveThread(QWidget *parent)
    :QThread()
{
    stopFlag=false;
    spStatus = false;
    //qDebug()<<parent->objectName();
    sp=((MySerialPort *)(parent))->getSerialPort();
    //qDebug()<<sp;

    //dw=((MySerialPort *)(parent))->getDrawWave();

    connect((MySerialPort *)parent,SIGNAL(killReceThread()),this,SLOT(stop()));
    //connect(parent,SIGNAL())
}

void ReceiveThread::run()
{
    QByteArray requestData=0 ;
    //int num=0;

    while(!stopFlag)
    {
        if(sp->isOpen()&&sp->isReadable())
        {
//            while(this->sp->waitForReadyRead(5))
//            {

//            }
            //requestData += this->sp->readAll();
            //char temp;
            //if(this->sp->getChar(&temp)==false) return;
            //requestData += temp;
            requestData += this->sp->readAll();       //单个字符接收
            if(!requestData.isEmpty())
            {
                int temp=requestData.size();
                //qDebug()<<temp;
                //qDebug()<<requestData[temp-1];
                read_port_data(requestData);
                int dataval=(int)(requestData[temp-1]);
                //qDebug()<<dataval;
                //dw->setYVal(dataval);
                requestData.clear();
            }
        }


        //msleep(100);

    }

}

void ReceiveThread::updateStatus(bool Status)
{
    spStatus=Status;
    //qDebug()<<spStatus;
    //qDebug()<<"opStatus="<<sp->isOpen();
    //qDebug()<<"read="<<sp->isReadable();
}

void ReceiveThread::stop()
{
    stopFlag=true;
}

