// myrunnable.h

#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include "socketadapter.h"
#include "myserver.h"

class MyRunnable : public QRunnable
{
public:
    MyRunnable();

protected:
    void run();

public:
    qintptr socketDescriptor;

    ShareData* data;
private:
    SocketAdapter socket;
    int messageNumber = 0;
private:
    void read();
    void write();
};
#endif // MYRUNNABLE_H
