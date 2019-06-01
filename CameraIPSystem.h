#ifndef CAMERAIPSYSTEM_H
#define CAMERAIPSYSTEM_H

#include <QRunnable>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include "myserver.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <QTcpServer>
#include "raspicam_cv.h"
#include "socketadapter.h"
class CameraIPSystem : public QObject
{
    Q_OBJECT
public:
    CameraIPSystem(QObject *parent, MyServer *server, QThread *threadCamera);

    raspicam::RaspiCam_Cv Camera;
    cv::Mat imageToSend;// Буфер для сжатого изображения
private:
    QMutex	mutexForCameraGrab;
protected:
    void run();

public slots:
    void RetrieveFrameToServer();
    void GrabLoop();
signals:
    void frameReady(cv::Mat *imageToSend);
};

#endif // CAMERAIPSYSTEM_H
