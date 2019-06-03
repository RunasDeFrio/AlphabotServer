#ifndef SERIALPORTMODULE_H
#define SERIALPORTMODULE_H
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>

class SerialPortModule : public QObject
{
    Q_OBJECT
public:
    SerialPortModule(QThread *thread);

public slots:
    void openSerialPort();
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
