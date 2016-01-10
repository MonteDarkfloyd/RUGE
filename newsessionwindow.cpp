#include "newsessionwindow.h"
#include "ui_newsessionwindow.h"
#include <QCheckBox>


newsessionwindow::newsessionwindow(TrafficWindow *parent,Session* _session) :
    ui(new Ui::newsessionwindow)
{
    this->parentPointer = parent;
    this->tempPointer = 0;
    this->newSession = _session;
    ui->setupUi(this);

}


newsessionwindow::newsessionwindow(predef *parent, Session *_session) :
    ui(new Ui::newsessionwindow)
{
    this->parentPointer = 0;
    this->tempPointer = parent;
    this->newSession = _session;
    ui->setupUi(this);

}


newsessionwindow::~newsessionwindow()
{
    delete ui;
}


//IPv4 button
void newsessionwindow::on_radioButton_2_toggled(bool checked)
{

    QGridLayout *ip4grid = new QGridLayout();

    QLabel *IPsrc = new QLabel("IP Source:", ui->ipbox);
    QLineEdit *IPsrcinput = new QLineEdit(ui->ipbox);
    IPsrcinput->setFixedWidth(200);
    IPsrcinput->setFixedHeight(20);


    QLabel *IPdst = new QLabel("IP Destination :",ui->ipbox);
    QLineEdit *IPdstinput = new QLineEdit(ui->ipbox);
    IPdstinput->setFixedWidth(200);
    IPdstinput->setFixedHeight(20);

    ip4grid->addWidget(IPsrc,0,0,1,1);
    ip4grid->addWidget(IPsrcinput,0,1,1,1);

    ip4grid->addWidget(IPdst,1,0,1,1);
    ip4grid->addWidget(IPdstinput,1,1,1,1);



    if(newSession != 0){
        IPsrcinput->setText(this->newSession->srcIP);
        IPdstinput->setText(this->newSession->dstIP);
    }

    ui->ipbox->setTitle("Internet Protocol Version 4");
    ui->ipbox->setLayout(ip4grid);

}

void newsessionwindow::on_radioButton_8_toggled(bool checked)  //payload
{

    ui->payloadinput->setEnabled(checked);
    if(this->newSession != 0){
       if(this->newSession->payload != "")
       ui->payloadinputBox->setItemText(0,this->newSession->payload);
    }
}

/*void newsessionwindow::on_radioButton_11_toggled(bool checked)  //payloadbutton
{

    ui->payloadinput->setEnabled(checked);
}*/

void newsessionwindow::RemoveLayout(QWidget* widget)
{
    QLayout * layout = widget->layout();
    if(layout != 0 ){
    QLayoutItem * item;
    QLayout * sublayout;
    QWidget * widget2;
    while ((item = layout->takeAt(0))) {
        if ((sublayout = item->layout()) != 0) {/* do the same for sublayout*/}
        else if ((widget2 = item->widget()) != 0) {widget2->hide(); delete widget2;}
        else {delete item;}
    }


    delete layout;
    }
}

void newsessionwindow::on_radioButton_4_toggled(bool checked) //TCP button
{

    this->RemoveLayout(ui->protocolBox);


    QGridLayout *tcpgrid = new QGridLayout();

    QLabel *PORTsrc = new QLabel("Port Source:");
    QLineEdit *PORTsrcinput = new QLineEdit();

    PORTsrcinput->setFixedWidth(185);
    PORTsrcinput->setFixedHeight(20);


    QLabel *PORTdst = new QLabel("Port Destination :");
    QLineEdit *PORTdstinput = new QLineEdit();
    PORTdstinput->setFixedWidth(185);
    PORTdstinput->setFixedHeight(20);

    tcpgrid->addWidget(PORTsrc,0,0,1,1);
    tcpgrid->addWidget(PORTsrcinput,0,1,1,1);

    tcpgrid->addWidget(PORTdst,1,0,1,1);
    tcpgrid->addWidget(PORTdstinput,1,1,1,1);

    //QCheckBox *urg = new QCheckBox("Urgent",this);
    //tcpgrid->addWidget(urg,0,2,1,1);

    QLabel *seqNo = new QLabel("Sequence No :");
    QLineEdit *seqNoinput = new QLineEdit();


    tcpgrid->addWidget(seqNo,2,0,1,1);
    tcpgrid->addWidget(seqNoinput,2,1,1,1);

    QLabel *window = new QLabel("Window :");
    QLineEdit *windowinput = new QLineEdit();

    tcpgrid->addWidget(window,2,2,1,1);
    tcpgrid->addWidget(windowinput,2,3,1,1);


    QCheckBox *urg = new QCheckBox("URG");
    tcpgrid->addWidget(urg,2,4,1,1);

    QCheckBox *ack = new QCheckBox("ACK");
    tcpgrid->addWidget(ack,2,5,1,1);

    QCheckBox *syn = new QCheckBox("SYN");
    tcpgrid->addWidget(syn,2,6,1,1);


    QLabel *ackNo = new QLabel("Ack No :");
    QLineEdit *ackNoinput = new QLineEdit();

    tcpgrid->addWidget(ackNo,3,0,1,1);
    tcpgrid->addWidget(ackNoinput,3,1,1,1);

    QCheckBox *rst = new QCheckBox("RST");
    tcpgrid->addWidget(rst,3,4,1,1);

    QCheckBox *psh = new QCheckBox("PSH");
    tcpgrid->addWidget(psh,3,5,1,1);

    QCheckBox *fin = new QCheckBox("FIN");
    tcpgrid->addWidget(fin,3,6,1,1);



    if(this->newSession != 0){
        if(this->newSession->tcp != 0){
        PORTsrcinput->setText(this->newSession->tcp->srcPort);
        PORTdstinput->setText(this->newSession->tcp->dstPort);
        seqNoinput->setText(this->newSession->tcp->seqNo);
        windowinput->setText(this->newSession->tcp->window);
        urg->setChecked(this->newSession->tcp->urg);
        ack->setChecked(this->newSession->tcp->ack);
        syn->setChecked(this->newSession->tcp->syn);
        ackNoinput->setText(this->newSession->tcp->ackNo);
        rst->setChecked(this->newSession->tcp->rst);
        psh->setChecked(this->newSession->tcp->psh);
        fin->setChecked(this->newSession->tcp->fin);
        }
    }

    ui->protocolBox->setTitle("Transmission Control Protocol");
    ui->protocolBox->setLayout(tcpgrid);

}

void newsessionwindow::on_radioButton_7_toggled(bool checked) //UDP button
{
    this->RemoveLayout(ui->protocolBox);



    QGridLayout *udpgrid = new QGridLayout();

    QLabel *PORTsrc = new QLabel("Port Source:");
    QLineEdit *PORTsrcinput = new QLineEdit();
    PORTsrcinput->setFixedWidth(200);
    PORTsrcinput->setFixedHeight(20);


    QLabel *PORTdst = new QLabel("Port Destination :");
    QLineEdit *PORTdstinput = new QLineEdit();
    PORTdstinput->setFixedWidth(200);
    PORTdstinput->setFixedHeight(20);

    udpgrid->addWidget(PORTsrc,0,0,1,1);
    udpgrid->addWidget(PORTsrcinput,0,1,1,1);

    udpgrid->addWidget(PORTdst,1,0,1,1);
    udpgrid->addWidget(PORTdstinput,1,1,1,1);


    QLabel *length = new QLabel("Length:");
    QLineEdit *lengthinput = new QLineEdit();
    lengthinput->setFixedWidth(200);
    lengthinput->setFixedHeight(20);

    udpgrid->addWidget(length,2,0,1,1);
    udpgrid->addWidget(lengthinput,2,1,1,1);

    QLabel *checksum = new QLabel("Checksum :");
    QLineEdit *checksuminput = new QLineEdit();

    checksuminput->setFixedWidth(200);
    checksuminput->setFixedHeight(20);

    udpgrid->addWidget(checksum,3,0,1,1);
    udpgrid->addWidget(checksuminput,3,1,1,1);

    if( this->newSession != 0 ){
        if(this->newSession->udp != 0){
        PORTsrcinput->setText(this->newSession->udp->srcPort);
        PORTdstinput->setText(this->newSession->udp->dstPort);
        lengthinput->setText(this->newSession->udp->length);
  //      checksuminput->setText(this->newSession->udp->checksum);
        }
    }


    ui->protocolBox->setTitle("User Datagram Protocol");
    ui->protocolBox->setLayout(udpgrid);

}


void newsessionwindow::createSession(){

    QWidget *widget1 = ui->ipbox->layout()->itemAt(1)->widget();
    QWidget *widget2 = ui->ipbox->layout()->itemAt(3)->widget();
    if(ui->radioButton_4->isChecked()){    //TCP

        TCPvalues* tcpvalues = new TCPvalues(
          qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(1)->widget())->text(),
          qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(3)->widget())->text(),
          qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(5)->widget())->text(),
          qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(7)->widget())->text(),
          qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(12)->widget())->text(),
          qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(8)->widget())->isChecked(),
          qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(9)->widget())->isChecked(),
          qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(10)->widget())->isChecked(),
          qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(13)->widget())->isChecked(),
          qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(14)->widget())->isChecked(),
          qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(15)->widget())->isChecked());


        this->newSession = new Session( ui->nameInput->text(),
                                   qobject_cast<QLineEdit*>(widget1)->text(),
                                   qobject_cast<QLineEdit*>(widget2)->text(),
                                   ui->MACsrc->text(),
                                   ui->MACdst->text(),
                                   ui->payloadinputBox->currentText(),
                                   QString("TCP"),
                                   QString("IPv4"),
                                       tcpvalues,
                                       0
                                        );

    }
    if( ui->radioButton_7->isChecked() ){  //UDP

        UDPvalues* udpvalues = new UDPvalues(
                    qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(1)->widget())->text(),
                    qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(3)->widget())->text(),
                    qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(5)->widget())->text()
                                              );


        this->newSession = new Session(ui->nameInput->text(),
                                       qobject_cast<QLineEdit*>(widget1)->text(),
                                       qobject_cast<QLineEdit*>(widget2)->text(),
                                       ui->MACsrc->text(),
                                       ui->MACdst->text(),
                                       ui->payloadinputBox->currentText(),
                                       QString("UDP"),
                                       QString("IPv4"),
                                       0,
                                       udpvalues
                                       );
    }




    if(this->parentPointer != 0)
    this->parentPointer->addSession(this->newSession);
    //if(this->tempPointer != 0)
    //this->tempPointer->add(this->newSession);

}


void newsessionwindow::on_pushButton_clicked()
{
    this->destroy(true,true);
}

void newsessionwindow::on_pushButton_2_clicked()
{
    this->createSession();
}

void newsessionwindow::displaySession(){
    if(this->newSession != 0){
        ui->nameInput->setText(newSession->sessName);

        ui->MACsrc->setText(newSession->srcMAC);
        ui->MACdst->setText(newSession->dstMAC);

        /*QWidget *widget1 = ui->ipbox->layout()->itemAt(1)->widget();
        QWidget *widget2 = ui->ipbox->layout()->itemAt(3)->widget();
        qobject_cast<QLineEdit*>(widget1)->setText(newSession->srcIP);
        qobject_cast<QLineEdit*>(widget2)->setText(newSession->dstIP);
*/


        if(this->newSession->ipVersion == "IPv4")
        ui->radioButton_2->setChecked(true);

        if(this->newSession->tcp != 0)
        ui->radioButton_4->setChecked(true);

        if(this->newSession->udp != 0)
        ui->radioButton_7->setChecked(true);

        if(this->newSession->payload != "")
        ui->radioButton_8->setChecked(true);






    /*    qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(1)->widget())->setText(this->newSession->tcp->srcPort);
        qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(3)->widget())->setText(this->newSession->tcp->dstPort);
        qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(5)->widget())->setText(this->newSession->tcp->seqNo);
        qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(7)->widget())->setText(this->newSession->tcp->window);
        qobject_cast<QLineEdit*>(ui->protocolBox->layout()->itemAt(12)->widget())->setText(this->newSession->tcp->ackNo);
        qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(8))->setChecked(this->newSession->tcp->urg);
        qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(9))->setChecked(this->newSession->tcp->ack);
        qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(10))->setChecked(this->newSession->tcp->syn);
        qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(13))->setChecked(this->newSession->tcp->rst);
        qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(14))->setChecked(this->newSession->tcp->psh);
        qobject_cast<QCheckBox*>(ui->protocolBox->layout()->itemAt(15))->setChecked(this->newSession->tcp->fin);
    */
    }


}
