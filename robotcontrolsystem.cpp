#include "robotcontrolsystem.h"

RobotControlSystem::RobotControlSystem(MyServer *server, SerialPortModule *serial, QThread *threadRobot, QObject *parent) : QObject(parent)
{
    connect(serial, SIGNAL(newRobotData(QString)), this, SLOT(pushNewRobotSerialData(QString)));
    connect(this, SIGNAL(pullRobotDataToServer(RobotData*)), server, SLOT(sendRobotData(RobotData*)));

    moveToThread(threadRobot);
    connect(threadRobot, SIGNAL(started()), this, SLOT(start()));
}

void RobotControlSystem::pushNewRobotSerialData(QString str)
{
    RobotData* data = new RobotData;
    QStringList stringList = str.split(" ");
    data->time = stringList[0].toFloat();
    data->Ul = stringList[1].toFloat();
    data->Ur = stringList[2].toFloat();
    data->wl = stringList[3].toFloat();
    data->wr = stringList[4].toFloat();
    data->v = stringList[5].toFloat();
    data->fi = stringList[6].toFloat();
    data->x = stringList[7].toFloat()/100;
    data->y = stringList[8].toFloat()/100;
    dataHystory.push_back(data);

    emit pullRobotDataToServer(data);
    //qDebug()<< data.time << data.Ul << data.Ur << data.wl << data.wr << data.v << data.fi << data.x << data.y;
}

void RobotControlSystem::start()
{
    qDebug()<<"Robot Control System start!";
}
