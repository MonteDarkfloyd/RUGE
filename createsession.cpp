#include "createsession.h"
#include "ui_createsession.h"
#include "mainwindow.h"
#include "session.h"

QVBoxLayout* v_ip4_layout;

createsession::createsession(QWidget *parent, TrafficWindow* Tparent) :
    QWidget(parent),
    ui(new Ui::createsession)
{
    parentPointer = Tparent;
    Qt::WindowFlags flags = windowFlags();
    this->setWindowFlags(flags | Qt::Window);
    ui->setupUi(this);




    /* v_ip4_layout=new QVBoxLayout(this);

    QVBoxLayout* v_ip6_layout=new QVBoxLayout(this);
    QLabel* lbl_empty=new QLabel(this);
    lbl_empty->setText("");
    v_ip6_layout->addWidget(lbl_empty);


    QHBoxLayout* h11_layout=new QHBoxLayout(this);


    QLabel* lbl_IPsource=new QLabel(this);
    lbl_IPsource->setText("IP Source: ");
    h11_layout->addWidget(lbl_IPsource);


    QLineEdit* txtbx_IPsource=new QLineEdit(this);
    h11_layout->addWidget(txtbx_IPsource);

    QLabel* lbl_incount=new QLabel(this);
    lbl_incount->setText("Increment Count: ");
    lbl_incount->setStyleSheet("QLabel{margin-left:20px}");
    h11_layout->addWidget(lbl_incount);


    QLineEdit* txtbx_incount=new QLineEdit(this);
    h11_layout->addWidget(txtbx_incount);

    v_ip4_layout->addItem(h11_layout);


    QHBoxLayout* h12_layout=new QHBoxLayout(this);


    QLabel* lbl_IPdestination=new QLabel(this);
    lbl_IPdestination->setText("IP Destination: ");
    h12_layout->addWidget(lbl_IPdestination);


    QLineEdit* txtbx_IPdestination=new QLineEdit(this);
    h12_layout->addWidget(txtbx_IPdestination);

    QLabel* lbl_incount_dest=new QLabel(this);
    lbl_incount_dest->setText("Increment Count: ");
    lbl_incount_dest->setStyleSheet("QLabel{margin-left:20px}");
    h12_layout->addWidget(lbl_incount_dest);


    QLineEdit* txtbx_incount_dest=new QLineEdit(this);
    h12_layout->addWidget(txtbx_incount_dest);

    v_ip4_layout->addItem(h12_layout);

    QHBoxLayout* h13_layout=new QHBoxLayout(this);


    QLabel* lbl_Offset=new QLabel(this);
    lbl_Offset->setText("Offset: ");
    h13_layout->addWidget(lbl_Offset);


    QLineEdit* txtbx_offset=new QLineEdit(this);
    h13_layout->addWidget(txtbx_offset);

    QLabel* lbl_TTL=new QLabel(this);
    lbl_TTL->setText("TTL: ");
    lbl_TTL->setStyleSheet("QLabel{margin-left:20px}");
    h13_layout->addWidget(lbl_TTL);


    QLineEdit* txtbx_TTL=new QLineEdit(this);
    h13_layout->addWidget(txtbx_TTL);



    v_ip4_layout->addItem(h13_layout);

*/



    //ui->groupBox_2->setLayout(v_ip4_layout);


    //ui->groupBox_2->setLayout(v_ip4_layout);




    //ui->rb_ip4->setChecked(true);
    ui->groupBox_2->setVisible(false);
    ui->groupBox_5->setVisible(false);
    ui->groupBox_3->setVisible(false);
    ui->groupBox_6->setVisible(false);
    //ui->rb_tcp->setChecked(true);


}

createsession::~createsession()
{
    delete ui;
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
    ui->txt_ipincrement->setEnabled(checked);
    ui->txt_ip_src->setEnabled(checked);
    ui->label_ipsrc->setEnabled(checked);
    ui->label_ipsrcincrement->setEnabled(checked);
}

void createsession::on_ttl_checkBox_toggled(bool checked)
{
    ui->label_ttl->setEnabled(checked);
    ui->txt_ttl->setEnabled(checked);
}

void createsession::on_checkBox_9_toggled(bool checked)
{
    ui->label_offset->setEnabled(checked);
    ui->txt_offset->setEnabled(checked);
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

void createsession::on_length_checkBox_toggled(bool checked)
{
    ui->label_udp__len->setEnabled(checked);
    ui->txt_udp_len->setEnabled(checked);
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
// Mysterious bug for now...
void createsession::on_rb_sctp_clicked()
{

}

void createsession::on_payload_checkBox_toggled(bool checked)
{
    ui->groupBox_4->setEnabled(checked);

}

void createsession::on_confirm_Button_clicked()
{
    Session* createdSession = new Session();
    createdSession->sessName = ui->name_edit->text();
    createdSession->ipVersion = "IPv4";
    createdSession->protocol = "UDP";
    createdSession->dstIP =  ui->txt_ip_dest->text();
    createdSession->dstMAC = ui->txt_mac_dest->text();
    if(ui->mac_checkBox->isChecked()){
        createdSession->srcMAC = ui->txt_mac_source->text();
    }
    if(ui->overrideipsrc->isChecked()){
        createdSession->srcIP = ui->txt_ip_src->text();
    }
    if(ui->ttl_checkBox->isChecked()){
        createdSession->ttl = ui->txt_ttl->text();
    }
    if(ui->txt_udp_dest_port->text() != ""){
        createdSession->udp->dstPort = ui->txt_udp_dest_port->text();
    }
    if(ui->sourceport_checkBox->isChecked()){
        createdSession->udp->srcPort = ui->txt_udp_src_port->text();
    }
    if(ui->length_checkBox->isChecked()){
        createdSession->udp->length = ui->txt_udp_len->text();
    }
    if(ui->payload_checkBox->isChecked()){
        createdSession->payload = ui->payloadEdit->text();
    }

    this->close();
    parentPointer->addSession(createdSession);

}


