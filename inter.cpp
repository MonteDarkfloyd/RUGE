#include "inter.h"
#include "ui_inter.h"
#include "createsession.h"
#include "predef.h"

inter::inter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inter)
{
    ui->setupUi(this);
}

inter::~inter()
{
    delete ui;
}

void inter::on_checkBox_10_toggled(bool checked)
{
    ui->cmb_payload->setEnabled(checked);
    ui->label_payload->setEnabled(checked);
}

void inter::on_pushButton_3_clicked()
{
    predef* detailed = new predef();
    detailed->show();
}
