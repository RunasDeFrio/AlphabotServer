// MyServer.cpp

#include "myserver.h"
#include "myrunnable.h"

MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
    //pool = new QThreadPool(this);
    //pool->setMaxThreadCount(1);
    data.messages<<"STARTING WORK\n";
}

void MyServer::startServer()
{
    if(this->listen(QHostAddress::Any, 1234))
        qDebug() << "Server started";
    else
        qDebug() << "Server did not start!";
}

void MyServer::saveData(cv::Mat &capture)
{
    if(data.writeIsComplite)
    {
		data.writeIsComplite = false;
		
		std::vector<int> quality_params(2);  // Вектор параметров качества сжатия
		quality_params[0] = cv::IMWRITE_JPEG_QUALITY; // Кодек JPEG
		quality_params[1] = 20;
		// Кодируем изображение кодеком JPEG
		cv::imencode(".jpg", capture, data.buf, quality_params);
		
        data.rows = capture.rows;
        data.cols = capture.cols;
        data.type = capture.type();
		data.capture = QByteArray(reinterpret_cast<const char*>(data.buf.data()),  data.buf.size());
        //data.capture = QByteArray(reinterpret_cast<const char*>(capture.dataend),  1+(capture.datastart-capture.dataend));
	    //qDebug() << 1+(capture.datastart-capture.dataend);
        //capture((const char*)&imgBuf[0], static_cast<int>(imgBuf.size()));
        data.haveNewMessage = true;
    }
}

bool MyServer::ServerReady()
{
    return data.writeIsComplite;
}

void MyServer::incomingConnection(qintptr handle)
{
    qDebug()<<"NEW CONNECT";
    MyRunnable *task = new MyRunnable();
    task->setAutoDelete(true);
    task->socketDescriptor = handle;
    task->data = &data;
    pool->start(task);
    data.writeIsComplite = true;
    qDebug() << "pool started";
}
