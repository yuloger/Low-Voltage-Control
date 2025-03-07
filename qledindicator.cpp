#include "qledindicator.h"

#include <QPainter>

QLedIndicator::QLedIndicator(QWidget *parent)
    : QWidget{parent}, isOn(false)  // 这里使用{}避免窄化转换
{
    setFixedSize(15, 15); // 画布大小为20×20，指示灯大小为15×15
}

void QLedIndicator::setState(bool state)
{
    isOn = state;
    update();  // 将重绘请求添加到事件队列中
}

bool QLedIndicator::getState()
{
    return isOn;
}

void QLedIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);  // 忽略未使用的参数

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿模式

    QColor color = isOn ? Qt::green : Qt::red;  // 根据状态设置颜色
    painter.setBrush(QBrush(color));  // 图形内部
    painter.setPen(Qt::NoPen);  // 图形轮廓

    painter.drawEllipse(0, 0, width(), height());  // 前两个是外接矩形的左上角坐标
}
