#include "CameraIPSystem.h"
using namespace std;
CameraIPSystem::CameraIPSystem(QObject *parent, MyServer *server, QThread* threadCamera):
    QObject(parent)
{
    connect(server, SIGNAL(readyReadNewCapture()), this, SLOT(RetrieveFrameToServer()));
    connect(this, SIGNAL(frameReady(cv::Mat*)), server, SLOT(sendFrame(cv::Mat*)));
    connect(threadCamera, SIGNAL(started()), camera_system, SLOT(GrabLoop()));

    moveToThread(threadCamera);
}

void CameraIPSystem::GrabLoop()
{
    cout<<"Capturing thread start"<<endl;

    double time_=cv::getTickCount();

    while(true)
    {
        mutexForCameraGrab.lock();
        Camera.grab();
        mutexForCameraGrab.unlock();
            //double secondsElapsed= double ( cv::getTickCount()-time_ ) /double ( cv::getTickFrequency() ); //time in second
            //cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<< ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
    }
    Camera.release();
}

void CameraIPSystem::RetrieveFrameToServer()
{
    mutexForCameraGrab.lock();
    Camera.retrieve (imageToSend);
    mutexForCameraGrab.unlock();
    emit frameReady(*imageToSend);
}

/**/
