#ifndef WIDGET_H
#define WIDGET_H

#include "qledindicator.h"
#include "modbusrtu.h"

#include <QWidget>
#include <QTextBrowser>
#include <QLoggingCategory>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onActivateButtonClicked();  // 激活按钮
    void onConnectButtonClicked();  // 连接按钮
    // 也可以用函数指针
    // void (*onPowerCheckBoxChanged[10]) (int state);
    void onPower10CheckBoxChanged(int state);  // 电源10开关
    void onPower1CheckBoxChanged(int state);  // 电源1开关
    void onPower2CheckBoxChanged(int state);  // 电源2开关
    void onPower3CheckBoxChanged(int state);  // 电源3开关
    void onPower4CheckBoxChanged(int state);  // 电源4开关
    void onPower5CheckBoxChanged(int state);  // 电源5开关
    void onPower6CheckBoxChanged(int state);  // 电源6开关
    void onPower7CheckBoxChanged(int state);  // 电源7开关
    void onPower8CheckBoxChanged(int state);  // 电源8开关
    void onPower9CheckBoxChanged(int state);  // 电源9开关
    void onSlaveAddressChanged(int value);  // 从机地址
    void update10VoltageAndCurrent();  // 读取并更新10的电压和电流值
    void update1VoltageAndCurrent();  // 读取并更新1的电压和电流值
    void update2VoltageAndCurrent();  // 读取并更新2的电压和电流值
    void update3VoltageAndCurrent();  // 读取并更新3的电压和电流值
    void update4VoltageAndCurrent();  // 读取并更新4的电压和电流值
    void update5VoltageAndCurrent();  // 读取并更新5的电压和电流值
    void update6VoltageAndCurrent();  // 读取并更新6的电压和电流值
    void update7VoltageAndCurrent();  // 读取并更新7的电压和电流值
    void update8VoltageAndCurrent();  // 读取并更新8的电压和电流值
    void update9VoltageAndCurrent();  // 读取并更新9的电压和电流值
    void clearLogs();  // 清除日志

private:
    static void handleLogMessage(QtMsgType type, const QMessageLogContext &context,
                                const QString &msg);

private:
    Ui::Widget *ui;  // 组合优于继承
    QLedIndicator *ledIndicator[10];
    ModbusRTU *modbus;
    int slaveAddress;  // 当前从机地址
    bool isActivated;
    static QTextBrowser *logBrowser;
};
#endif // WIDGET_H






