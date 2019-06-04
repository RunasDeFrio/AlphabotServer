#ifndef SOCKETADAPTER_H
#define SOCKETADAPTER_H
#include <QDataStream>
#include <QTcpSocket>
#include <qglobal.h>
#include <QObject>
enum Signal: quint8
{
    Nothing = 0,
    Message,
    File,
    Capture,
    Command,
    CapEnd,
    NewPosition,
    Move,
    Stop
};

class SocketAdapter
{
public:
    SocketAdapter(QTcpSocket *socket);
    ~SocketAdapter();
    Signal readNextBlock(QByteArray &arrBlock);
    void sendMessege(const QString &str);
    void sendFile(const QString &filePath);
    void sendCapture(QByteArray &Capture, quint16 rows, quint16 cols, quint16 type);
    void sendCommand(Signal signal);
    QTcpSocket* p_QTcpSocket;
private:

    void sendBytes();
    QByteArray  _arrBlock;
    QDataStream out;
    QDataStream in;
    quint64 m_nNextBlockSize = 0;
};

#endif // SOCKETADAPTER_H
