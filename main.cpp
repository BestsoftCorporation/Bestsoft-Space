#include "mainwindow.h"
#include <QApplication>
#include "start.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Start window;
    window.resize(1200, 840);
    window.show();
    return app.exec();
}
