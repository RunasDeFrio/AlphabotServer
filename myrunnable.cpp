// MyRunnalbe.cpp

#include "myrunnable.h"
#include <QTimer>
#include <QEventLoop>
#include <QTime>
#include <QDataStream>
#include <QByteArray>
#include <QFile>


MyRunnable::MyRunnable(raspicam::RaspiCam_Cv *camera, QMutex *mutex):
    Camera(camera),
    mutexForCameraGrab(mutex)
{}

void MyRunnable::run()
{
    //double time_=cv::getTickCount();
    //qDebug() << "GRAB";
    while(true)
    {
        mutexForCameraGrab->lock();
	qDebug() << "GRAB";
        Camera->grab();
        mutexForCameraGrab->unlock();
        //double secondsElapsed= double ( cv::getTickCount()-time_ ) /double ( cv::getTickFrequency() ); //time in second
        //cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<< ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
    }
    Camera->release();
}
