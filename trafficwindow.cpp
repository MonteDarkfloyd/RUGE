#include "trafficwindow.h"
#include "ui_trafficwindow.h"
#include "sessionwindow.h"

TrafficWindow::TrafficWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrafficWindow)
{
    ui->setupUi(this);

}

TrafficWindow::~TrafficWindow()
{
    delete ui;
}

void TrafficWindow::on_actionQuit_triggered()
{
  QApplication::quit();
}

void TrafficWindow::on_pushButton_pressed()
{
    Window* sesson = new Window;
    sesson->show();
    this->close();
}
