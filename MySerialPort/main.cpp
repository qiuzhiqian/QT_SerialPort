#include "myserialport.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySerialPort w;
    w.show();

    return a.exec();
}
