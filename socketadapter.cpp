#include "socketadapter.h"
#include <QFile>
#include <QFileInfo>

SocketAdapter::SocketAdapter(QTcpSocket* socket):
    out(&_arrBlock, QIODevice::WriteOnly),
    in(socket),
    p_QTcpSocket(socket)
{
    out << quint64(0);
}

SocketAdapter::~SocketAdapter() {}

void SocketAdapter::sendMessege(const QString& str)
{
    out << (quint8)Message ;
    out << str;

    sendBytes();
}

void SocketAdapter::sendCommand(Signal signal)
{
    out << (quint8)Command ;
    out << (quint8)signal;

    sendBytes();
}

void SocketAdapter::sendNewPosition(float x, float y)
{
    out << (quint8)Command ;
    out << (quint8)NewPosition;

    out << x;
    out << y;

    sendBytes();
}

void SocketAdapter::sendRobotData(RobotData& data)
{
    out << (quint8)DataRobot;

    out << data.time;
    out << data.Ul;
    out << data.Ur;
    out << data.wl;
    out << data.wr;
    out << data.v;
    out << data.fi;
    out << data.x;
    out << data.y;

    sendBytes();
}

void SocketAdapter::sendRobotData(QVector<RobotData*>& data)
{
    out << (quint8)DataRobot;
    out << (quint16)data.size();

    qDebug() << "PUSH!";
    for(int i = 0; i < data.size(); i++)
    {
        qDebug() << "RobotData"<< i;
        out << data[i]->time;
        out << data[i]->Ul;
        out << data[i]->Ur;
        out << data[i]->wl;
        out << data[i]->wr;
        out << data[i]->v;
        out << data[i]->fi;
        out << data[i]->x;
        out << data[i]->y;
    }
    sendBytes();
}

void SocketAdapter::sendCapture(QByteArray &capture, quint16 rows, quint16 cols, quint16 type)
{
    out << (quint8)Capture;
    out << rows;
    out << cols;
    out << type;
    _arrBlock.append(capture);
    //_arrBlock.append(qCompress(capture,5));
    sendBytes();
}

void SocketAdapter::sendFile(const QString& filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QFileInfo fileInf(file);

    out << (quint8)File << fileInf.fileName() << file.readAll();
    sendBytes();
}

void SocketAdapter::sendBytes()
{
    out.device()->seek(0);
    out << quint64(_arrBlock.size() - sizeof(quint64));

    qDebug() << QString::number((_arrBlock.size() - sizeof(quint64))/1024.0)+" KB";

    p_QTcpSocket->write(_arrBlock);
    out.device()->seek(0);
    _arrBlock.clear();
    out << quint64(0);
}


Signal SocketAdapter::readNextBlock(QByteArray &arrBlock)
{
    //Каждый переданный блок начинается полем, хранящим размер блока.
    if (m_nNextBlockSize == 0)
    {
        if (p_QTcpSocket->bytesAvailable() < sizeof(quint64))
            return Nothing;
        in >> m_nNextBlockSize;
     }

    if (p_QTcpSocket->bytesAvailable() < m_nNextBlockSize)
        return Nothing;

    Signal typeMessage;
    in >> (quint8&)typeMessage;

    arrBlock.append(p_QTcpSocket->read(m_nNextBlockSize));

    m_nNextBlockSize = 0;

    return typeMessage;
}
