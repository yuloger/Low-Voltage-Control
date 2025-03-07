#ifndef QLEDINDICATOR_H
#define QLEDINDICATOR_H

#include <QWidget>

class QLedIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit QLedIndicator(QWidget *parent = nullptr);

    void setState(bool state);
    bool getState();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool isOn;
};

#endif
