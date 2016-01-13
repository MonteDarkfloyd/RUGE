#include "trafficwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrafficWindow main;

    main.show();

    return a.exec();
}
