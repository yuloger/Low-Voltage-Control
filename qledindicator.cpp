#include "qledindicator.h"

#include <QPainter>

QLedIndicator::QLedIndicator(QWidget *parent)
    : QWidget{parent}, isOn(false)  // 这里使用{}避免窄化转换
{
    setFixedSize(15, 15);
}

void QLedIndicator::setState(bool state)
{
    isOn = state;
    update();
}

bool QLedIndicator::getState()
{
    return isOn;
}

void QLedIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 根据状态设置颜色
    QColor color = isOn ? Qt::green : Qt::red;
    painter.setBrush(QBrush(color));
    painter.setPen(Qt::NoPen);

    // 绘制圆形
    painter.drawEllipse(0, 0, width(), height());
}
