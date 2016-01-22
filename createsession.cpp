#include "createsession.h"
#include "ui_createsession.h"
#include "session.h"
#include "variabledata.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QHostAddress>

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
    this->setWindowFlags(flags | Qt::Window);
    this->setWindowModality(Qt::WindowModal);
    ui->setupUi(this);

    // Hide most information at the beginning
    ui->groupBox_2->setVisible(false);
    ui->groupBox_5->setVisible(false);
    ui->groupBox_3->setVisible(false);
    ui->groupBox_6->setVisible(false);

    ui->payloadEdit->setEnabled(false);

    ui->txt_udp_src_port->setValidator(new QIntValidator(0, 65535, this) );
    ui->txt_udp_dest_port->setValidator(new QIntValidator(0, 65535, this) );

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

    if(editedSession->getDstIP().max != ""){
        ui->ipDestComboBox->setCurrentIndex(1);
        ui->txt_ipdstmax->setEnabled(true);
        ui->label_ipdstinc->setEnabled(true);
        ui->txt_ipdstmax->setText(editedSession->getDstIP().max);
    }

    if(editSession_->getSrcIP().value != ""){
        ui->overrideipsrc->setChecked(true);
        ui->txt_ip_src->setText(editSession_->getSrcIP().value);
        if(editedSession->getSrcIP().max != ""){
            ui->ipSourceComboBox->setCurrentIndex(1);
            ui->txt_ipsrcmax->setEnabled(true);
            ui->label_ipsrcinc->setEnabled(true);
            ui->txt_ipsrcmax->setText(editedSession->getSrcIP().max);
        }
    }
    // Ports
    ui->txt_udp_dest_port->setText(editedSession->getVariable("UDP_DST_PORT").value);
    if(editedSession->getVariable("UDP_SRC_PORT").value != ""){
        ui->sourceport_checkBox->setChecked(true);
        ui->txt_udp_src_port->setText(editedSession->getVariable("UDP_SRC_PORT").value);
    }
    // Payload
    if(editSession_->getPayload() != ""){
        ui->payload_checkBox->setChecked(true);
        ui->payloadEdit->setText(editSession_->getPayload());
    }
}

void createsession::setEditMode(){
    editMode = true;
}

void createsession::on_rb_ip4_toggled(bool checked)
{
    if(checked){
      ui->groupBox_2->setVisible(true);
      ui->groupBox_5->setVisible(false);
    }
}

void createsession::on_lb_ip6_toggled(bool checked)
{
    if(checked){
      ui->groupBox_2->setVisible(false);
      ui->groupBox_5->setVisible(true);
    }
}

void createsession::on_rb_tcp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_3->setVisible(true);
        ui->groupBox_6->setVisible(false);
    }
}

void createsession::on_rb_udp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_3->setVisible(false);
        ui->groupBox_6->setVisible(true);
    }
}

void createsession::on_rb_icmp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_3->setVisible(false);
        ui->groupBox_6->setVisible(false);
    }
}


void createsession::on_rb_sctp_toggled(bool checked)
{
    if(checked){
        ui->groupBox_3->setVisible(false);
        ui->groupBox_6->setVisible(false);
    }
}

void createsession::on_mac_checkBox_toggled(bool checked)
{
    ui->txt_mac_source->setEnabled(checked);
}

void createsession::on_overrideipsrc_toggled(bool checked)
{
    ui->txt_ip_src->setEnabled(checked);
    ui->label_ipsrc->setEnabled(checked);
    ui->ipSourceComboBox->setEnabled(checked);
}



void createsession::on_checkBox_toggled(bool checked)
{
    ui->txt_urgent->setEnabled(checked);
    ui->label_urgent->setEnabled(checked);
}

void createsession::on_sourceport_checkBox_toggled(bool checked)
{
    ui->txt_udp_src_port->setEnabled(checked);
    ui->label_src_port_udp->setEnabled(checked);
}

void createsession::on_checkBox_seq_tcp_toggled(bool checked)
{
    ui->label_seq_tcp->setEnabled(checked);
    ui->txt_seq_tcp->setEnabled(checked);
}

void createsession::on_checkBox_ack_tcp_toggled(bool checked)
{
   ui->txt_tcp_ack_no->setEnabled(checked);
   ui->label_tcp_ack->setEnabled(checked);

}

void createsession::on_checkBox_src_porttcp_toggled(bool checked)
{
    ui->label_tcp_src->setEnabled(checked);

    ui->txt_tcp_src_port->setEnabled(checked);
}

void createsession::on_cancel_button_clicked()
{
    this->close();
}

void createsession::on_payload_checkBox_toggled(bool checked)
{
    ui->payloadEdit->setEnabled(checked);

}

// Add new session to the trafficwindow table or just edit
// the existing session depending on the value of bool editMode.
// Close the window when done.
void createsession::on_confirm_Button_clicked()
{
    if(makeSession()){
      this->close();
    }
    // Session was not created. Delete it if
    // not in edit mode.
    else if(editSession_ == 0 && !editMode){
        delete createdSession_;
    }
}

// When range is selected in ip source, enable increment editing.
void createsession::on_ipSourceComboBox_activated(int index)
{
    if(index != 0){

        ui->txt_ipsrcmax->setEnabled(true);
        ui->label_ipsrcinc->setEnabled(true);
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
    if(index != 0){

        ui->txt_ipdstmax->setEnabled(true);
        ui->label_ipdstinc->setEnabled(true);
        ui->txt_ipdstmax->setText(ui->txt_ip_dest->text());
    }
    else{
        ui->txt_ipdstmax->setEnabled(false);
        ui->label_ipdstinc->setEnabled(false);
    }
}

// Create a session from the given data
// if possible. Return value false if failed.
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

        createdSession_->removeVariable("IP_SRC");


    }



    // Dest. port
    if(ui->txt_udp_dest_port->text() != ""){
        rugeVariable UDPdstPORT = vdata.getData("UDP_DST_PORT");
        UDPdstPORT.value = ui->txt_udp_dest_port->text();
        createdSession_->addVariable(UDPdstPORT);
    }

    // Source port
    if(ui->sourceport_checkBox->isChecked() && ui->txt_udp_src_port->text() != ""){
        rugeVariable UDPsrcPORT = vdata.getData("UDP_SRC_PORT");
        UDPsrcPORT.value = ui->txt_udp_src_port->text();
        createdSession_->addVariable(UDPsrcPORT);
    }
    else{
        createdSession_->removeVariable("UDP_SRC_PORT");
    }

    // payload
    if(ui->payload_checkBox->isChecked()){
        createdSession_->setPayload(ui->payloadEdit->text());
    }
    else{
        createdSession_->setPayload("");
    }


    if(editMode){
       parentPointer->displaySessions();
    }
    else{
       parentPointer->addSession(createdSession_);
    }
    return true;
}
