#include "CameraIPSystem.h"
using namespace std;
CameraIPSystem::CameraIPSystem(MyServer *server, MyRunnable** task, QObject *parent):
    QObject(parent)
{
    //*(task) = new MyRunnable(&Camera, &mutexForCameraGrab);
    connect(server, SIGNAL(readyReadNewCapture()), this, SLOT(RetrieveFrameToServer()));
    connect(this, SIGNAL(frameReady(cv::Mat*)), server, SLOT(sendFrame(cv::Mat*)));
}

void CameraIPSystem::GrabLoop()
{
}

void CameraIPSystem::RetrieveFrameToServer()
{
    Camera.grab();
    Camera.retrieve (imageToSend);
    emit frameReady(&imageToSend);
}

/**/
