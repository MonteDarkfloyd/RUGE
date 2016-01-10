#include "predef.h"
#include "ui_predef.h"

predef::predef(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::predef)
{
    ui->setupUi(this);
}

predef::~predef()
{
    delete ui;
}
