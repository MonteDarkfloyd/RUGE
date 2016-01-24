#include "trafficwindow.h"
#include "ui_trafficwindow.h"
#include "session.h"
#include "newsessiondialog.h"
#include "createsession.h"
#include "sessionloader.h"
#include "sessionsaver.h"
#include "trafficloader.h"
#include "trafficsaver.h"
#include "tabledelegate.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>



TrafficWindow::TrafficWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrafficWindow)
{

    ui->setupUi(this);
    connect(ui->actionNew_Session,SIGNAL(triggered(bool)), this, SLOT(on_newButton_clicked()));
    connect(ui->actionLoad_Traffic_Profile,SIGNAL(triggered(bool)),this,SLOT(on_loadTButton_clicked()));
    connect(ui->actionSoft_Reset,SIGNAL(triggered(bool)),this,SLOT(on_resetButton_clicked()));
    ui->tableWidget->removeRow(0);
    saveCancel = false;
    edited = false;

    // Set table headers to be the same size.
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set table header styles
    ui->tableWidget->horizontalHeader()->setStyleSheet("::section{background: #e4f2f1; font-family: arial; font-size: 8pt}");

    ui->tableWidget->setItemDelegate(new TableDelegate);
    ui->startButton->setEnabled(false);

    // Set menubar font
    ui->menuBar->setStyleSheet("font-size:12px;font-family: arial");

}


TrafficWindow::~TrafficWindow()
{
    deleteSessions();
    delete ui;
}

// Top menu quit
void TrafficWindow::on_actionQuit_triggered()
{
  QApplication::quit();
}


void TrafficWindow::addSession(Session *newSess){
    this->sessionList.append(newSess);
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
     ui->tableWidget->setItem(row,0, new QTableWidgetItem(temp->getName()));

     // Disable name editing
     ui->tableWidget->item(row,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

     ui->tableWidget->setItem(row,1, new QTableWidgetItem(temp->getMultiply()));
     ui->tableWidget->setItem(row,2, new QTableWidgetItem(temp->getRampup()));
     ui->tableWidget->setItem(row,3, new QTableWidgetItem(temp->getOffset()));
     ui->tableWidget->setItem(row,4, new QTableWidgetItem(temp->getLoopover()));
     ui->tableWidget->setItem(row,5, new QTableWidgetItem(temp->getLoopoverTimespan()));
     row++;

    }
    if(!sessionList.empty()){
        ui->saveTButton->setEnabled(true);
        ui->startButton->setEnabled(true);
    }
    else{
        ui->saveTButton->setEnabled(false);
        ui->startButton->setEnabled(false);
    }
    edited = true;

    // Select previous selection.
   if(lastRow >= 0){
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(lastRow,0));
   }

}


// Edit button clicked
void TrafficWindow::on_editButton_clicked()
{

    if(lastRow >= 0){
        if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) != 0 ) ){
            Session* session = this->sessionList.at(lastRow);
            createsession* editWindow = new createsession(this,this);
            editWindow->setSession(session);
            editWindow->setEditMode();
            editWindow->show();
        }
    }

}

// Checks if given name already exist in the sessionList.
bool TrafficWindow::checkName(QString name){
    for(int i = 0; i < sessionList.size(); ++i){
        if(name == sessionList.at(i)->getName()){
            return false;
        }
    }
    return true;
}


// Delete button
void TrafficWindow::on_deleteButton_clicked()
{
    if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) != 0 ) ){
        delete sessionList.at(lastRow);
        sessionList.removeAt(lastRow);
    }
    lastRow--;
    if(sessionList.size() > 0 && lastRow < 0){
        lastRow++;
    }
    displaySessions();


}

// New session button
void TrafficWindow::on_newButton_clicked()
{
    NewSessionDialog* predefWin = new NewSessionDialog(this);
    if(predefWin->getPredefinedAmount() == 0){
            createsession* newSess = new createsession(this, this);
            newSess->show();
            delete predefWin;
        }
    else {
        predefWin->exec();
    }
    //createsession* newSess = new createsession();//qobject_cast<TrafficWindow*>(this));
    //newSess->show();
}


// Item selection is changed either by clicking or using keyboard
// Enable delete and edit buttons and change the lastrow variable
// which tells the selected row.
void TrafficWindow::on_tableWidget_itemSelectionChanged()
{
    if(ui->tableWidget->selectedItems().size() > 0){
        ui->deleteButton->setEnabled(true);
        ui->editButton->setEnabled(true);
        ui->saveSButton->setEnabled(true);
        lastRow = ui->tableWidget->selectedItems().at(0)->row();
    }
    else{

        // Nothing is selected or selected was edited or deleted
        ui->deleteButton->setEnabled(false);
        ui->editButton->setEnabled(false);
        ui->saveSButton->setEnabled(false);
        ui->tableWidget->setToolTip("");
    }


}

// Mouse is over the Name colunm, show tool tip.
void TrafficWindow::on_tableWidget_itemEntered(QTableWidgetItem *item)
{

    QString ipdest = "Default";
    QString ipsour = "Default";
    QString macsour = "Default";
    QString macdest = "Default";
    QString portdest = "Default";
    QString portsour = "Default";
    QString payloadtext = "No Payload";
    if(item->column() == 0){

        // Search the right session from list
        for(int i = 0; i < sessionList.size(); ++i){
            if(sessionList.at(i)->getName() == item->text()){
                ipdest = sessionList.at(i)->getDstIP().value;
                ipsour = sessionList.at(i)->getSrcIP().value;
                macsour = sessionList.at(i)->getSrcMAC();
                macdest = sessionList.at(i)->getDstMAC();
                payloadtext = sessionList.at(i)->getPayload();
                QString tooltiptext = " Destination IP: " + ipdest + "\n Source IP: " + ipsour + "\n";
                tooltiptext = tooltiptext + " MAC Source: " + macsour + "\n MAC Destination: " + macdest + "\n Payload: " + payloadtext;
                ui->tableWidget->item(i,0)->setToolTip(tooltiptext);
            }
        }

    }
}

// Load session button
void TrafficWindow::on_loadSButton_clicked()
{
    QString error = "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Session"),"session_profiles/",tr("Session XML file (*.xml)"));
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

void TrafficWindow::on_saveSButton_clicked()
{
    if(lastRow >= 0){
        if( (this->sessionList.size() > 0 )&& (this->sessionList.at(lastRow) != 0 ) ){
                 QString filename = QFileDialog::getSaveFileName(this, tr("Save Session"),"session_profiles/" + sessionList.at(lastRow)->getName(),tr("Session XML file (*.xml)"));
                 SessionSaver saver(sessionList.at(lastRow),filename);
                 saver.Save_Session();
        }
      }
}

// Load traffic button pressed.
void TrafficWindow::on_loadTButton_clicked()
{
    QString error = "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Traffic Profile"),"traffic_profiles/",tr("Traffic XML file (*.xml)"));
    TrafficLoader loader(filename);
    if(filename != ""){
        if(!loader.checkTraffic(error) ){
            QMessageBox messageBox;
            messageBox.critical(0,"Error",error);
            messageBox.setFixedSize(500,200);
        }
        else{
            qDebug() << "Create loaded traffic" << filename;
            deleteSessions();
            sessionList = loader.loadTraffic();
            this->displaySessions();
            // Set edited to false. The function above sets it true.
            // But here we haven't edited anything yet.
            edited = false;
            currentTraffic = filename;
            qDebug() << currentTraffic;
            ui->startButton->setEnabled(true);
        }
    }
}

// Used to delete all sessions permanently.
// Used when loading traffic profile.
void TrafficWindow::deleteSessions(){
    for(int i = 0; i < sessionList.size(); ++i){
        delete sessionList.at(i);
    }
    sessionList.clear();
}


// Start generation button.
void TrafficWindow::on_startButton_clicked()
{
    if(currentTraffic == ""){
        QMessageBox messageBox;
        QMessageBox::StandardButton saveAnswer;
        saveAnswer = messageBox.information(0,"Save traffic file",
                               "Traffic Profile needs to be saved before starting packet generation. Do you want to save it?"
                               ,QMessageBox::Yes|QMessageBox::No);
        if(saveAnswer == QMessageBox::No){
            return;
        }
        else{
            on_saveTButton_clicked();
        }

    }
    qDebug() << "Start Generation";
    if(edited){
        // Create a messagebox that asks overwriting
        QString overwriteText = "Traffic profile and/or sessions has been edited. \nSave and continue?";
        QMessageBox overw(QMessageBox::Question,"Overwrite",overwriteText,QMessageBox::Yes|QMessageBox::SaveAll|QMessageBox::No);
        overw.setButtonText(QMessageBox::SaveAll,"Overwrite all");
        int answer = overw.exec();
        TrafficSaver saver(currentTraffic,sessionList);
        switch (answer) {
        case QMessageBox::Yes:
            on_saveTButton_clicked();
            break;
        case QMessageBox::No:
            return;
            break;
        case QMessageBox::SaveAll:

            // Save and don't ask overwriting.
            saver.saveTraffic(false);
            edited = false;
            break;
        default:
            break;
        }
    }

    // Saving was cancelled when asked above in the yes answer.
    if(saveCancel){
        return;
    }


    QFileInfo name(currentTraffic);
    QString trafficFile = name.fileName();

    QString program = "RUGE";
    QStringList arguments;
    arguments << trafficFile;


    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);

    myProcess->waitForFinished();
    QString strOut = myProcess->readAllStandardOutput();

    QMessageBox messageBox;
    messageBox.setText(strOut);
    messageBox.exec();
}

// Save traffic button.
void TrafficWindow::on_saveTButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Traffic"),"traffic_profiles/",tr("Traffic XML file (*.xml)"));
    if(filename != ""){
        TrafficSaver saver(filename,sessionList);
        saver.saveTraffic();
        saveCancel = false;
        edited = false;
        currentTraffic =filename;
        qDebug() << currentTraffic;
        ui->startButton->setEnabled(true);
        return;
    }
    saveCancel = true;


}


// Reset
void TrafficWindow::on_resetButton_clicked()
{
    QString program = "RUGE";
    QStringList arguments;
    arguments << "-r";
    arguments << "soft";


    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);

    myProcess->waitForFinished();
    QString strOut = myProcess->readAllStandardOutput();

    QMessageBox messageBox;
    messageBox.setText(strOut);
    messageBox.exec();
}

// Hard reset
void TrafficWindow::on_actionHard_Reset_triggered()
{
    QString program = "RUGE";
    QStringList arguments;
    arguments << "-r";
    arguments << "hard";


    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);

    myProcess->waitForFinished();
    QString strOut = myProcess->readAllStandardOutput();

    QMessageBox messageBox;
    messageBox.setText(strOut);
    messageBox.exec();
}

// When table is edited, set the values to maximum value if necessary.
// And save the value to session.
void TrafficWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{

    // Multiply
    if(item->column() == 1){
        if(item->text().toLongLong() > 1000000){
            item->setText("1000000");
        }
        edited = true;
        sessionList.at(item->row())->setMultiply(item->text());
    }
    // rampup
    else if(item->column() == 2){
        if(item->text().toLongLong() > 1000000000){
            item->setText("1000000000");
        }
        edited = true;
        sessionList.at(item->row())->setRampup(item->text());
    }
    // offset
    else if(item->column() == 3){
        if(item->text().toLongLong() > 1000000000){
            item->setText("1000000000");
        }
        edited = true;
        sessionList.at(item->row())->setOffset(item->text());
    }
    // loopover
    else if(item->column() == 4){
        if(item->text().toLongLong() > 1000000){
            item->setText("1000000");
        }
        edited = true;
        sessionList.at(item->row())->setLoopover(item->text());
    }
    // loopovertimespan
    else if(item->column() == 5){
        edited = true;
        sessionList.at(item->row())->setLoopoverTimespan(item->text());
    }

}

void TrafficWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    if(item->column() == 0){
        item->setSelected(true);
        on_editButton_clicked();
    }
    else{
        item->setSelected(true);
    }

}
