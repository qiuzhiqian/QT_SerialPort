#ifndef DRAWWAVE_H
#define DRAWWAVE_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPainterPath>

class DrawWave : public QWidget
{
    Q_OBJECT
    
public:
    explicit DrawWave(QWidget *parent = 0);

    QPoint Point;   /* 线条绘制点的坐标 */

    void startPainting(void);    /* 开启绘画曲线 也就是开启定时器 */
    void closePainting(void);    /* 停止绘画曲线 也就是停止定时器 */

    void setYVal(int val);

    void paintEvent(QPaintEvent *); /* 重绘事件函数 */

private slots:
    void timerUpDate();     /* 定时器槽函数 */

private:

    unsigned int t; /* 用来保存时间 */
    int p; /* 用来调整坐标原点 */

    QPainterPath *path;
    QTimer *timer;

    /*
     *横坐标更新时间
     * 纵坐标最大值
     * 当前值
     * 坐标系转换：(x-min)/(max-min)=k=(Val-0)/(H-0)
    */

    int dataMin;
    int dataMax;
    int dataVal;

    int YPoint;
};

#endif // DRAWWAVE_H
