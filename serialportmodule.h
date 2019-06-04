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
    int num;
    QString str;
    QSerialPort *serial;
};

#endif // SERIALPORTMODULE_H
