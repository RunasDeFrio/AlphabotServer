#ifndef CAMERAIPSYSTEM_H
#define CAMERAIPSYSTEM_H

#include <QRunnable>
#include <QDebug>
#include <QThread>
#include "myserver.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "raspicam_cv.h"
class CameraIPSystem : public QRunnable
{
public:
    CameraIPSystem(QThreadPool* pool);
	
    void sendFrame(cv::Mat &capture);
    raspicam::RaspiCam_Cv Camera;
private:
    MyServer server;
protected:
    void run();
    cv::Mat image;// Буфер для сжатого изображения
};

#endif // CAMERAIPSYSTEM_H
