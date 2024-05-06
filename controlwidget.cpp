#include "controlwidget.h"
#include "ui_controlwidget.h"

#include <QKeyEvent>

void process(int id, void *d, void *t);

typedef void(Fun)(int, void *, void *);

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControlWidget)
{
    ui->setupUi(this);

    connect(ui->positionInput, &QDial::valueChanged, this, &ControlWidget::moveMotor);

    connect(ui->upButton, &QAbstractButton::clicked, this, [=] {moveMotor(90);});
    connect(ui->downButton, &QAbstractButton::clicked, this, [=] {moveMotor(90);});
    connect(ui->leftButton, &QAbstractButton::clicked, this, [=] {moveMotor(0);});
    connect(ui->rightButton, &QAbstractButton::clicked, this, [=] {moveMotor(180);});

    m_motor.open(QIODevice::ReadWrite);
    connect(&m_motor, &Motor::positionChanged, ui->positionOutput, QOverload<int>::of(&QLCDNumber::display));
    connect(&m_motor, &Motor::positionChanged, ui->positionInput, &QDial::setValue);

    Fun *x = &process;
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        int key = 0;
    }

    QWidget::keyPressEvent(event);
}

void ControlWidget::moveMotor(int pos)
{
    m_motor.setPosition(pos);
}
