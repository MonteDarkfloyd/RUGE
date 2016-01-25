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
#include <QProcess>



TrafficWindow::TrafficWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrafficWindow)
{

    ui->setupUi(this);
    connect(ui->actionNew_Session,SIGNAL(triggered(bool)), this, SLOT(on_newButton_clicked()));
    connect(ui->actionLoad_Traffic_Profile,SIGNAL(triggered(bool)),this,SLOT(on_loadTButton_clicked()));
    connect(ui->actionSoft_Reset,SIGNAL(triggered(bool)),this,SLOT(on_resetButton_clicked()));

    // Remove the empty row from table.
    ui->tableWidget->removeRow(0);
    saveCancel_ = false;
    edited_ = false;

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

// Used to update the table in the view
// that shows the sessions.
void TrafficWindow::displaySessions(){

    QListIterator<Session *> i(this->sessionList_);
    int row = 0;

    // Clear the table
    ui->tableWidget->clearSelection();
    ui->tableWidget->setRowCount(0);

    // Go throught the sessionList_ and add the name
    // and other info to table.
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

    // If we have sessions enable save Traffic and start button.
    if(!sessionList_.empty()){
        ui->saveTButton->setEnabled(true);
        ui->startButton->setEnabled(true);
    }
    else{
        ui->saveTButton->setEnabled(false);
        ui->startButton->setEnabled(false);
    }

    // When this function is called, it is very likely that something was
    // edited_.
    edited_ = true;

    // Select previous selection.
   if(selectedRow_ >= 0){
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(selectedRow_,0));
   }

}

// Add new session to the sessionList_.
// Will also do displaySessions when used.
void TrafficWindow::addSession(Session *newSess){
    this->sessionList_.append(newSess);
    this->displaySessions();

}

// Checks if given name already exist in the sessionList_.
bool TrafficWindow::checkName(QString name){
    for(int i = 0; i < sessionList_.size(); ++i){
        // Name is in use, return true.
        if(name == sessionList_.at(i)->getName()){
            return false;
        }
    }
    return true;
}

// Quit is selected from top menu
// Quit the program
void TrafficWindow::on_actionQuit_triggered()
{
  QApplication::quit();
}


// Edit button is clicked. Send the selected sessions
// data to the createsession view so user can edit it.
void TrafficWindow::on_editButton_clicked()
{
// Should not happen, but check that there really is session to edit.
    if(selectedRow_ >= 0){
        if( (this->sessionList_.size() > 0 )&& (this->sessionList_.at(selectedRow_) != 0 ) ){
            Session* session = this->sessionList_.at(selectedRow_);
            createsession* editWindow = new createsession(this,this);
            editWindow->setSession(session);
            editWindow->setEditMode();
            editWindow->show();
        }
    }

}

// Delete the selected session when delete is clicked.
void TrafficWindow::on_deleteButton_clicked()
{
    if( (this->sessionList_.size() > 0 )&& (this->sessionList_.at(selectedRow_) != 0 ) ){
        delete sessionList_.at(selectedRow_);
        sessionList_.removeAt(selectedRow_);
    }
    selectedRow_--;
    // We deleted the first session, put selectedRow_ forward by one.
    if(sessionList_.size() > 0 && selectedRow_ < 0){
        selectedRow_++;
    }
    displaySessions();


}

// Add new session by opening newsessiondialog or if
// there is no predefined sessions then move to createsession view.
void TrafficWindow::on_newButton_clicked()
{
    NewSessionDialog* predefWin = new NewSessionDialog(this);

    // If there is no predefined sessions, go straight to create session
    // view.
    if(predefWin->getPredefinedAmount() == 0){
            createsession* newSess = new createsession(this, this);
            newSess->show();
            delete predefWin;
        }
    else {
        predefWin->exec();
    }
}


// Item selection is changed either by clicking or using keyboard
// Enable delete and edit buttons and change the selectedRow_ variable
// which tells the selected row.
void TrafficWindow::on_tableWidget_itemSelectionChanged()
{
    if(ui->tableWidget->selectedItems().size() > 0){
        ui->deleteButton->setEnabled(true);
        ui->editButton->setEnabled(true);
        ui->saveSButton->setEnabled(true);
        // Single selection is on, so only one item should be selected.
        selectedRow_ = ui->tableWidget->selectedItems().at(0)->row();
    }
    else{

        // Nothing is selected or selected was edited_ or deleted
        ui->deleteButton->setEnabled(false);
        ui->editButton->setEnabled(false);
        ui->saveSButton->setEnabled(false);
        ui->tableWidget->setToolTip("");
    }


}

// Mouse is over an item in the table. If the item is the session name
// set a new tooltip.
void TrafficWindow::on_tableWidget_itemEntered(QTableWidgetItem *item)
{

    QString ipdest;
    QString ipsour;
    QString macsour;
    QString macdest;
    QString portdest;
    QString portsour;
    QString payloadtext;

    // Item is the session name.
    if(item->column() == 0){

        // Search the right session from list
        for(int i = 0; i < sessionList_.size(); ++i){
            if(sessionList_.at(i)->getName() == item->text()){

                // Set the tooltip text.
                ipdest = sessionList_.at(i)->getDstIP().value;
                ipsour = sessionList_.at(i)->getSrcIP().value;
                macsour = sessionList_.at(i)->getSrcMAC();
                macdest = sessionList_.at(i)->getDstMAC();
                payloadtext = sessionList_.at(i)->getPayload();
                QString tooltiptext = " Destination IP: " + ipdest + "\n Source IP: " + ipsour + "\n";
                tooltiptext = tooltiptext + " MAC Source: " + macsour + "\n MAC Destination: " + macdest + "\n Payload: " + payloadtext;
                ui->tableWidget->item(i,0)->setToolTip(tooltiptext);
            }
        }

    }
}

// Load session button, open file dialog and then try to open
// the xml user chose.
void TrafficWindow::on_loadSButton_clicked()
{
    QString error = "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Session"),"session_profiles/",tr("Session XML file (*.xml)"));  
    if(filename != ""){
        SessionLoader loader(filename);
        // Check if the session is valid.
        if(!loader.checkSession(error) ){
            QMessageBox messageBox;
            messageBox.critical(0,"Error",error);
            messageBox.setFixedSize(500,200);
        }
        // Add session to sessionList_ and update table.
        else{
            this->sessionList_.append(loader.loadSession());
            this->displaySessions();
        }
    }
}

// Save session button, open file dialog and then try to save
// the session profile file.
void TrafficWindow::on_saveSButton_clicked()
{
    // Should not happen, but check that there really is session to save.
    if(selectedRow_ >= 0){
        if( (this->sessionList_.size() > 0 )&& (this->sessionList_.at(selectedRow_) != 0 ) ){
                 QString filename = QFileDialog::getSaveFileName(this, tr("Save Session"),"session_profiles/" + sessionList_.at(selectedRow_)->getName(),tr("Session XML file (*.xml)"));
                 if(filename != ""){
                     SessionSaver saver(sessionList_.at(selectedRow_),filename);
                     saver.Save_Session();
                 }
        }
    }
}

// Load traffic button, open file dialog and then try to open
// the xml user chose.
void TrafficWindow::on_loadTButton_clicked()
{
    // Make filedialog
    QString error = "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Traffic Profile"),"traffic_profiles/",tr("Traffic XML file (*.xml)"));
    TrafficLoader loader(filename);

    // File dialog not cancelled
    if(filename != ""){
        // Try to load the traffic profile.
        if(!loader.checkTraffic(error) ){
            QMessageBox messageBox;
            messageBox.critical(0,"Error",error);
            messageBox.setFixedSize(500,200);
        }

        else{
            // Delete all old sessions.
            deleteSessions();
            sessionList_ = loader.loadTraffic();
            this->displaySessions();

            // Set edited_ to false. The function above sets it true.
            // But here we haven't edited_ anything yet.
            edited_ = false;
            currentTraffic_ = filename;
            ui->startButton->setEnabled(true);
        }
    }
}


// Save traffic button, open file dialog and then try to save
// the traffic profile file and all sessions.
void TrafficWindow::on_saveTButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Traffic"),"traffic_profiles/",tr("Traffic XML file (*.xml)"));
    if(filename != ""){
        TrafficSaver saver(filename,sessionList_);
        saver.saveTraffic();
        saveCancel_ = false;
        edited_ = false;
        currentTraffic_ =filename;
        ui->startButton->setEnabled(true);
        return;
    }
    // Save was cancelled.
    saveCancel_ = true;

}

// Start generation button. First, if edited_, ask user to save the traffic profile.
// Then use command line to sent the traffic file for the RUGE program.
void TrafficWindow::on_startButton_clicked()
{
    if(currentTraffic_ == ""){
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

    if(edited_){
        // Create a messagebox that asks overwriting
        QString overwriteText = "Traffic profile and/or sessions has been edited. \nSave and continue?";
        QMessageBox overw(QMessageBox::Question,"Overwrite",overwriteText,QMessageBox::Yes|QMessageBox::SaveAll|QMessageBox::No);
        overw.setButtonText(QMessageBox::SaveAll,"Overwrite all");
        int answer = overw.exec();
        TrafficSaver saver(currentTraffic_,sessionList_);
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
            edited_ = false;
            break;
        default:
            break;
        }
    }

    // Saving was cancelled when asked above in the yes answer.
    if(saveCancel_){
        return;
    }

    // Build the command line command
    QString reset = "RUGE";
    QStringList resetarg;
    resetarg << "-r";
    resetarg << "soft";

    // Start the RUGE program
    QProcess *resetProcess = new QProcess(this);
    resetProcess->start(reset, resetarg);

    resetProcess->waitForFinished();

    // Make the command line command.
    QFileInfo name(currentTraffic_);
    QString trafficFile = name.fileName();

    QString program = "RUGE";
    QStringList arguments;
    arguments << trafficFile;

    // Start the RUGE program
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);

    myProcess->waitForFinished();
    QString strOut = myProcess->readAllStandardOutput();

    // Make messagebox for the RUGE result
    QMessageBox messageBox;
    messageBox.setText(strOut);
    messageBox.exec();
}

// Reset button pressed. Use command line to send
// a command to the RUGE program to soft reset the engine
void TrafficWindow::on_resetButton_clicked()
{
    // Build the command line command
    QString program = "RUGE";
    QStringList arguments;
    arguments << "-r";
    arguments << "soft";

    // Start the RUGE program
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);

    myProcess->waitForFinished();
    QString strOut = myProcess->readAllStandardOutput();

    // Make messagebox for the reset result.
    QMessageBox messageBox;
    messageBox.setText(strOut);
    messageBox.exec();
}

// Hard reset chosed from top menu. Use command line to send
// a command to the RUGE program to hard reset the engine
void TrafficWindow::on_actionHard_Reset_triggered()
{
    // Build the command line command
    QString program = "RUGE";
    QStringList arguments;
    arguments << "-r";
    arguments << "hard";

    // Start the RUGE program
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);

    myProcess->waitForFinished();

    // Make messagebox for the reset result.

        QMessageBox messageBox;
        messageBox.setText("Hard reset has finished.");
        messageBox.exec();

}

// When table is edited_, set the values to maximum value if necessary.
// And save the value to session.
void TrafficWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{

    // Multiply
    if(item->column() == 1){
        if(item->text().toLongLong() > 1000000){
            item->setText("1000000");
        }
        edited_ = true;
        sessionList_.at(item->row())->setMultiply(item->text());
    }
    // rampup
    else if(item->column() == 2){
        if(item->text().toLongLong() > 1000000000){
            item->setText("1000000000");
        }
        edited_ = true;
        sessionList_.at(item->row())->setRampup(item->text());
    }
    // offset
    else if(item->column() == 3){
        if(item->text().toLongLong() > 1000000000){
            item->setText("1000000000");
        }
        edited_ = true;
        sessionList_.at(item->row())->setOffset(item->text());
    }
    // loopover
    else if(item->column() == 4){
        if(item->text().toLongLong() > 1000000){
            item->setText("1000000");
        }
        edited_ = true;
        sessionList_.at(item->row())->setLoopover(item->text());
    }
    // loopovertimespan
    else if(item->column() == 5){
        edited_ = true;
        sessionList_.at(item->row())->setLoopoverTimespan(item->text());
    }

}

// Item was double clicked. If item was session name, move to the edit
// otherwise enter editing the value in the table.
void TrafficWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    // Name is doubleclicked
    if(item->column() == 0){
        item->setSelected(true);
        on_editButton_clicked();
    }
    else{
        item->setSelected(true);
    }

}

// Used to delete all sessions permanently.
// Used when loading traffic profile.
void TrafficWindow::deleteSessions(){
    for(int i = 0; i < sessionList_.size(); ++i){
        delete sessionList_.at(i);
    }
    sessionList_.clear();
}

// About button triggered
void TrafficWindow::on_actionAbout_triggered()
{
    // Make messagebox for the reset result.
    QMessageBox messageBox;
    messageBox.setText("Ruge Slim GUI version 1.0");
    messageBox.exec();
}
