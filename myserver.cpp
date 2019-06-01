#include "myserver.h"
#include <QTime>
using namespace std;

MyServer::MyServer(QObject *parent):
    QObject(parent),
    quality_params(2)
{
    quality_params[0] = cv::IMWRITE_JPEG_QUALITY; // Кодек JPEG
    quality_params[1] = 20;
}

void MyServer::startServer()
{
    qDebug() << "Server start";
    server = new QTcpServer(this);
    qDebug() << "server listen = " << server->listen(QHostAddress::Any, 1234);
    connect(server, SIGNAL(newConnection()), this, SLOT(incommingConnection())); // подключаем сигнал "новое подключение" к нашему обработчику подключений
}

void MyServer::incommingConnection()
{
    QTcpSocket* new_socket = server->nextPendingConnection();
    socket = new SocketAdapter(new_socket);

    connect(new_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(new_socket, SIGNAL(readyRead()), this, SLOT(readyRead())); // подключаем входящие сообщения от вещающего на наш обработчик

    socket->sendMessege("Connection complite");
    qDebug() << "Connect to PC";
}

void MyServer::readyRead()
{
    qDebug() << "NEW DATA";

    QByteArray arrBlock;
    QDataStream in(&arrBlock, QIODevice::ReadOnly);

    Signal typeMessage;
    QString str;
    typeMessage = socket->readNextBlock(arrBlock);

    switch (typeMessage)
    {
    case Message:
        in >> str;
        qDebug() <<"from PC: " + QTime::currentTime().toString()+" "+str;
        break;
    case Command:
        Signal command;
        in >> (quint8&)command;
        if(command == CapEnd)
        {
            qDebug()<<"Client Comlite";
            emit readyReadNewCapture();
        }
        break;
    case File:
        qDebug() << "File save function is off, becouse it is not necessary, sorry :(";
        break;
    default:
        break;
    }
}

void MyServer::sendFrame(cv::Mat *capture)
{
    writeIsComplite = false;

    // Кодируем изображение кодеком JPEG
    cv::imencode(".jpg", *capture, buf, quality_params);

    rows = capture->rows;
    cols = capture->cols;
    type = capture->type();
    captureByteArray = QByteArray(reinterpret_cast<const char*>(buf.data()),  buf.size());

    socket->sendCapture(captureByteArray, rows, cols, type);

    writeIsComplite = true;
    //captureByteArray = QByteArray(reinterpret_cast<const char*>(capture.dataend),  1+(capture.datastart-capture.dataend));
    //captureByteArray((const char*)&imgBuf[0], static_cast<int>(imgBuf.size()));
}
/**/

void MyServer::stateChanged(QAbstractSocket::SocketState state) // обработчик статуса, нужен для контроля за "вещающим"
{
    if(state == QAbstractSocket::UnconnectedState)
         qDebug() << "PC connection terminated";
    if(state == QAbstractSocket::ClosingState)
        qDebug() << "Socket close";
}
