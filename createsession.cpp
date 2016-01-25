#include "createsession.h"
#include "ui_createsession.h"
#include "session.h"
#include "variabledata.h"
#include "sessionsaver.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHostAddress>
#include <QKeyEvent>
#include <QSettings>

QVBoxLayout* v_ip4_layout;

createsession::createsession(QWidget *parent, TrafficWindow* Tparent) :
    QWidget(parent),
    ui(new Ui::createsession)
{
    parentPointer = Tparent;
    editMode = false;
    createdSession_ = 0;
    editSession_ = 0;
    Qt::WindowFlags flags = windowFlags();

    // Set window to windowmodal
    this->setWindowFlags(flags | Qt::Window);
    this->setWindowModality(Qt::WindowModal);
    ui->setupUi(this);

    // Hide most information at the beginning
    ui->groupBox_2->setVisible(false);
    ui->groupBox_5->setVisible(false);
    ui->groupBox_6->setVisible(false);

    ui->payloadEdit->setEnabled(false);

    // Set validator to the ports
    ui->txt_udp_src_port->setValidator(new QIntValidator(0, 65535, this) );
    ui->txt_udp_dest_port->setValidator(new QIntValidator(0, 65535, this) );
    ui->label_mac_src->move(20,18);

    // Check if we find rugged.ini, if we do read the ip source
    if(QFile("rugged.ini").exists()){
        QSettings settings("rugged.ini", QSettings::IniFormat);
        settings.beginGroup("engine_interfaces");
        QString ipsource = settings.value("engines_IPv4_addresses", "0.0.0.0").toString();
        ui->txt_ip_src->setText(ipsource);
    }


}

createsession::~createsession()
{
    delete ui;
}

// Sets session when editing or loading a predefined session.
void createsession::setSession(Session* editedSession){
    editSession_ = editedSession;

    // Set items to UI
    ui->name_edit->setText(editSession_->getName());
    // Only UDP supported for now.
    ui->rb_ip4->setChecked(true);
    ui->rb_udp->setChecked(true);
    ui->txt_mac_dest->setText(editSession_->getDstMAC());

    // If MAC source is set, enable checkbox and set text.
    if(editSession_->getSrcMAC() != ""){
        ui->mac_checkBox->setChecked(true);
        ui->txt_mac_source->setText(editSession_->getSrcMAC());
    }
    // IP addresses
    ui->txt_ip_dest->setText(editedSession->getDstIP().value);

    // IP destination has max value so enable range.
    if(editedSession->getDstIP().max != ""){
        ui->ipDestComboBox->setCurrentIndex(1);
        ui->txt_ipdstmax->setEnabled(true);
        ui->label_ipdstinc->setEnabled(true);
        ui->txt_ipdstmax->setText(editedSession->getDstIP().max);
    }

    // Check if session has source IP. Also check if it has range.
    if(editSession_->getSrcIP().value != ""){
        ui->overrideipsrc->setChecked(true);
        ui->txt_ip_src->setText(editSession_->getSrcIP().value);

        // Range check.
        if(editedSession->getSrcIP().max != ""){
            ui->ipSourceComboBox->setCurrentIndex(1);
            ui->txt_ipsrcmax->setEnabled(true);
            ui->label_ipsrcinc->setEnabled(true);
            ui->txt_ipsrcmax->setText(editedSession->getSrcIP().max);
        }

    }
    // Set ports
    ui->txt_udp_dest_port->setText(editedSession->getVariable("UDP_DST_PORT").value);
    if(editedSession->getVariable("UDP_SRC_PORT").value != ""){
        ui->sourceport_checkBox->setChecked(true);
        ui->txt_udp_src_port->setText(editedSession->getVariable("UDP_SRC_PORT").value);
    }
    // Set payload
    if(editSession_->getPayload() != ""){
        ui->payload_checkBox->setChecked(true);
        ui->payloadEdit->setText(editSession_->getPayload());
    }
}

// When called sets edit mode to true.
// This will used when editing existing session.
void createsession::setEditMode(){
    editMode = true;
}

// Radio button IP4 toggled, show proper information.
void createsession::on_rb_ip4_toggled(bool checked)
{
    if(checked){
      ui->groupBox_2->setVisible(true);
      ui->groupBox_5->setVisible(false);

      // If udp is also toggled, enable confirm button
      if(ui->rb_udp->isChecked()){
         ui->confirm_Button->setEnabled(true);
      }
    }
}

// Radio button IP6 toggled, show proper information.
// This is disabled for now.
void createsession::on_lb_ip6_toggled(bool checked)
{
    if(checked){
      ui->groupBox_2->setVisible(false);
      ui->groupBox_5->setVisible(true);
    }
}

// Radio button TCP toggled, show proper information.
// This is disabled for now.
void createsession::on_rb_tcp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_6->setVisible(false);
    }
}

// Radio button UDP toggled, show proper information.
void createsession::on_rb_udp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_6->setVisible(true);
    }
    // If IP4 is also toggled, enable confirm button
    if(ui->rb_ip4->isChecked()){
       ui->confirm_Button->setEnabled(true);
    }
}

// Radio button ICMP toggled, show proper information.
// This is disabled for now.
void createsession::on_rb_icmp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_6->setVisible(false);
    }
}

// Radio button SCTP toggled, show proper information.
// This is disabled for now.
void createsession::on_rb_sctp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_6->setVisible(false);
    }
}

// Checkbox about MAC source toggled, enable/disable editing.
void createsession::on_mac_checkBox_toggled(bool checked)
{
    ui->label_mac_src->setEnabled(checked);
    ui->txt_mac_source->setEnabled(checked);
}

// Checkbox about IP source toggled, enable/disable editing.
void createsession::on_overrideipsrc_toggled(bool checked)
{
    ui->txt_ip_src->setEnabled(checked);
    ui->label_ipsrc->setEnabled(checked);
    ui->ipSourceComboBox->setEnabled(checked);
}

// Checkbox about source port toggled, enable/disable editing.
void createsession::on_sourceport_checkBox_toggled(bool checked)
{
    ui->txt_udp_src_port->setEnabled(checked);
    ui->label_src_port_udp->setEnabled(checked);
}

// Checkbox about payload toggled, enable/disable editing.
void createsession::on_payload_checkBox_toggled(bool checked)
{
    ui->payloadEdit->setEnabled(checked);

}


// When range is selected in ip source, enable increment editing.
void createsession::on_ipSourceComboBox_activated(int index)
{
    // Enable
    if(index != 0){

        ui->txt_ipsrcmax->setEnabled(true);
        ui->label_ipsrcinc->setEnabled(true);
        // Set the max range same as the beginning range.
        ui->txt_ipsrcmax->setText(ui->txt_ip_src->text());
    }
    else{
        ui->txt_ipsrcmax->setEnabled(false);
        ui->label_ipsrcinc->setEnabled(false);
    }
}

// When range is selected in ip destination, enable increment editing.
void createsession::on_ipDestComboBox_activated(int index)
{
    // Enable
    if(index != 0){

        ui->txt_ipdstmax->setEnabled(true);
        ui->label_ipdstinc->setEnabled(true);
        // Set the max range same as the beginning range.
        ui->txt_ipdstmax->setText(ui->txt_ip_dest->text());
    }
    else{
        ui->txt_ipdstmax->setEnabled(false);
        ui->label_ipdstinc->setEnabled(false);
    }
}

// Cancel button pressed. Close view.
void createsession::on_cancel_button_clicked()
{
    this->close();
}



// Add new session to the trafficwindow table or just edit
// the existing session depending on the value of boolean editMode.
// Close the window when done.
void createsession::on_confirm_Button_clicked()
{
    // Check if we can make session.
    if(makeSession()){
        // If editmode then just order trafficwindow to update
        // its table.
        if(editMode){
           parentPointer->displaySessions();
        }

        // Edit mode false. Order trafficwindow to add new session.
        else{
           parentPointer->addSession(createdSession_);
        }
      this->close();
    }
    // Session was not created. Delete it if
    // not in edit mode.
    else if(editSession_ == 0 && !editMode){
        delete createdSession_;
    }
}


// Save as Predefined button pressed.
void createsession::on_predefined_Button_clicked()
{
  // Try to create session from the information.
  if(!makeSession()){
      return;
  }

  // Add .xml to the session name if there is not one.
  createdSession_->addXMLextension();

  QFile check("predefined/" + createdSession_->getName());

  // If the file exists in predefined folder, ask overwriting.
    if(check.exists()){
        // Create a messagebox that asks overwriting
        QMessageBox::StandardButton overw;
        QString overwriteText = "Predefined session " + createdSession_->getName() + " already exists.\nOverwrite?";
        overw = QMessageBox::warning(0, "Overwrite?", overwriteText,
                                      QMessageBox::Yes|QMessageBox::No);
        // Overwrite the file
        if (overw == QMessageBox::Yes) {
            SessionSaver predefSaver(createdSession_,"predefined/" + createdSession_->getName());
            predefSaver.Save_Session();
        }
        // or else do nothing.
        else {
          return;
        }
      }

  // File did not exist, so save it.
  SessionSaver predefSaver(createdSession_,"predefined/" + createdSession_->getName());
  predefSaver.Save_Session();
  QMessageBox saveBox;
  saveBox.setText("Save was successful.");
  saveBox.exec();
}


// Create a session from the given data
// if possible. Return value false if failed.
// True if successful. Session is in createdSession_.
bool createsession::makeSession(){

    VariableData vdata;
    // Check if editing
    if(editMode){
        createdSession_ = editSession_;
    }

    // Not editing, create a new session.
    else{
        createdSession_ = new Session();
    }

    // Remove whitespace from the name.
    QString name = ui->name_edit->text().trimmed();

    // Check that name is not empty
    if(name.isEmpty()){
        QMessageBox messageBox;

        messageBox.critical(0,"No name inserted.","Please insert session name.");
        messageBox.setFixedSize(500,200);
        return false;
    }

    // Check if name is over 200 characters.
    if(name.size() > 200){
        QMessageBox messageBox;

        messageBox.critical(0,"Too long name.","Please insert shorter session name.");
        messageBox.setFixedSize(500,200);
        return false;
    }


    // Check if name is already in use and we are not in edit mode.
    if(!parentPointer->checkName(name) && !editMode){
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid name","Name is already in use.");
        messageBox.setFixedSize(500,200);
        return false;
    }
    createdSession_->setName(name);

    // Chance these when more protocols are added.
    createdSession_->setipVersion ("IPv4");
    createdSession_->setProtocol("UDP");

    // MAC destination, remove whitespace
    QString macdest = ui->txt_mac_dest->text();
    macdest = macdest.simplified();
    macdest.replace( " ", "" );
    // Check if mac dest. address is in right size
    if(macdest.size() < 17){
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid MAC destination","MAC destination address is not in right format.");
        messageBox.setFixedSize(500,200);
        return false;
    }

    // Get the variable data
    rugeVariable macDestData = vdata.getData("MAC_DST");
    macDestData.value = macdest;

    createdSession_->addVariable(macDestData);

    // Add mac source
    if(ui->mac_checkBox->isChecked()){
        // MAC Source, remove whitespace
        QString macsrc = ui->txt_mac_source->text();
        macsrc = macsrc.simplified();
        macsrc.replace( " ", "" );
        // Check if mac source address is in right size
        if(macsrc.size() < 17){
            QMessageBox messageBox;
            messageBox.critical(0,"Invalid MAC source","MAC source address is not in right format.");
            messageBox.setFixedSize(500,200);
            return false;
        }

        // Get the variable data
        rugeVariable macSrcData = vdata.getData("MAC_SRC");
        macSrcData.value = macsrc;

        createdSession_->addVariable(macSrcData);
    }
    else{
        // Remove the MAC source from session. It might exist
        // if editing existing session.
        createdSession_->removeVariable("MAC_SRC");
    }


    // IP destination address and remove whitespace
    QString ipdest = ui->txt_ip_dest->text();
    ipdest = ipdest.simplified();
    ipdest.replace( " ", "" );

    QHostAddress address(ipdest);
    // Check if address is in IPv4 format.
    if (QAbstractSocket::IPv4Protocol != address.protocol())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid IP destination","IP destination address is not in IPv4 format.");
        messageBox.setFixedSize(500,200);
        return false;
    }
    // Get the variable data
    rugeVariable ipDestData = vdata.getData("IP_DST");
    ipDestData.value = ipdest;

    // Save ip destination max if combo box is in "range".
    // IP destination max and remove whitespace
    if(ui->ipDestComboBox->currentIndex() == 1){
        QString ipdestmax = ui->txt_ipdstmax->text();
        ipdestmax = ipdestmax.simplified();
        ipdestmax.replace( " ", "" );

        QHostAddress address(ui->txt_ip_dest->text());
        // Check if address is in IPv4 format.
        if (QAbstractSocket::IPv4Protocol != address.protocol())
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Invalid IP destination","IP destination address is not in IPv4 format.");
            messageBox.setFixedSize(500,200);
            return false;
        }
        // Add data to increment to work.
        ipDestData.increment = 1;
        ipDestData.min = ipDestData.value;
        ipDestData.max = ipdestmax;
    }


    createdSession_->addVariable(ipDestData);


    // Overwrite ip source
    if(ui->overrideipsrc->isChecked()){
        // IP source address and remove whitespace
        QString ipsrc = ui->txt_ip_src->text();
        ipsrc = ipsrc.simplified();
        ipsrc.replace( " ", "" );

        QHostAddress address(ipsrc);
        // Check if address is in IPv4 format.
        if (QAbstractSocket::IPv4Protocol != address.protocol())
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Invalid IP source","IP source address is not in IPv4 format.");
            messageBox.setFixedSize(500,200);
            return false;
        }
        // Get the variable data
        rugeVariable ipSrcData = vdata.getData("IP_SRC");
        ipSrcData.value = ipsrc;

        // Save ip source max if combo box is in "range".
        // IP source max and remove whitespace
        if(ui->ipSourceComboBox->currentIndex() == 1){
            QString ipsrcmax = ui->txt_ipsrcmax->text();
            ipsrcmax = ipsrcmax.simplified();
            ipsrcmax.replace( " ", "" );

            QHostAddress address(ipsrcmax);
            // Check if address is in IPv4 format.
            if (QAbstractSocket::IPv4Protocol != address.protocol())
            {
                QMessageBox messageBox;
                messageBox.critical(0,"Invalid IP destination","IP destination address is not in IPv4 format.");
                messageBox.setFixedSize(500,200);
                return false;
            }
            // Add data to increment to work.
            ipSrcData.increment = 1;
            ipSrcData.min = ipSrcData.value;
            ipSrcData.max = ipsrcmax;
        }


        createdSession_->addVariable(ipSrcData);
    }

    // Overwrite source IP is not on.
    else{
        // Remove the IP source from session. It might exist
        // if editing existing session.
        createdSession_->removeVariable("IP_SRC");


    }

    // Dest. port
    if(ui->txt_udp_dest_port->text() != ""){
        QString dstPort = ui->txt_udp_dest_port->text();
        dstPort.remove( QRegExp("^[0]*") );
        rugeVariable UDPdstPORT = vdata.getData("UDP_DST_PORT");
        UDPdstPORT.value = dstPort;
        createdSession_->addVariable(UDPdstPORT);
    }

    // Source port
    if(ui->sourceport_checkBox->isChecked() && ui->txt_udp_src_port->text() != ""){
        QString srcPort = ui->txt_udp_src_port->text();
        srcPort.remove( QRegExp("^[0]*") );
        rugeVariable UDPsrcPORT = vdata.getData("UDP_SRC_PORT");
        UDPsrcPORT.value = srcPort;
        createdSession_->addVariable(UDPsrcPORT);
    }
    else{
        // Remove source port.
        createdSession_->removeVariable("UDP_SRC_PORT");
    }

    // Payload
    if(ui->payload_checkBox->isChecked()){
        createdSession_->setPayload(ui->payloadEdit->text());
    }
    else{
        createdSession_->setPayload("");
    }
    return true;
}

// Key pressed. Escape = close. Enter = set focus to confirm button
// or press confirm button if it already has focus
void createsession::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Escape){
        this->close();
    }
    if(event->key() == Qt::Key_Return){
        if(ui->confirm_Button->hasFocus()){
            on_confirm_Button_clicked();
        }
        else {
         ui->confirm_Button->setFocus();
        }
    }
}


