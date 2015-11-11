#include "predefinedwindow.h"
#include "ui_predefinedwindow.h"
#include "trafficwindow.h"
#include "sessionwindow.h"

predefinedwindow::predefinedwindow(TrafficWindow *parent) :
    QDialog(parent),
    ui(new Ui::predefinedwindow)
{
    parentPointer = parent;
    ui->setupUi(this);
}

predefinedwindow::~predefinedwindow()
{
    delete ui;
}

void predefinedwindow::on_buttonBox_accepted()
{
    // TODO: check selected and send data to sessionwindow
    Window* newSess = new Window(parentPointer);
    newSess->show();
}
