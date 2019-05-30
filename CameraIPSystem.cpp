#include "CameraIPSystem.h"
using namespace std;
CameraIPSystem::CameraIPSystem(QThreadPool* pool)
{
    server.pool = pool;
    server.startServer();
}

void CameraIPSystem::run()
{
    cout<<"Capturing thread start"<<endl;

    double time_=cv::getTickCount();

    while(true)
    {
        if(server.ServerReady())
    	{
        Camera.grab();
        Camera.retrieve ( image );
	
	    sendFrame(image);
	   //double secondsElapsed= double ( cv::getTickCount()-time_ ) /double ( cv::getTickFrequency() ); //time in second
        //cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<< ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
	}
    }
    Camera.release();
}
void CameraIPSystem::sendFrame(cv::Mat &capture)
{
    // Объявляем переменные
    if(server.ServerReady())
    {
        if (capture.isContinuous())
        {
            server.saveData(capture);// Отправляем данные
        }
    }
}
/**/
