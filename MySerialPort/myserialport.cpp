#include "myserialport.h"
#include "ui_myserialport.h"
#include <QDebug>
#include <QHeaderView>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <mybutton.h>

MySerialPort::MySerialPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MySerialPort)
{
    ui->setupUi(this);

    my_serialport = new QSerialPort(this);
    //qDebug()<<my_serialport;
    selectInit();

    receString="";
    curVal=&receByte;

    //updataTimer=new QTimer(this);
    //updataTimer->start(100);

    ReceiveThread *receThread=new ReceiveThread(this);
    connect(receThread,SIGNAL(read_port_data(QByteArray)),this,SLOT(receiveData(QByteArray)));
    sendSpStatus(false);
    receThread->start();

    connect(ui->connectBtn,SIGNAL(clicked(bool)),this,SLOT(portConnect()));
    connect(ui->addItemBtn,SIGNAL(clicked(bool)),this,SLOT(addTableRow()));
//    connect(ui->inputTable,SIGNAL(cellClicked(int,int)),this,SLOT(sendTableRowData(int,int)));

    connect(ui->portNameBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setPort(QString)));
    connect(ui->baudRateBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setBaud(QString)));
    connect(ui->databitBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setDataBits(QString)));
    connect(ui->stopbitBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setStopBits(QString)));
    connect(ui->parityBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setParity(QString)));
    connect(ui->rTypeBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setReceType(QString)));

    //waves=new DrawWave();
    //ui->stackedWidget->insertWidget(1,waves);


    //connect(this,SIGNAL(destroyed(QObject*))
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));

    WaveDemo1();
}

MySerialPort::~MySerialPort()
{
    qDebug()<<"delete";
    delete ui;
    //killReceThread();           //结束子进程
}

QSerialPort* MySerialPort::getSerialPort()
{
    return my_serialport;
}

DrawWave* MySerialPort::getDrawWave()
{
    return waves;
}

void MySerialPort::portConnect()
{
    QSerialPortInfo *curInfo=new QSerialPortInfo(curPortName);
    if((ui->connectBtn->text()==tr("未连接"))&&(curInfo->isBusy()==false)&&(curInfo->isValid()==true))
    {
        ui->connectBtn->setText(tr("已连接"));

        my_serialport->setPortName(curPortName);

        my_serialport->setBaudRate(curBaudRate.toInt());

        if(curDatabits=="5位")   my_serialport->setDataBits(QSerialPort::Data5);
        else if(curDatabits=="6位")   my_serialport->setDataBits(QSerialPort::Data6);
        else if(curDatabits=="7位")   my_serialport->setDataBits(QSerialPort::Data7);
        else if(curDatabits=="8位")   my_serialport->setDataBits(QSerialPort::Data8);

        if(curStopBits=="1位") my_serialport->setStopBits(QSerialPort::OneStop);
        else if(curStopBits=="1.5位") my_serialport->setStopBits(QSerialPort::OneAndHalfStop);
        else if(curStopBits=="2位") my_serialport->setStopBits(QSerialPort::TwoStop);

        if(curParity=="无校验") my_serialport->setParity(QSerialPort::NoParity);
        else if(curParity=="偶校验") my_serialport->setParity(QSerialPort::EvenParity);
        else if(curParity=="奇校验") my_serialport->setParity(QSerialPort::OddParity);
        else if(curParity=="置0") my_serialport->setParity(QSerialPort::SpaceParity);
        else if(curParity=="置1") my_serialport->setParity(QSerialPort::MarkParity);

        my_serialport->setFlowControl(QSerialPort::NoFlowControl);

        my_serialport->open(QIODevice::ReadWrite);

        ui->portNameBox->setDisabled(true);//禁止输入
        ui->baudRateBox->setDisabled(true);
        ui->databitBox->setDisabled(true);
        ui->stopbitBox->setDisabled(true);
        ui->parityBox->setDisabled(true);

        sendSpStatus(true);
    }
    else if(ui->connectBtn->text()!=tr("未连接"))
    {
        ui->connectBtn->setText(tr("未连接"));
        my_serialport->close();

        ui->portNameBox->setDisabled(false);//允许输入
        ui->baudRateBox->setDisabled(false);
        ui->databitBox->setDisabled(false);
        ui->stopbitBox->setDisabled(false);
        ui->parityBox->setDisabled(false);

        sendSpStatus(false);
    }
}

void MySerialPort::selectInit()
{
    ui->inputTable->setColumnCount(6);  //设置表格列数,这个必须首先使用，要不然后面的东西不会生效
    ui->inputTable->verticalHeader()->setVisible(false);
    QStringList tableHeader;
    tableHeader<<tr("序号")<<tr("名称")<<tr("类型")<<tr("内容")<<tr("延时")<<tr("循环");
    ui->inputTable->setHorizontalHeaderLabels(tableHeader);

    ui->inputTable->horizontalHeader()->setStretchLastSection(true);

    ui->inputTable->setColumnWidth(0,30);
    ui->inputTable->setColumnWidth(1,60);
    ui->inputTable->setColumnWidth(2,70);
    ui->inputTable->setColumnWidth(3,120);
    ui->inputTable->setColumnWidth(4,35);
    ui->inputTable->setColumnWidth(5,28);


    addTableRow();

    ui->portNameBox->setEditable(true);     //允许自定义输入

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        ui->portNameBox->addItem(info.portName());
    }

    //150,300,600,1200,2400,4800,9600,19200,38400,57600,115200,128000,256000
    curPortName=ui->portNameBox->currentText();
    curBaudRate=ui->baudRateBox->currentText();
    curDatabits=ui->databitBox->currentText();
    curStopBits=ui->stopbitBox->currentText();
    curParity=ui->parityBox->currentText();
    curReceType=ui->rTypeBox->currentText();
}

void MySerialPort::addTableRow()
{
    int num=ui->inputTable->rowCount();
    ui->inputTable->insertRow(num);
    ui->inputTable->setRowHeight(num,30);
    //第一个button,第二个lineEdit,第三个combobox,第四个lineEdit,第五个lineEdit,第六个复选框
    //ui->inputTable->setItem(0,0,new QTableWidgetItem(tr("A")));
    qDebug()<<num;

    MyButton *numBtn=new MyButton();            //该方式布局有很大的弊端，即无法触发列表中的控件事件，改用代理实现
    numBtn->setText(QString::number(num+1));
    numBtn->setRowIndex(num);
    numBtn->setColumnIndex(0);
    ui->inputTable->setCellWidget(num,0,numBtn);

    QLineEdit *sNameEdit=new QLineEdit();
    ui->inputTable->setCellWidget(num,1,sNameEdit);

    QComboBox *tTypeBox=new QComboBox();
    tTypeBox->addItem(tr("十六进制"));
    tTypeBox->addItem(tr("字符串"));
    tTypeBox->addItem(tr("文件"));
    ui->inputTable->setCellWidget(num,2,tTypeBox);

    QLineEdit *tDataEdit=new QLineEdit();
    ui->inputTable->setCellWidget(num,3,tDataEdit);

    QLineEdit *delayEdit=new QLineEdit();
    ui->inputTable->setCellWidget(num,4,delayEdit);

    QCheckBox *cycCheckBox=new QCheckBox();     //将复选框放入水平布局，将水平布局应用到QWidget,将QWidget添加到列表中
    QHBoxLayout *checkLayout=new QHBoxLayout();
    checkLayout->setMargin(0);
    checkLayout->setAlignment(Qt::AlignCenter);
    checkLayout->addWidget(cycCheckBox);
    QWidget *wd=new QWidget();
    wd->setLayout(checkLayout);
    ui->inputTable->setCellWidget(num,5,wd);

//    ui->inputTable->insertRow(num+1);
//    ui->inputTable->setRowHeight(num+1,30);
//    ui->inputTable->setItem(num+1,0,new QTableWidgetItem(tr("11")));

    //QWidget *
    connect(((QPushButton *)ui->inputTable->cellWidget(num,0)),SIGNAL(clicked(bool)),this,SLOT(sendTableRowData()));

}

//void

void MySerialPort::sendTableRowData()
{

    MyButton *sendBtn=(MyButton *)sender();
    int row=sendBtn->row();
    int column=sendBtn->column();
    qDebug()<<"row:"<<row<<",col:"<<column;
    QSerialPortInfo *curInfo=new QSerialPortInfo(curPortName);
    if((column==0)&&(curInfo->isBusy()==true)&&(curInfo->isValid()==true))
    {
        QByteArray TxData=((QLineEdit *)(ui->inputTable->cellWidget(row,3)))->text().toLocal8Bit();
        my_serialport->write(TxData);
    }
}

void MySerialPort::receiveData(QByteArray data)
{
    //receByte+=data.toLocal8Bit();
    //receString+=data;
    //qDebug()<<data;
    try
    {
        receByte+=data;
        dataLen=receByte.length();
        if(dataLen>1000)    receByte=receByte.right(1000);
    }
    catch(QString exception)                      //定義異常處理，可以抓取多種類型的異常信息
     //    catch(int errorcode)
     {
     QMessageBox::about(this,"Error",exception);
     //        QMessageBox::about(this,"Error",QString::number(errorcode));
     }

    //qDebug()<<receByte[dataLen];
    //int lens=data.size();
    //int dataval=(int)(data[lens-1]);
    //qDebug()<<dataval;
    //sendToWave(dataval);


    //waves->setYVal(dataval);
    //qDebug()<<receByte;
//    if(curReceType=="十六进制")
//    {
//        //QString temp="";
//        receBString=String2Hex(receByte);
//        ui->recetEdit->setPlainText((QString)receBString);
//    }
//    else if(curReceType=="字符串")
//    {
//        ui->recetEdit->setPlainText((QString)receByte);
//    }
//    else if(curReceType=="文件")
//    {

//    }

}

QByteArray MySerialPort::String2Hex(QByteArray data)
{
    quint8 Hval=0;
    quint8 Lval=0;
    QByteArray hexString;
    for(int i=0;i<data.size();i++)
    {
        Hval=data[i]/16;
        Lval=data[i]%16;
        if(Hval>0&&Hval<10) Hval='0'+Hval;
        else if(Hval>9&&Hval<16)  Hval='a'+Hval-10;

        if(Lval>=0&&Lval<10) Lval='0'+Lval;
        else if(Lval>9&&Lval<16)  Lval='a'+Lval-10;
        hexString[i*3]=Hval;
        hexString[i*3+1]=Lval;
        hexString[i*3+2]=' ';
        //qDebug()<<Hval<<Lval;
    }
    //qDebug()<<hexString;
    return hexString;
    //
}

void MySerialPort::setPort(QString port)
{
    curPortName=port;
    qDebug()<<curPortName;
}

void MySerialPort::setBaud(QString bd)
{
    curBaudRate=bd;
    qDebug()<<bd;
}

void MySerialPort::setDataBits(QString dbs)
{
    curDatabits=dbs;
    qDebug()<<dbs;
}

void MySerialPort::setStopBits(QString sbs)
{
    curStopBits=sbs;
    qDebug()<<sbs;
}

void MySerialPort::setParity(QString par)
{
    curParity=par;
    qDebug()<<par;
}

void MySerialPort::setReceType(QString rt)
{
    curReceType=rt;
    qDebug()<<rt;
    if(curReceType=="十六进制")
    {
        ui->recetEdit->setPlainText((QString)receBString);
    }
    else if(curReceType=="字符串")
    {
        ui->recetEdit->setPlainText((QString)receByte);
    }
    else if(curReceType=="文件")
    {

    }
}

void MySerialPort::closeEvent(QCloseEvent *event)
{
    qDebug()<<"closeEvent";
    killReceThread();           //结束子进程
}

//画图API
void MySerialPort::WaveDemo1()
{
//#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
//  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
//#endif
//  demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  dataLen=0;
  ui->customPlot->addGraph(); // blue line
  ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
  ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  ui->customPlot->graph(0)->setAntialiasedFill(false);
  //ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(0));

//  ui->customPlot->addGraph(); // blue dot
//  ui->customPlot->graph(1)->setPen(QPen(Qt::blue));
//  ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
//  ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

  ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  ui->customPlot->xAxis->setAutoTickStep(false);
  ui->customPlot->xAxis->setTickStep(1);
  ui->customPlot->yAxis->setRange(0, 255);

  //ui->customPlot->axisRect()->setupFullAxesBox();



  // make left and bottom axes transfer their ranges to right and top axes:
  //connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  //connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realTimeUpdate1_st()));
  dataTimer.start(50); // Interval 0 means to refresh as fast as possible
}

void MySerialPort::realTimeUpdate1_st()
{
    // calculate two new data points:
  #if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    double key = 0;
  #else
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
  #endif
    static int lastPointKey = 0;
    int serialLen=receByte.length();
//    if (key-lastPointKey > 0.01) // at most add point every 10 ms
//    {
      static double value0=0; //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;

//      if(serialLen==0)
//      {
//          return;
//      }
//      else if(serialLen!=(lastPointKey+1))
//      {
//          value0=(double)(receByte[serialLen-1]);
//      }
//        lastPointKey=serialLen-1;
      try
      {

      if(serialLen==0)
      {
          return;
      }
      else
      {
          value0=(double)(receByte[serialLen-1]);
      }
//      else if((serialLen-1)>=lastPointKey)
//      {
//          value0=(double)(receByte[lastPointKey]);
//          lastPointKey++;
//      }
//      else if((serialLen-1)<lastPointKey)
//      {
//          value0=(double)(receByte[serialLen-1]);
//      }
      //qDebug()<<receByte[dataLen];
      //qDebug()<<value0;
      // add data to lines:
      ui->customPlot->graph(0)->addData(key, value0);
      // set data of dots:
//      ui->customPlot->graph(1)->clearData();
//      ui->customPlot->graph(1)->addData(key, value0);
      // remove data of lines that's outside visible range:
      ui->customPlot->graph(0)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
      ui->customPlot->graph(0)->rescaleValueAxis();

//    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->customPlot->yAxis->setRange(0, 255);

    ui->customPlot->replot();
      }
      catch(QString exception)                      //定義異常處理，可以抓取多種類型的異常信息
       //    catch(int errorcode)
       {
       QMessageBox::about(this,"Error",exception);
       //        QMessageBox::about(this,"Error",QString::number(errorcode));
       }
}
