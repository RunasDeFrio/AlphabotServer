// MyServer.h

#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include <QList>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <QTcpServer>
#include "raspicam_cv.h"
#include "socketadapter.h"

struct ShareData
{

};

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);

private:

    QTcpServer* server;
    SocketAdapter socket;

    bool haveNewMessage = false, writeIsComplite = false;
    QByteArray captureByteArray;
    quint16 rows, cols, type;
    QList<QString> messages;
    std::vector<uchar> buf;
    std::vector<int> quality_params;  // Вектор параметров качества сжатия

    void saveData(cv::Mat &capture);
    bool ServerReady();
    QThreadPool *pool;
protected:
    void incomingConnection( qintptr handle );
public slots:
    void sendFrame(cv::Mat *capture);

    void incommingConnection(); // обработчик входящего подключения
    void readyRead(); // обработчик входящих данных
    void stateChanged(QAbstractSocket::SocketState stat); // обработчик изменения состояния вещающего сокета (он нам важен, дабы у нас всегда был кто-то, кто будет вещать
signals:
    void readyReadNewCapture();
};

#endif // MYSERVER_H
