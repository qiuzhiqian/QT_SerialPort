#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QWidget>
#include <QThread>
#include "myserialport.h"
#include "drawwave.h"

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    ReceiveThread(QWidget *parent);
    void stop();
private:
    //MySerialPort *spc;
    QSerialPort *sp;
    volatile bool spStatus;
    bool stopFlag;

    DrawWave *dw;
protected:
    void run();

signals:
    void read_port_data(QByteArray data);
private slots:
    void updateStatus(bool Status);

};

#endif // RECEIVETHREAD_H
