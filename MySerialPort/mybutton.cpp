#include "mybutton.h"
#include <QPushButton>

MyButton::MyButton(QWidget *parent)
    :QPushButton(parent)
{

}

void MyButton::setRowIndex(int row)
{
    rowIndex=row;
}

void MyButton::setColumnIndex(int column)
{
    columnIndex=column;
}

int MyButton::row()
{
    return rowIndex;
}

int MyButton::column()
{
    return columnIndex;
}

