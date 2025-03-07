#include "widget.h"

#include <QVBoxLayout>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QDebug>

#include "ui_widget.h"

QTextBrowser *Widget::logBrowser = nullptr;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , modbus(new ModbusRTU(this))
    , slaveAddress(1)
    , isActivated(false)
{
    ui->setupUi(this);

    for (int i = 0; i < 10; ++i) {
        ledIndicator[i] = new QLedIndicator(this);
    }

    QVBoxLayout *layout_1 = new QVBoxLayout(ui->ledIndicator_1);
    layout_1->addWidget(ledIndicator[0]);
    layout_1->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_2 = new QVBoxLayout(ui->ledIndicator_2);
    layout_2->addWidget(ledIndicator[1]);
    layout_2->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_3 = new QVBoxLayout(ui->ledIndicator_3);
    layout_3->addWidget(ledIndicator[2]);
    layout_3->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_4 = new QVBoxLayout(ui->ledIndicator_4);
    layout_4->addWidget(ledIndicator[3]);
    layout_4->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_5 = new QVBoxLayout(ui->ledIndicator_5);
    layout_5->addWidget(ledIndicator[4]);
    layout_5->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_6 = new QVBoxLayout(ui->ledIndicator_6);
    layout_6->addWidget(ledIndicator[5]);
    layout_6->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_7 = new QVBoxLayout(ui->ledIndicator_7);
    layout_7->addWidget(ledIndicator[6]);
    layout_7->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_8 = new QVBoxLayout(ui->ledIndicator_8);
    layout_8->addWidget(ledIndicator[7]);
    layout_8->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_9 = new QVBoxLayout(ui->ledIndicator_9);
    layout_9->addWidget(ledIndicator[8]);
    layout_9->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *layout_10 = new QVBoxLayout(ui->ledIndicator_10);
    layout_10->addWidget(ledIndicator[9]);
    layout_10->setContentsMargins(0, 0, 0, 0);

    // 初始化控件状态
    ui->buttonConnect->setEnabled(false);
    ui->comboBoxPorts->setEnabled(false);
    ui->spinBoxSlaveAddress->setEnabled(false);
    ui->checkBoxPower_10->setEnabled(false);
    ui->checkBoxPower_1->setEnabled(false);
    ui->checkBoxPower_2->setEnabled(false);
    ui->checkBoxPower_3->setEnabled(false);
    ui->checkBoxPower_4->setEnabled(false);
    ui->checkBoxPower_5->setEnabled(false);
    ui->checkBoxPower_6->setEnabled(false);
    ui->checkBoxPower_7->setEnabled(false);
    ui->checkBoxPower_8->setEnabled(false);
    ui->checkBoxPower_9->setEnabled(false);
    ui->pushButtonUpdate->setEnabled(false);

    // 获取可用的串口列表并填充到 QComboBox
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBoxPorts->addItem(info.portName());
    }

    logBrowser = ui->textBrowserLogs;

    qInstallMessageHandler(Widget::handleLogMessage);

    // 激活按钮
    connect(ui->pushButtonActivate, &QPushButton::clicked, this, &Widget::onActivateButtonClicked);

    // 连接按钮
    connect(ui->buttonConnect, &QPushButton::clicked, this, &Widget::onConnectButtonClicked);

    // 电源开关按钮
    connect(ui->checkBoxPower_10, &QCheckBox::checkStateChanged, this, &Widget::onPower10CheckBoxChanged);
    connect(ui->checkBoxPower_1, &QCheckBox::checkStateChanged, this, &Widget::onPower1CheckBoxChanged);
    connect(ui->checkBoxPower_2, &QCheckBox::checkStateChanged, this, &Widget::onPower2CheckBoxChanged);
    connect(ui->checkBoxPower_3, &QCheckBox::checkStateChanged, this, &Widget::onPower3CheckBoxChanged);
    connect(ui->checkBoxPower_4, &QCheckBox::checkStateChanged, this, &Widget::onPower4CheckBoxChanged);
    connect(ui->checkBoxPower_5, &QCheckBox::checkStateChanged, this, &Widget::onPower5CheckBoxChanged);
    connect(ui->checkBoxPower_6, &QCheckBox::checkStateChanged, this, &Widget::onPower6CheckBoxChanged);
    connect(ui->checkBoxPower_7, &QCheckBox::checkStateChanged, this, &Widget::onPower7CheckBoxChanged);
    connect(ui->checkBoxPower_8, &QCheckBox::checkStateChanged, this, &Widget::onPower8CheckBoxChanged);
    connect(ui->checkBoxPower_9, &QCheckBox::checkStateChanged, this, &Widget::onPower9CheckBoxChanged);

    // 从机地址
    connect(ui->spinBoxSlaveAddress, &QSpinBox::valueChanged, this, &Widget::onSlaveAddressChanged);

    // 刷新
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update10VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update1VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update2VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update3VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update4VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update5VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update6VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update7VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update8VoltageAndCurrent);
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &Widget::update9VoltageAndCurrent);

    // 清除日志
    connect(ui->pushButtonClearLogs, &QPushButton::clicked, this, &Widget::clearLogs);
}

Widget::~Widget()
{
    qInstallMessageHandler(nullptr);
    delete ui;
    for (int i = 0; i < 10; ++i) {
        delete ledIndicator[i];
    }
}

void Widget::onActivateButtonClicked()
{
    if (!isActivated) {
        isActivated = true;
        ui->pushButtonActivate->setText("running");
        ui->pushButtonActivate->setEnabled(false);

        ui->buttonConnect->setEnabled(true);
        ui->comboBoxPorts->setEnabled(true);
        ui->spinBoxSlaveAddress->setEnabled(true);
        ui->checkBoxPower_10->setEnabled(true);
        ui->checkBoxPower_1->setEnabled(true);
        ui->checkBoxPower_2->setEnabled(true);
        ui->checkBoxPower_3->setEnabled(true);
        ui->checkBoxPower_4->setEnabled(true);
        ui->checkBoxPower_5->setEnabled(true);
        ui->checkBoxPower_6->setEnabled(true);
        ui->checkBoxPower_7->setEnabled(true);
        ui->checkBoxPower_8->setEnabled(true);
        ui->checkBoxPower_9->setEnabled(true);
        ui->pushButtonUpdate->setEnabled(true);

        qDebug() << "System activated.";
    }
}

void Widget::onConnectButtonClicked()
{
    if (!isActivated) {
        qWarning() << "System not activated. Please activate first.";
        return;
    }

    // 获取 QComboBox 中选择的串口名称
    QString selectedPort = ui->comboBoxPorts->currentText();

    if (!selectedPort.isEmpty()) {
        modbus->connectDevice(selectedPort);
    } else {
        qWarning() << "No serial port selected!";
    }
}

void Widget::onPower10CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 0, true);
        ledIndicator[9]->setState(true);
        update10VoltageAndCurrent();
        qDebug() << "Power 10 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 0, false);
        ledIndicator[9]->setState(false);
        update10VoltageAndCurrent();
        qDebug() << "Power 10 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower1CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 1, true);
        ledIndicator[0]->setState(true);
        update1VoltageAndCurrent();
        qDebug() << "Power 1 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 1, false);
        ledIndicator[0]->setState(false);
        update1VoltageAndCurrent();
        qDebug() << "Power 1 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower2CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 2, true);
        ledIndicator[1]->setState(true);
        update2VoltageAndCurrent();
        qDebug() << "Power 2 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 2, false);
        ledIndicator[1]->setState(false);
        update2VoltageAndCurrent();
        qDebug() << "Power 2 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower3CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 3, true);
        ledIndicator[2]->setState(true);
        update3VoltageAndCurrent();
        qDebug() << "Power 3 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 3, false);
        ledIndicator[2]->setState(false);
        update3VoltageAndCurrent();
        qDebug() << "Power 3 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower4CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 4, true);
        ledIndicator[3]->setState(true);
        update4VoltageAndCurrent();
        qDebug() << "Power 4 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 4, false);
        ledIndicator[3]->setState(false);
        update4VoltageAndCurrent();
        qDebug() << "Power 4 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower5CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 5, true);
        ledIndicator[4]->setState(true);
        update5VoltageAndCurrent();
        qDebug() << "Power 5 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 5, false);
        ledIndicator[4]->setState(false);
        update5VoltageAndCurrent();
        qDebug() << "Power 5 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower6CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 6, true);
        ledIndicator[5]->setState(true);
        update6VoltageAndCurrent();
        qDebug() << "Power 6 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 6, false);
        ledIndicator[5]->setState(false);
        update6VoltageAndCurrent();
        qDebug() << "Power 6 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower7CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 7, true);
        ledIndicator[6]->setState(true);
        update7VoltageAndCurrent();
        qDebug() << "Power 7 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 7, false);
        ledIndicator[6]->setState(false);
        update7VoltageAndCurrent();
        qDebug() << "Power 7 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower8CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 8, true);
        ledIndicator[7]->setState(true);
        update8VoltageAndCurrent();
        qDebug() << "Power 8 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 8, false);
        ledIndicator[7]->setState(false);
        update8VoltageAndCurrent();
        qDebug() << "Power 8 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onPower9CheckBoxChanged(int state)
{
    if (!isActivated) return;

    if (state == Qt::Checked) {
        modbus->sendPowerCommand(slaveAddress, 9, true);
        ledIndicator[8]->setState(true);
        update9VoltageAndCurrent();
        qDebug() << "Power 9 ON command sent to slave address" << slaveAddress;
    } else {
        modbus->sendPowerCommand(slaveAddress, 9, false);
        ledIndicator[8]->setState(false);
        update9VoltageAndCurrent();
        qDebug() << "Power 9 OFF command sent to slave address" << slaveAddress;
    }
}

void Widget::onSlaveAddressChanged(int value)
{
    if (!isActivated) return;

    slaveAddress = value;
    qDebug() << "Slave address updated to" << slaveAddress;
}

void Widget::update10VoltageAndCurrent()
{
    if (ledIndicator[9]->getState()) {
        double voltage10 = modbus->readVoltageOrCurrent(slaveAddress, 0);
        double current10 = modbus->readVoltageOrCurrent(slaveAddress, 10);
        ui->lineEditPower10Voltage->setText(QString::number(voltage10, 'f', 3));
        ui->lineEditPower10Current->setText(QString::number(current10, 'f', 3));
    } else {
        ui->lineEditPower10Voltage->clear();
        ui->lineEditPower10Current->clear();
    }
}

void Widget::update1VoltageAndCurrent()
{
    if (ledIndicator[0]->getState()) {
        double voltage1 = modbus->readVoltageOrCurrent(slaveAddress, 1);
        double current1 = modbus->readVoltageOrCurrent(slaveAddress, 11);
        ui->lineEditPower1Voltage->setText(QString::number(voltage1, 'f', 3));
        ui->lineEditPower1Current->setText(QString::number(current1, 'f', 3));
    } else {
        ui->lineEditPower1Voltage->clear();
        ui->lineEditPower1Current->clear();
    }
}

void Widget::update2VoltageAndCurrent()
{
    if (ledIndicator[1]->getState()) {
        double voltage2 = modbus->readVoltageOrCurrent(slaveAddress, 2);
        double current2 = modbus->readVoltageOrCurrent(slaveAddress, 12);
        ui->lineEditPower2Voltage->setText(QString::number(voltage2, 'f', 3));
        ui->lineEditPower2Current->setText(QString::number(current2, 'f', 3));
    } else {
        ui->lineEditPower2Voltage->clear();
        ui->lineEditPower2Current->clear();
    }
}

void Widget::update3VoltageAndCurrent()
{
    if (ledIndicator[2]->getState()) {
        double voltage3 = modbus->readVoltageOrCurrent(slaveAddress, 3);
        double current3 = modbus->readVoltageOrCurrent(slaveAddress, 13);
        ui->lineEditPower3Voltage->setText(QString::number(voltage3, 'f', 3));
        ui->lineEditPower3Current->setText(QString::number(current3, 'f', 3));
    } else {
        ui->lineEditPower3Voltage->clear();
        ui->lineEditPower3Current->clear();
    }
}

void Widget::update4VoltageAndCurrent()
{
    if (ledIndicator[3]->getState()) {
        double voltage4 = modbus->readVoltageOrCurrent(slaveAddress, 4);
        double current4 = modbus->readVoltageOrCurrent(slaveAddress, 14);
        ui->lineEditPower4Voltage->setText(QString::number(voltage4, 'f', 3));
        ui->lineEditPower4Current->setText(QString::number(current4, 'f', 3));
    } else {
        ui->lineEditPower4Voltage->clear();
        ui->lineEditPower4Current->clear();
    }
}

void Widget::update5VoltageAndCurrent()
{
    if (ledIndicator[4]->getState()) {
        double voltage5 = modbus->readVoltageOrCurrent(slaveAddress, 5);
        double current5 = modbus->readVoltageOrCurrent(slaveAddress, 15);
        ui->lineEditPower5Voltage->setText(QString::number(voltage5, 'f', 3));
        ui->lineEditPower5Current->setText(QString::number(current5, 'f', 3));
    } else {
        ui->lineEditPower5Voltage->clear();
        ui->lineEditPower5Current->clear();
    }
}

void Widget::update6VoltageAndCurrent()
{
    if (ledIndicator[5]->getState()) {
        double voltage6 = modbus->readVoltageOrCurrent(slaveAddress, 6);
        double current6 = modbus->readVoltageOrCurrent(slaveAddress, 16);
        ui->lineEditPower6Voltage->setText(QString::number(voltage6, 'f', 3));
        ui->lineEditPower6Current->setText(QString::number(current6, 'f', 3));
    } else {
        ui->lineEditPower6Voltage->clear();
        ui->lineEditPower6Current->clear();
    }
}

void Widget::update7VoltageAndCurrent()
{
    if (ledIndicator[6]->getState()) {
        double voltage7 = modbus->readVoltageOrCurrent(slaveAddress, 7);
        double current7 = modbus->readVoltageOrCurrent(slaveAddress, 17);
        ui->lineEditPower7Voltage->setText(QString::number(voltage7, 'f', 3));
        ui->lineEditPower7Current->setText(QString::number(current7, 'f', 3));
    } else {
        ui->lineEditPower7Voltage->clear();
        ui->lineEditPower7Current->clear();
    }
}

void Widget::update8VoltageAndCurrent()
{
    if (ledIndicator[7]->getState()) {
        double voltage8 = modbus->readVoltageOrCurrent(slaveAddress, 8);
        double current8 = modbus->readVoltageOrCurrent(slaveAddress, 18);
        ui->lineEditPower8Voltage->setText(QString::number(voltage8, 'f', 3));
        ui->lineEditPower8Current->setText(QString::number(current8, 'f', 3));
    } else {
        ui->lineEditPower8Voltage->clear();
        ui->lineEditPower8Current->clear();
    }
}

void Widget::update9VoltageAndCurrent()
{
    if (ledIndicator[8]->getState()) {
        double voltage9 = modbus->readVoltageOrCurrent(slaveAddress, 9);
        double current9 = modbus->readVoltageOrCurrent(slaveAddress, 19);
        ui->lineEditPower9Voltage->setText(QString::number(voltage9, 'f', 3));
        ui->lineEditPower9Current->setText(QString::number(current9, 'f', 3));
    } else {
        ui->lineEditPower9Voltage->clear();
        ui->lineEditPower9Current->clear();
    }
}

void Widget::handleLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!logBrowser) return;

    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QString logPrefix;
    QString color;

    switch (type) {
    case QtDebugMsg:
        logPrefix = "[DEBUG]";
        color = "#00008B";  // 蓝色
        break;
    case QtInfoMsg:
        logPrefix = "[INFO]";
        color = "#006400";  // 绿色
        break;
    case QtWarningMsg:
        logPrefix = "[WARNING]";
        color = "#FF8C00";  // 橘色
        break;
    case QtCriticalMsg:
        logPrefix = "[CRITICAL]";
        color = "red";
        break;
    case QtFatalMsg:
        logPrefix = "[FATAL]";
        color = "darkred";
        break;
    }

    QString formattedMessage = QString("<span style='color:%1;'>%2 %3 %4</span>")
                                   .arg(color)
                                   .arg(currentTime)
                                   .arg(logPrefix)
                                   .arg(msg);

    logBrowser->append(formattedMessage);
}

void Widget::clearLogs()
{
    ui->textBrowserLogs->clear();
    qDebug() << "Logs cleared";
}



