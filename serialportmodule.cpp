#include "serialportmodule.h"
#include <iostream>
SerialPortModule::SerialPortModule(QThread *thread)
{
    serial = new QSerialPort(this);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
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


    moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(openSerialPort()));
}

void SerialPortModule::openSerialPort()
{
    qDebug()<<"port select: "<<QSerialPortInfo::availablePorts()[num].portName();
    serial->setPortName(QSerialPortInfo::availablePorts()[num].portName());
    serial->setBaudRate(QSerialPort::Baud57600);
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

void SerialPortModule::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
}

void SerialPortModule::writeData()
{
    str+='\n';
    serial->write(str.toLocal8Bit());
}

void SerialPortModule::readData()
{
    qDebug()<<"New Serial data: ";
    QString str = serial->readAll();
    qDebug() << str;
}

void SerialPortModule::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        qDebug()<<"Critical Error" << serial->errorString();
        closeSerialPort();
    }
}
