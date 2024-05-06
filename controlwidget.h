#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

#include "motor.h"

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void moveMotor(int pos);

    Ui::ControlWidget *ui;

    Motor m_motor;
};

#endif // CONTROLWIDGET_H
