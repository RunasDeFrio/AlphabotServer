#include "CameraIPSystem.h"
using namespace std;
CameraIPSystem::CameraIPSystem(QObject *parent, MyServer *server, QThread* threadCamera):
    QObject(parent)
{
    moveToThread(threadCamera);

    connect(server, SIGNAL(readyReadNewCapture()), this, SLOT(RetrieveFrameToServer()));
    connect(this, SIGNAL(frameReady(cv::Mat*)), server, SLOT(sendFrame(cv::Mat*)));
    connect(threadCamera, SIGNAL(started()), this, SLOT(GrabLoop()));
}

void CameraIPSystem::GrabLoop()
{

    double time_=cv::getTickCount();
    qDebug() << "GRAB";
    while(true)
    {

        mutexForCameraGrab.lock();
        //Camera.grab();
        mutexForCameraGrab.unlock();
            //double secondsElapsed= double ( cv::getTickCount()-time_ ) /double ( cv::getTickFrequency() ); //time in second
            //cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<< ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
    }
    Camera.release();
}

void CameraIPSystem::RetrieveFrameToServer()
{
    qDebug() << "RetrieveFrameToServer";
    mutexForCameraGrab.lock();
    //Camera.retrieve (imageToSend);
    mutexForCameraGrab.unlock();
    emit frameReady(&imageToSend);
}

/**/
