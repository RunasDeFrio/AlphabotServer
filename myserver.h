// MyServer.h

#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include <QList>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

struct ShareData
{
    bool haveNewMessage = false, writeIsComplite = false;
    QByteArray capture;
    quint16 rows, cols, type;
    QList<QString> messages;
	std::vector<uchar> buf;
};

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);

    ShareData data;

    void startServer();
    void saveData(cv::Mat &capture);
    bool ServerReady();
    QThreadPool *pool;
protected:
    void incomingConnection( qintptr handle );
private:
    


};

#endif // MYSERVER_H
