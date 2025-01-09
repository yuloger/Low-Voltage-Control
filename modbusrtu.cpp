#include "modbusrtu.h"

#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QModbusReply>
#include <QModbusDataUnit>
#include <QDebug>
#include <QVariant>
#include <QSerialPortInfo>
#include <QEventLoop>

ModbusRTU::ModbusRTU(QObject *parent)
    : QObject(parent), modbusDevice(new QModbusRtuSerialMaster(this))
{
    // 设置超时和重试次数
    modbusDevice->setTimeout(1000);
    modbusDevice->setNumberOfRetries(3);
}

ModbusRTU::~ModbusRTU()
{
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
    }
}

void ModbusRTU::connectDevice(const QString &portName)
{
    // 设置连接参数
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, QVariant(portName)); // 串口名称
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QVariant(QSerialPort::Baud9600)); // 波特率
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QVariant(QSerialPort::Data8)); // 数据位
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QVariant(QSerialPort::OneStop)); // 停止位
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, QVariant(QSerialPort::NoParity)); // 校验

    if (!modbusDevice->connectDevice()) {
        qWarning() << "Connect failed:" << modbusDevice->errorString();
    } else {
        qDebug() << "Modbus device connected on" << portName;
    }
}

void ModbusRTU::readRegisters(int serverAddress, int startAddress, int count)
{
    if (!modbusDevice) return;

    QModbusDataUnit request(QModbusDataUnit::HoldingRegisters, startAddress, count);
    auto reply = modbusDevice->sendReadRequest(request, serverAddress);
    if (!reply) {
        qWarning() << "Failed to send request:" << modbusDevice->errorString();
        return;
    }

    // 处理响应
    connect(reply, &QModbusReply::finished, [reply]() {
        if (reply->error() == QModbusDevice::NoError) {
            const auto data = reply->result().values();
            for (int i = 0; i < data.size(); ++i) {
                qDebug() << "Register" << i << ":" << data[i];
            }
        } else {
            qWarning() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void ModbusRTU::sendPowerCommand(int slaveAddress, int coilAddress, bool state)
{
    QModbusDataUnit request(QModbusDataUnit::Coils, coilAddress, 1);
    request.setValue(0, state ? 0xFF : 0x00);
    auto reply = modbusDevice->sendWriteRequest(request, slaveAddress);
    if (!reply) {
        qWarning() << "Failed to send power command:" << modbusDevice->errorString();
    } else {
        qDebug() << "Power command sent to coil" << coilAddress << "with state" << state;
    }
}

double ModbusRTU::readVoltageOrCurrent(int slaveAddress, int registerAddress)
{
    QModbusDataUnit request(QModbusDataUnit::HoldingRegisters, registerAddress, 1);

    auto reply = modbusDevice->sendReadRequest(request, slaveAddress);
    if (!reply) {
        qWarning() << "Failed to send read request for address" << registerAddress
                   << ":" << modbusDevice->errorString();
        return 0.0;
    }

    QEventLoop loop;
    connect(reply, &QModbusReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QModbusDevice::NoError) {
        quint16 rawValue = reply->result().value(0);
        double value = rawValue / 1000.0;  // 比例因数为1000
        reply->deleteLater();
        return value;
    } else {
        qWarning() << "Error reading register:" << reply->errorString();
        reply->deleteLater();
        return 0.0;
    }
}







