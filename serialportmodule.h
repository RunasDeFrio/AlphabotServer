#ifndef SERIALPORTMODULE_H
#define SERIALPORTMODULE_H
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>
#include "myserver.h"
class SerialPortModule : public QObject
{
    Q_OBJECT
public:
    SerialPortModule(MyServer* server, QThread *thread);
    RobotData data;
public slots:
    void openSerialPort();
    void writePosition(float x, float y);
    void writeCommand(QString str);

private slots:
    void closeSerialPort();
    void writeData();
    void readData();
    void handleError(QSerialPort::SerialPortError error);
private:
    int num, numOfData;
    bool readCompleted = true;
    QString str, strData;
    QSerialPort *serial;
signals:
    void newRobotData(QString);
};

#endif // SERIALPORTMODULE_H
