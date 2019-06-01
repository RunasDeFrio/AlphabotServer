#include "socketadapter.h"

#include <QFile>
#include <QFileInfo>

SocketAdapter::SocketAdapter():
    out(&_arrBlock, QIODevice::WriteOnly),
    in(this)
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
    write(_arrBlock);
    out.device()->seek(0);
    _arrBlock.clear();
    out << quint64(0);
}


Signal SocketAdapter::readNextBlock(QByteArray &arrBlock)
{
    //Каждый переданный блок начинается полем, хранящим размер блока.
    if (m_nNextBlockSize == 0)
    {
        if (bytesAvailable() < sizeof(quint64))
            return Nothing;
        in >> m_nNextBlockSize;
     }

    if (bytesAvailable() < m_nNextBlockSize)
        return Nothing;

    Signal typeMessage;
    in >> (quint8&)typeMessage;

    arrBlock.append(read(m_nNextBlockSize));

    m_nNextBlockSize = 0;

    return typeMessage;
}
