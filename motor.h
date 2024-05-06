#ifndef MOTOR_H
#define MOTOR_H

#include <QtSerialPort/qserialport.h>

#include <QtCore/qqueue.h>
#include <QtCore/qtimer.h>

class Motor : public QSerialPort
{
    Q_OBJECT

public:
    explicit Motor(QObject *parent = nullptr);

    int position() const;
    Q_SLOT void setPosition(int pos);
    Q_SIGNAL void positionChanged(int pos);
    Q_SIGNAL void positionReceived(int position);

private:
    Q_SLOT void moveMotors();
    Q_SLOT void receiveData();

    int m_pos;
    QQueue<int> m_posQueue;

    QTimer m_timer;
};

#endif // MOTOR_H
