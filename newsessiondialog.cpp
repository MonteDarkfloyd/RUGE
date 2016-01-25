#include "newsessiondialog.h"
#include "ui_newsessiondialog.h"
#include "trafficwindow.h"
#include "createsession.h"
#include "sessionloader.h"
#include "session.h"
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QSignalMapper>

NewSessionDialog::NewSessionDialog(TrafficWindow *parent) :
    QDialog(parent),
    ui(new Ui::NewSessionDialog)
{
    parentPointer = parent;
    // Get rid of the help button from top right.
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->setupUi(this);
    // Double clicking list is same as accepting.
    //connect(ui->predefinedList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_buttonBox_accepted()));



    //Connect signal from Table so that whenever a row/column gets clicked
    //It sends a signal to the corresponding function

    connect( ui->predefinedList, SIGNAL( cellDoubleClicked (int, int) ),
     this, SLOT( cellSelected( int, int ) ) );



    // Disable OK button
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );


    //Fill the table
    this->fill();




    // Does predefined folder exist?
    /*if(folder.exists()){
        // Get all xml files from folder.
        QStringList filter;
        filter << "*.xml";
        QStringList xmlList = folder.entryList(filter,QDir::Files);
        for(int i = 0; i < xmlList.size(); i++){
            ui->predefinedList->addItem(xmlList.at(i));
        }
    }*/
}

void NewSessionDialog::fill(){

    //Create xmlreader for filling in table with predefined session


    //Read into list
    this->sessionList = Xreader.ReadPredefXML();

    //Get the number of sessions in list
    int predefNo = this->sessionList.count();


    //Set corresponding row and columns for table
    ui->predefinedList->setColumnCount(3);
    ui->predefinedList->setRowCount(predefNo);

    //Header variables if needed
    /*QStringList TableHeader;
    TableHeader<<"Session Name"<<""<<"";
    ui->predefinedList->setHorizontalHeaderLabels(TableHeader);*/

    //Hide header data
    ui->predefinedList->horizontalHeader()->hide();

    //Column customization
    ui->predefinedList->setColumnWidth(0,300);
    ui->predefinedList->setColumnWidth(1,85);
    ui->predefinedList->setColumnWidth(2,85);
    ui->predefinedList->verticalHeader()->setVisible(false);
    ui->predefinedList->setShowGrid(false);
    ui->predefinedList->setEditTriggers(QAbstractItemView::NoEditTriggers);


    //Fill in the name of sessions into the table from predefinedList
    QString sessName;
    for(int i=0;i<predefNo;i++){
    sessName = this->sessionList.at(i)->getName();
    ui->predefinedList->setItem(i,0,new QTableWidgetItem(sessName));


    //Add edit button next to name
    QPushButton *editButton = new QPushButton("Edit");
    ui->predefinedList->setCellWidget(i,1,editButton);
    editButton->setAttribute(Qt::WA_DeleteOnClose);

    editButton->setFixedWidth(75);


    //Add delete button next to name
    QPushButton *delButton = new QPushButton("Delete");


    ui->predefinedList->setCellWidget(i,2,delButton);
    delButton->setAttribute(Qt::WA_DeleteOnClose);
    delButton->setFixedWidth(75);


    //Map the signals of edit and delete buttons so they call
    //the function with correct row and column numbers
    QSignalMapper *signalMapper = new QSignalMapper(this);
    QSignalMapper *signalMapper2= new QSignalMapper(this);
    signalMapper->setMapping(editButton, int(i) );
    signalMapper2->setMapping(delButton, int(i) );
    connect(editButton, SIGNAL(clicked(bool)), signalMapper, SLOT(map()));
    connect(delButton, SIGNAL(clicked(bool)), signalMapper2, SLOT(map()));
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(editClicked(int)));
    connect(signalMapper2, SIGNAL(mapped(int)), this, SLOT(deleteClicked(int)));

    }


}






NewSessionDialog::~NewSessionDialog()
{
    delete ui;
}

// Return the amount of predefined sessions found.
unsigned int NewSessionDialog::getPredefinedAmount(){
    return this->sessionList.count();
}

// One predefinned session selected, go to session window with
// the data.
void NewSessionDialog::on_buttonBox_accepted()
{
    /*QString error = "";
    QString filepath = "predefined/" + ui->predefinedList->currentItem()->text();
    SessionLoader predefinedLoad(filepath);

    // The predefined session could't been loaded.
    if(!predefinedLoad.checkSession(error)){
        QMessageBox messageBox;
        messageBox.critical(0,"Error",error);
        messageBox.setFixedSize(500,200);
    }
    */


    // Open the createsession with predefined sessions data.
    int index = ui->predefinedList->currentRow();
    //If index is invalid
    if(index < 0 || this->sessionList.at(index) == NULL)
        QMessageBox::warning(this,"Error","Invalid index");
    else{
        Session* predefinedSession = this->sessionList.at(index);
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
    connect(newSess,SIGNAL(predefEdited()),this,SLOT(update()));
    newSess->show();


}

void NewSessionDialog::cellSelected(int row, int col){
    if(col ==0){
        createsession* newSess = new createsession(this->parentWidget(), parentPointer);
        newSess->setSession(this->sessionList.at(row));
        newSess->show();
        this->accept();
    }else if(col == 1){

        createsession* newSess = new createsession(this, parentPointer);
        connect(newSess,SIGNAL(predefEdited()),this,SLOT(update()));
        newSess->editPredefinedView(row);
        newSess->setSession(this->sessionList.at(row));
        newSess->show();
    }else if(col == 2){

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm deletion",
        "Are you sure you want to delete " + this->sessionList.at(row)->getName() ,
                                        QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
        this->xWriter.remove(row);
        this->update();
        }else{

        }
    }

}

void NewSessionDialog::editClicked(int nRow){
    this->cellSelected(nRow,1);
}

void NewSessionDialog::deleteClicked(int nRow){
    this->cellSelected(nRow,2);

}

void NewSessionDialog::clearList(){
    this->sessionList.clear();
}


void NewSessionDialog::update(){
    this->clearList();
    this->fill();
}




// Item is selected from the list, enable ok button.
void NewSessionDialog::on_predefinedList_itemSelectionChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( true );
}
