#include "robotcontrolsystem.h"
#include <algorithm>
RobotControlSystem::RobotControlSystem(MyServer *server, SerialPortModule *serial, QThread *threadRobot, QObject *parent) : QObject(parent)
{
    dataHystory = new QVector<RobotData>(10);
    dataHystoryBuffer = new QVector<RobotData>(10);

    connect(serial, SIGNAL(newRobotData(QString)), this, SLOT(pushNewRobotSerialData(QString)));

    connect(this, SIGNAL(pullRobotDataToServer(QVector<RobotData>*)), server, SLOT(sendRobotData(QVector<RobotData>*)));
    connect(server, SIGNAL(readyReadNewRobotData()), this, SLOT(getNewRobotSerialData()));

    moveToThread(threadRobot);
    connect(threadRobot, SIGNAL(started()), this, SLOT(start()));
}

void RobotControlSystem::pushNewRobotSerialData(QString str)
{
    dataHystory->push_back(RobotData());

    QStringList stringList = str.split(" ");
    dataHystory->last().time = stringList[0].toFloat();
    dataHystory->last().Ul = stringList[1].toFloat();
    dataHystory->last().Ur = stringList[2].toFloat();
    dataHystory->last().wl = stringList[3].toFloat();
    dataHystory->last().wr = stringList[4].toFloat();
    dataHystory->last().v = stringList[5].toFloat();
    dataHystory->last().fi = stringList[6].toFloat();
    dataHystory->last().x = stringList[7].toFloat()/100;
    dataHystory->last().y = stringList[8].toFloat()/100;

    //qDebug()<< data.time << data.Ul << data.Ur << data.wl << data.wr << data.v << data.fi << data.x << data.y;
}

void RobotControlSystem::start()
{

    qDebug()<<"Robot Control System start!";
}

void RobotControlSystem::getNewRobotSerialData()
{
    std::swap(dataHystory, dataHystoryBuffer);
    emit pullRobotDataToServer(dataHystoryBuffer);
}
