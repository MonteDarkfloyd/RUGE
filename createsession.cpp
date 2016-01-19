#include "createsession.h"
#include "ui_createsession.h"
#include "session.h"
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
    editSession = 0;
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

void createsession::setSession(Session* editedSession){
    editSession = editedSession;

    // Set items to UI
    ui->name_edit->setText(editSession->sessName);
    // Only UDP supported for now.
    ui->rb_ip4->setChecked(true);
    ui->rb_udp->setChecked(true);
    ui->txt_mac_dest->setText(editSession->dstMAC);

    // If MAC source is set, enable checkbox and set text.
    if(editSession->srcMAC != ""){
        ui->mac_checkBox->setChecked(true);
        ui->txt_mac_source->setText(editSession->srcMAC);
    }
    // IP addresses
    ui->txt_ip_dest->setText(editedSession->dstIP);

    if(editSession->srcIP != ""){
        ui->overrideipsrc->setChecked(true);
        ui->txt_ip_src->setText(editSession->srcIP);
    }
    // Ports
    ui->txt_udp_dest_port->setText(editedSession->udp->dstPort);
    if(editSession->udp->srcPort != ""){
        ui->sourceport_checkBox->setChecked(true);
        ui->txt_udp_src_port->setText(editSession->udp->dstPort);
    }
    // Payload
    if(editSession->payload != ""){
        ui->payload_checkBox->setChecked(true);
        ui->payloadEdit->setText(editSession->payload);
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
    Session* createdSession = 0;
    // Check if editing
    if(editMode){
        createdSession = editSession;
    }
    else{
        createdSession = new Session();
    }

    // Remove whitespace from the name.
    QString name = ui->name_edit->text().trimmed();

    // Check that name is not empty
    if(name.isEmpty()){
        QMessageBox messageBox;
        messageBox.critical(0,"No name inserted.","Please insert session name.");
        messageBox.setFixedSize(500,200);
        return;
    }


    // Check if name is already in use and we are not in edit mode.
    if(!parentPointer->checkName(name) && !editMode){
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid name","Name is already in use.");
        messageBox.setFixedSize(500,200);
        return;
    }
    createdSession->sessName = name;

    createdSession->ipVersion = "IPv4";
    createdSession->protocol = "UDP";

    // MAC destination, remove whitespace
    QString macdest = ui->txt_mac_dest->text();
    macdest = macdest.simplified();
    macdest.replace( " ", "" );
    // Check if mac dest. address is in right size
    if(macdest.size() < 17){
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid MAC destination","MAC destination address is not in right format.");
        messageBox.setFixedSize(500,200);
        return;
    }

    createdSession->dstMAC = macdest;

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
            return;
        }

        createdSession->srcMAC = macsrc;
    }
    else{
        createdSession->srcMAC = "";
    }



    // IP destination address and remove whitespace
    QString ipdest = ui->txt_ip_dest->text();
    ipdest = ipdest.simplified();
    ipdest.replace( " ", "" );

    QHostAddress address(ui->txt_ip_dest->text());
    // Check if address is in IPv4 format.
    if (QAbstractSocket::IPv4Protocol != address.protocol())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid IP destination","IP destination address is not in IPv4 format.");
        messageBox.setFixedSize(500,200);
        return;
    }
    createdSession->dstIP =  ipdest;

    // Overwrite ip source
    if(ui->overrideipsrc->isChecked()){
        // IP source address and remove whitespace
        QString ipsrc = ui->txt_ip_dest->text();
        ipsrc = ipsrc.simplified();
        ipsrc.replace( " ", "" );

        QHostAddress address(ui->txt_ip_src->text());
        // Check if address is in IPv4 format.
        if (QAbstractSocket::IPv4Protocol != address.protocol())
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Invalid IP source","IP source address is not in IPv4 format.");
            messageBox.setFixedSize(500,200);
            return;
        }
        createdSession->srcIP = ipsrc;
    }
    else{
        createdSession->srcIP = "";
    }


    // Dest. port
    if(ui->txt_udp_dest_port->text() != ""){
        createdSession->udp->dstPort = ui->txt_udp_dest_port->text();
    }

    // Source port
    if(ui->sourceport_checkBox->isChecked()){
        createdSession->udp->srcPort = ui->txt_udp_src_port->text();
    }
    else{
        createdSession->udp->srcPort = "";
    }

    // payload
    if(ui->payload_checkBox->isChecked()){
        createdSession->payload = ui->payloadEdit->text();
    }
    else{
        createdSession->payload = "";
    }


    if(editMode){
       parentPointer->displaySessions();
    }
    else{
       parentPointer->addSession(createdSession);
    }

    this->close();
}

// When range is selected in ip source, enable increment editing.
void createsession::on_ipSourceComboBox_activated(int index)
{
    if(index != 0){

        ui->txt_ipsrcinc->setEnabled(true);
        ui->label_ipsrcinc->setEnabled(true);
    }
    else{
        ui->txt_ipsrcinc->setEnabled(false);
        ui->label_ipsrcinc->setEnabled(false);
    }
}

// When range is selected in ip destination, enable increment editing.
void createsession::on_ipDestComboBox_activated(int index)
{
    if(index != 0){

        ui->txt_ipdstinc->setEnabled(true);
        ui->label_ipdstinc->setEnabled(true);
    }
    else{
        ui->txt_ipdstinc->setEnabled(false);
        ui->label_ipdstinc->setEnabled(false);
    }
}
