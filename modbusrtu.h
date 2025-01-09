#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QObject>
#include <QModbusRtuSerialMaster>  // 不能使用类前置声明

class ModbusRTU : public QObject
{
    Q_OBJECT

public:
    explicit ModbusRTU(QObject *parent = nullptr);
    ~ModbusRTU();

    void connectDevice(const QString &portName);
    void readRegisters(int serverAddress, int startAddress, int count);
    void sendPowerCommand(int slaveAddress, int coilAddress, bool state);
    double readVoltageOrCurrent(int slaveAddress, int registerAddress);

private:
    QModbusRtuSerialMaster *modbusDevice;
};

#endif // MODBUSRTU_H
