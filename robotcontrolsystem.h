#ifndef ROBOTCONTROLSYSTEM_H
#define ROBOTCONTROLSYSTEM_H

#include <QObject>
#include "myserver.h"
#include "serialportmodule.h"
class RobotControlSystem : public QObject
{
    Q_OBJECT
public:
   explicit RobotControlSystem(MyServer *server, SerialPortModule* serial, QThread* threadRobot, QObject *parent = nullptr);
   int n = 0;
   QVector<RobotData*> dataHystory;
signals:
    void pullRobotDataToServer(RobotData* data);

public slots:
    void pushNewRobotSerialData(QString str);
    void start();

};

#endif // ROBOTCONTROLSYSTEM_H
