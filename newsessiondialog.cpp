#include "newsessiondialog.h"
#include "ui_newsessiondialog.h"
#include "trafficwindow.h"
#include "sessionwindow.h"

NewSessionDialog::NewSessionDialog(TrafficWindow *parent) :
    QDialog(parent),
    ui(new Ui::NewSessionDialog)
{
    parentPointer = parent;
    // Get rid of the help button from top right.
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    // Disable OK button
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( false );
}

NewSessionDialog::~NewSessionDialog()
{
    delete ui;
}

// One predefinned session selected, go to session window with
// data.
void NewSessionDialog::on_buttonBox_accepted()
{
    // TODO:    check selected and send data to sessionwindow
    //          Where are predefined sessions saved?
    Window* newSess = new Window(parentPointer);
    newSess->show();
}

// Create a completely new session
void NewSessionDialog::on_createButton_clicked()
{
    this->accept();
    Window* newSess = new Window(parentPointer);
    newSess->show();
}

// Item is selected from the list, enable ok button.
void NewSessionDialog::on_listWidget_itemSelectionChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( true );
}
