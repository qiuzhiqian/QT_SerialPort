#include "drawwave.h"

DrawWave::DrawWave(QWidget *parent) :
    QWidget(parent)
{
    resize(600,500);    //窗口大小设置为600*500
//resize(100,100);    //窗口大小设置为600*500
    dataMin=0;
    dataMax=255;
    dataVal=20;

    p = t = 0;

    Point.setX(0);  /* 初始化起始点的纵坐标为0 */
    Point.setY(0);  /* 初始化起始点的横坐标为0 */

    path = new QPainterPath;
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));  //关联定时器计满信号和相应的槽函数
    timer->start(10);
}

/* 定时时间到 */
void DrawWave::timerUpDate()
{
    t += 2;
    YPoint=500*(dataVal-dataMin)/(dataMax-dataMin);
    Point.setX(t);       /* 时间加二秒 */
    Point.setY(YPoint);    /* 设置纵坐标值 */
    //Point.setY(qrand() % 100);    /* 设置纵坐标值 */
    path->lineTo(Point);    /* */

    if(t > width()) /* 当时间值 T大于窗口的宽度时需调整坐标原点  */
        p -= 2;    /* 调整坐标原点 */

    update();
}

/* 开启绘画曲线 也就是开启定时器 */
void DrawWave::startPainting(void)
{
    timer->start(10);
}

/* 停止绘画曲线 也就是停止定时器 */
void DrawWave::closePainting(void)
{
    timer->stop();
}

/*设置纵坐标对应值*/
void DrawWave::setYVal(int val)
{
    dataVal=val;
}

/* 重绘事件函数 */
void DrawWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(QPen(Qt::red, 2)); //设置画笔颜色和大小

    painter.translate(p,0);     //调整坐标原点

    painter.drawPath(*path);    /* 绘制路径 */

}
