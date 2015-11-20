#include "trafficwindow.h"
#include "ui_trafficwindow.h"
#include "sessionwindow.h"
#include "session.h"
#include "newsessiondialog.h"

TrafficWindow::TrafficWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrafficWindow)
{
    //connect(ui->tableWidget, SIGNAL(cellClicked(int,int)) , this, SLOT(setRow(int,int)));
    ui->setupUi(this);
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)) , this, SLOT(setRow(int,int)));
    connect(ui->actionNew_Session,SIGNAL(triggered(bool)), this, SLOT(on_newButton_clicked()));
    ui->tableWidget->removeRow(0);

}

void TrafficWindow::setRow(int a,int b){
    this->lastRow = a;
}

TrafficWindow::~TrafficWindow()
{
    delete ui;
}

// Top menu quit
void TrafficWindow::on_actionQuit_triggered()
{
  QApplication::quit();
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
   // ui->tableWidget->clear();
    ui->tableWidget->removeRow(0);
    ui->tableWidget->removeRow(1);

    while (i.hasNext())
    {
        Session* temp = i.next();

     ui->tableWidget->insertRow(row);
     ui->tableWidget->setItem(row,0, new QTableWidgetItem(temp->sessName));
     ui->tableWidget->setItem(row,2, new QTableWidgetItem(QString::number(temp->multiply)));
     ui->tableWidget->setItem(row,3, new QTableWidgetItem(QString::number(temp->rampup)));
     ui->tableWidget->setItem(row,4, new QTableWidgetItem(QString::number(temp->offset)));
     ui->tableWidget->setItem(row,5, new QTableWidgetItem(QString::number(temp->loopover)));
     ui->tableWidget->setItem(row,6, new QTableWidgetItem(QString::number(temp->loopovertimespan)));
     row = row +1;

    }



}


// Edit button clicked
void TrafficWindow::on_editButton_clicked()
{



    if(lastRow >= 0){
    if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) != 0 ) ){
    Session* session = this->sessionList.at(this->lastRow);
    Window* newSess = new Window(this);
    newSess->show();
    newSess->displaySession(session);
    //this->close();
    }
}

}


// Delete button
void TrafficWindow::on_deleteButton_clicked()
{
    if(lastRow >= 0){
    if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) != 0 ) ){
        this->sessionList.removeAt(lastRow);
        ui->deleteButton->setEnabled(false);
    }

}
    this->displaySessions();
    lastRow = -1;
}

// New session button
void TrafficWindow::on_newButton_clicked()
{
    NewSessionDialog* predefWin = new NewSessionDialog(this);
    predefWin->exec();
}


// Item is selected ( better way to do this? )
void TrafficWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    ui->deleteButton->setEnabled(true);
    ui->editButton->setEnabled(true);
}

void TrafficWindow::on_tableWidget_itemSelectionChanged()
{
    ui->deleteButton->setEnabled(true);
    ui->editButton->setEnabled(true);
}
