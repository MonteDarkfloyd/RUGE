#include "trafficwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrafficWindow main;
    qApp->setStyleSheet("QPushButton {color: white; border-radius: 6px;text-shadow: 0 1px 1px rgba(0, 0, 0, 0.2);background:#deebea; height: 45px;padding: 2px; color: black;border-color: gray;border-width: 2px;border-style: outset;  min-width: 150px;}QPushButton:!enabled{background:white;color:gray;cursor:default;opacity:0.2;}QGroupBox { border: 1px solid LightGray  ; border-radius: 8px ; } ");
    main.show();

    return a.exec();
}
