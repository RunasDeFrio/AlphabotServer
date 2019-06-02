// myrunnable.h

#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include "socketadapter.h"
#include "myserver.h"

class MyRunnable : public QRunnable
{
public:
    MyRunnable(raspicam::RaspiCam_Cv* Camera, QMutex* mutexForCameraGrab);

protected:
    void run();

public:
    raspicam::RaspiCam_Cv* Camera;
    QMutex*	mutexForCameraGrab;
};
#endif // MYRUNNABLE_H
