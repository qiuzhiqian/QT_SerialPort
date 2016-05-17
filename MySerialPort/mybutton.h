#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    MyButton(QWidget *parent=0);
    void setRowIndex(int row);
    void setColumnIndex(int column);
    int row();
    int column();

private:
    int rowIndex;
    int columnIndex;
};

#endif // MYBUTTON_H
