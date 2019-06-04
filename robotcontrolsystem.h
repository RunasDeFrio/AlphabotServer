#ifndef ROBOTCONTROLSYSTEM_H
#define ROBOTCONTROLSYSTEM_H

#include <QObject>
#include "myserver.h"
#include "serialportmodule.h"
class RobotControlSystem : public QObject
{
    Q_OBJECT
public:
    explicit RobotControlSystem(MyServer *server, SerialPortModule* serial, QObject *parent = nullptr);

   QVector<RobotData> dataHystory;
signals:
    void pullRobotDataToServer(RobotData* data);

public slots:
    void pushNewRobotSerialData(QString str);

};

#endif // ROBOTCONTROLSYSTEM_H
