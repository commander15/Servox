#include "motor.h"

#include "protocol.h"

#include <QSerialPortInfo>
#include <QDebug>

Motor::Motor(QObject *parent) :
    QSerialPort(parent),
    m_pos(0)
{
    connect(this, &QIODevice::readyRead, this, &Motor::receiveData);

    m_timer.setInterval(5);
    //m_timer.start();
    connect(&m_timer, &QTimer::timeout, this, &Motor::moveMotors);

    const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        if (port.vendorIdentifier() == 0x2341) {
            setPort(port);
            break;
        }
    }
}

int Motor::position() const
{
    return m_pos;
}

void Motor::setPosition(int pos)
{
    if (pos < 0)
        return;

    if (m_pos != pos) {
        QByteArray packet;
        packet.append(PK_START);

        if (pos < 10)
            packet.append("00");
        else if (pos < 100)
            packet.append('0');
        packet.append(QByteArray::number(pos));

        packet.append(PK_STOP);

        write(packet);
        flush();

        m_pos = pos;
        emit positionChanged(pos);
    }
}

void Motor::moveMotors()
{
    if (m_posQueue.isEmpty() || !isOpen())
        return;

    int pos = m_posQueue.dequeue();

    QByteArray packet;
    packet.append(PK_START);
    packet.append(QByteArray::number(pos));
    packet.append(PK_STOP);

    write(packet);
    flush();

    m_pos = pos;
    emit positionChanged(pos);
}

void Motor::receiveData()
{
    QByteArray data;

    startTransaction();

    while (bytesAvailable() > 0 && !data.startsWith(PK_START)) {
        QByteArray d = read(1);
        if (d == QChar(PK_START))
            data.append(d);
    }

    while (bytesAvailable() > 0 && !data.endsWith(PK_STOP))
        data.append(read(1));

    if (data.startsWith(PK_START) && data.endsWith(PK_STOP))
        commitTransaction();
    else {
        qDebug() << data;
        rollbackTransaction();
        return;
    }

    if (data.isEmpty())
        return;
    else {
        data.removeFirst();
        data.removeLast();
    }

    bool usablePos;
    int pos = data.toInt(&usablePos);

    if (usablePos) {
        emit positionReceived(pos);

        if (m_pos != pos) {
            m_pos = pos;
            emit positionChanged(pos);
        }
    }

    if (!atEnd())
        receiveData();
}
