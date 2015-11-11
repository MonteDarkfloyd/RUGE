#include "trafficwindow.h"
#include "ui_trafficwindow.h"
#include "sessionwindow.h"
#include "session.h"
#include "predefinedwindow.h"

TrafficWindow::TrafficWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrafficWindow)
{
    //connect(ui->tableWidget, SIGNAL(cellClicked(int,int)) , this, SLOT(setRow(int,int)));
    ui->setupUi(this);
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)) , this, SLOT(setRow(int,int)));


}

void TrafficWindow::setRow(int a,int b){
    this->lastRow = a;
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
    Window* newSess = new Window(this);
    newSess->show();
    //this->close();
}

void TrafficWindow::on_pushButton_clicked()
{
    Window* newSess = new Window(this);
    newSess->show();
    //this->close();

}


void TrafficWindow::addSession(Session *newSess){
    this->sessionList.append(newSess);

}

void TrafficWindow::editSession(Session *Sess){

    this->sessionList.removeAt(lastRow);
    this->sessionList.insert(lastRow,Sess);
    this->displaySessions();

}



void TrafficWindow::displaySessions(){

    QListIterator<Session *> i(this->sessionList);
    int row = 0;
    //int col = 0;
    ui->tableWidget->clear();
    ui->tableWidget->removeRow(0);
    ui->tableWidget->removeRow(1);

    while (i.hasNext())
    {
        Session* temp = i.next();

     ui->tableWidget->insertRow(row);
     ui->tableWidget->setItem(row,0, new QTableWidgetItem(temp->sessName));
     ui->tableWidget->setItem(row,1, new QTableWidgetItem(QString::number(temp->multiply)));
     ui->tableWidget->setItem(row,2, new QTableWidgetItem(QString::number(temp->rampup)));
     ui->tableWidget->setItem(row,3, new QTableWidgetItem(QString::number(temp->offset)));
     ui->tableWidget->setItem(row,4, new QTableWidgetItem(QString::number(temp->loopover)));
     ui->tableWidget->setItem(row,5, new QTableWidgetItem(QString::number(temp->loopovertimespan)));
     row = row +1;

    }



}



void TrafficWindow::on_pushButton_4_clicked()
{



    if(lastRow >= 0){
    if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) > 0 ) ){
    Session* session = this->sessionList.at(this->lastRow);
    Window* newSess = new Window(this);
    newSess->show();
    newSess->displaySession(session);
    //this->close();
    }
}

}

void TrafficWindow::on_pushButton_2_clicked()
{
    if(lastRow >= 0){
    if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) > 0 ) ){
        this->sessionList.removeAt(lastRow);
    }

}
    this->displaySessions();
    lastRow = -1;
}


void TrafficWindow::on_predefinedButton_clicked()
{
    predefinedwindow* predefWin = new predefinedwindow(this);
    predefWin->exec();
}
