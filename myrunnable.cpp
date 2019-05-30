// MyRunnalbe.cpp

#include "myrunnable.h"
#include <QTimer>
#include <QEventLoop>
#include <QTime>
#include <QDataStream>
#include <QByteArray>
#include <QFile>


MyRunnable::MyRunnable() {}

void MyRunnable::run()
{
    if(!socketDescriptor) return;

    socket.setSocketDescriptor(socketDescriptor);
    while(true)
    {
        if(socket.bytesAvailable())
        read();
        write();

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));

        loop.exec();
        if(socket.state()== QAbstractSocket::UnconnectedState)
            break;
    }
    socket.close();
}

void MyRunnable::read()
{
    QByteArray arrBlock;
    QDataStream in(&arrBlock, QIODevice::ReadOnly);
//    in.setVersion(QDataStream::Qt_4_2);

    Signal typeMessage;
    QString str;
    typeMessage = socket.readNextBlock(arrBlock);

    if(typeMessage == Nothing)
        return;

    if(typeMessage == Message)
    {
        in >> str;
        qDebug() << "new message: " << str;
        data->messages.push_back(QTime::currentTime().toString()+" "+str);
    }
    if(typeMessage == Command)
    {
	Signal command;
	in >> (quint8&)command;
	if(command == CapEnd)
	{
		data->writeIsComplite = true;
		qDebug()<<"Client Comlite";
	}
    }
    if(typeMessage == File)
    {
        in >> str;
        qDebug() << "Read new File: " + str;

        QFile newFile;
        QByteArray q;
        in >> q;
        newFile.setFileName("D:\\1"+str);

        if (!newFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            qDebug() << "Can't open file for written";
            return;
        }
        newFile.write(q);
        newFile.close();

        socket.sendMassege(QTime::currentTime().toString()+"Файл: " + str + " сохранен");
    }
}

void MyRunnable::write()
{
    if(data->haveNewMessage)
    {
        data->writeIsComplite = false;
        socket.sendCapture(data->capture, data->rows, data->cols, data->type);
        data->haveNewMessage = false;
    }
    for(; messageNumber < data->messages.length(); messageNumber++)
        socket.sendMassege(data->messages.at(messageNumber));
}
