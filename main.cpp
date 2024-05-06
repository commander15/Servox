#include <QApplication>

#include "controlwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ControlWidget w;
    w.show();

    return app.exec();
}
