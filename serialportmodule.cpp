#include "serialportmodule.h"
#include <iostream>
SerialPortModule::SerialPortModule(MyServer* server, QThread *thread)
{
    serial = new QSerialPort(this);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    qDebug()<<"Available ports:";
    for(int i = 0; i < QSerialPortInfo::availablePorts().size(); i++)
        qDebug()<<"[" + QString::number(i) + "] "
                  + QSerialPortInfo::availablePorts()[i].portName();

    do
    {
        std::cout << "Please, select port: ";
        std::cin >> num;
    }while(!(num < QSerialPortInfo::availablePorts().size() && num >= 0));


    connect(server, SIGNAL(newPosition(float,float)), this, SLOT(writePosition(float,float)));
    connect(server, SIGNAL(newCommand(QString)), this, SLOT(writeCommand(QString)));

    moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(openSerialPort()));
}

void SerialPortModule::openSerialPort()
{
    qDebug()<<"port select: "<<QSerialPortInfo::availablePorts()[num].portName();
    serial->setPortName(QSerialPortInfo::availablePorts()[num].portName());
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "\nPort settings successfully";
    } else
    {
        qDebug()<< serial->errorString();
        qDebug() << "\nPort settings failed";
    }

}

void SerialPortModule::writePosition(float x, float y)
{
    str ="X " + QString::number(x);
    writeData();

    str ="Y " + QString::number(y);
    writeData();
}

void SerialPortModule::writeCommand(QString str)
{
    this->str = str;
    writeData();
}

void SerialPortModule::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
}

void SerialPortModule::writeData()
{
    str+='\n';
    serial->write(str.toLocal8Bit());
    str = "";
}

void SerialPortModule::readData()
{
    QString str = serial->readAll();
    std::cout << str.toStdString();
}

void SerialPortModule::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        qDebug()<<"Critical Error" << serial->errorString();
        closeSerialPort();
    }
}
