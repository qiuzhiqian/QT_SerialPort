#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTableWidgetItem>
#include <QThread>
#include "receivethread.h"
#include "drawwave.h"
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class MySerialPort;
}

class MySerialPort : public QWidget
{
    Q_OBJECT

public:
    explicit MySerialPort(QWidget *parent = 0);
    ~MySerialPort();

    QSerialPort *getSerialPort();
    DrawWave *getDrawWave();

    void WaveDemo1();

signals:
    void sendSpStatus(bool spStatus);
    void killReceThread();
    void sendToWave(int val);

private:
    Ui::MySerialPort *ui;

    QSerialPort *my_serialport;

    void selectInit();

    QByteArray String2Hex(QByteArray data);

    bool spStatus;

    QString curPortName;
    QString curBaudRate;
    QString curDatabits;        //5,6,7,8
    QString curStopBits;        //1,3,2
    QString curParity;          //0,2,3
    QString curReceType;           //0,1,2

    QString receString;
    QByteArray receByte;
    QByteArray receBString;

    void closeEvent( QCloseEvent * event );

    DrawWave *waves;
    QByteArray *curVal;
    QTimer *updataTimer;

    QTimer dataTimer;

    int dataLen;
private slots:
    void portConnect();
    void addTableRow();
    void sendTableRowData();
    void receiveData(QByteArray data);
    void setPort(QString port);
    void setBaud(QString bd);
    void setDataBits(QString dbs);
    void setStopBits(QString sbs);
    void setParity(QString par);
    void setReceType(QString rt);

    void realTimeUpdate1_st();



};

#endif // MYSERIALPORT_H
