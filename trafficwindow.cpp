#include "trafficwindow.h"
#include "ui_trafficwindow.h"
#include "sessionwindow.h"
#include "session.h"
#include "newsessiondialog.h"
#include "sessionloader.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

TrafficWindow::TrafficWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrafficWindow)
{

    ui->setupUi(this);
    connect(ui->actionNew_Session,SIGNAL(triggered(bool)), this, SLOT(on_newButton_clicked()));
    ui->tableWidget->removeRow(0);

    // Set table headers to be the same size.
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

    // Clear table
    ui->tableWidget->clearSelection();
    ui->tableWidget->setRowCount(0);

    while (i.hasNext())
    {
        Session* temp = i.next();

     ui->tableWidget->insertRow(row);
     ui->tableWidget->setItem(row,0, new QTableWidgetItem(temp->sessName));

     // Disable name editing
     ui->tableWidget->item(row,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

     ui->tableWidget->setItem(row,1, new QTableWidgetItem(QString::number(temp->multiply)));
     ui->tableWidget->setItem(row,2, new QTableWidgetItem(QString::number(temp->rampup)));
     ui->tableWidget->setItem(row,3, new QTableWidgetItem(QString::number(temp->offset)));
     ui->tableWidget->setItem(row,4, new QTableWidgetItem(QString::number(temp->loopover)));
     ui->tableWidget->setItem(row,5, new QTableWidgetItem(QString::number(temp->loopovertimespan)));
     row++;

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
        }
    }

}


// Delete button
void TrafficWindow::on_deleteButton_clicked()
{
    if(lastRow >= 0){
    if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) != 0 ) ){
        this->sessionList.removeAt(lastRow);

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


// Item selection is changed either by clicking or using keyboard
// Enable delete and edit buttons and change the lastrow variable
// which tells the selected row.
void TrafficWindow::on_tableWidget_itemSelectionChanged()
{
    if(ui->tableWidget->selectedItems().size() > 0){
        ui->deleteButton->setEnabled(true);
        ui->editButton->setEnabled(true);
        this->lastRow = ui->tableWidget->selectedItems().at(0)->row();
    }
    else{

        // Nothing is selected or selected was edited or deleted
        ui->deleteButton->setEnabled(false);
        ui->editButton->setEnabled(false);
    }


}

// Mouse is over the Name colunm, show tool tip.
void TrafficWindow::on_tableWidget_itemEntered(QTableWidgetItem *item)
{

    QString ipdest = "";
    QString ipsour = "";
    QString portdest = "??";
    QString portsour = "??";
    if(item->column() == 0){

        // Search the right session from list
        for(int i = 0; i < sessionList.size(); ++i){
            if(sessionList.at(i)->sessName == item->text()){
                ipdest = sessionList.at(i)->dstIP;
                ipsour = sessionList.at(i)->srcIP;
                // TODO ports
            }
        }
        const QString tooltiptext = "Destination IP: " + ipdest + "\n Source IP: " + ipsour + "\n Destination port: " + portdest + "\n Source port: " + portsour;
        ui->tableWidget->setToolTip(tooltiptext);
    }
    else {
       ui->tableWidget->setToolTip("");
    }

}

void TrafficWindow::on_loadButton_clicked()
{
    QString error = "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Session"),"",tr("Session XML file (*.xml)"));
    qDebug() << filename;
    SessionLoader loader(filename);
    if(filename != ""){
        if(!loader.checkSession(error) ){
            QMessageBox messageBox;
            messageBox.critical(0,"Error",error);
            messageBox.setFixedSize(500,200);
        }
        else{
            qDebug() << "Create loaded session";
            this->sessionList.append(loader.loadSession());
            this->displaySessions();
        }
    }
}
