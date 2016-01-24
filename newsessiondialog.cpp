#include "newsessiondialog.h"
#include "ui_newsessiondialog.h"
#include "trafficwindow.h"
#include "createsession.h"
#include "sessionloader.h"
#include "session.h"
#include <QDir>
#include <QDebug>
#include <QMessageBox>

NewSessionDialog::NewSessionDialog(TrafficWindow *parent) :
    QDialog(parent),
    ui(new Ui::NewSessionDialog)
{
    parentPointer = parent;
    // Get rid of the help button from top right.
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->setupUi(this);
    // Double clicking list is same as accepting.
    connect(ui->predefinedList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_buttonBox_accepted()));

    // Disable OK button
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
    QDir folder("predefined");

    // Does predefined folder exist?
    if(folder.exists()){
        // Get all xml files from folder.
        QStringList filter;
        filter << "*.xml";
        QStringList xmlList = folder.entryList(filter,QDir::Files);
        for(int i = 0; i < xmlList.size(); i++){
            ui->predefinedList->addItem(xmlList.at(i));
        }
    }
}

NewSessionDialog::~NewSessionDialog()
{
    delete ui;
}

// Return the amount of predefined sessions found.
unsigned int NewSessionDialog::getPredefinedAmount(){
    return ui->predefinedList->count();
}

// One predefinned session selected, go to session window with
// the data.
void NewSessionDialog::on_buttonBox_accepted()
{
    QString error = "";
    QString filepath = "predefined/" + ui->predefinedList->currentItem()->text();
    SessionLoader predefinedLoad(filepath);

    // The predefined session could't been loaded.
    if(!predefinedLoad.checkSession(error)){
        QMessageBox messageBox;
        messageBox.critical(0,"Error",error);
        messageBox.setFixedSize(500,200);
    }

    // Open the createsession with predefined sessions data.
    else{
        Session* predefinedSession = predefinedLoad.loadSession();
        createsession* newSess = new createsession(this->parentWidget(), parentPointer);
        newSess->setSession(predefinedSession);
        newSess->show();
        this->accept();
    }
}

// Create a completely new session
void NewSessionDialog::on_createButton_clicked()
{

    createsession* newSess = new createsession(this->parentWidget(), parentPointer);
    newSess->show();
    this->accept();
}

// Item is selected from the list, enable ok button.
void NewSessionDialog::on_predefinedList_itemSelectionChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( true );
}
