#include <QtWidgets>
#include "sessionwindow.h"
#include "session.h"
//#include "ui_sessionwindow.h"



void Window::clearGroup(){
    QObjectList temp = this->children();
    QGroupBox *btemp = (QGroupBox*)temp.at(3);

    QListIterator<QObject *> i(btemp->children());
    while (i.hasNext())
    {
        QLineEdit* b = qobject_cast<QLineEdit*>( i.next() );
        if (b > 0 ) {
         b->clear();
        }
    }
    QComboBox* tempCombo = qobject_cast<QComboBox*>(btemp->children().at(10));
     tempCombo->setItemText(0,"");



}


void Window::displaySession(Session* session){
    QObjectList temp = this->children();
    QGroupBox *atemp = (QGroupBox*)temp.at(2);
    atemp->setVisible(1);
    QGroupBox *btemp = (QGroupBox*)temp.at(3);
    btemp->setVisible(1);

    QLineEdit* tempLine = qobject_cast<QLineEdit*>(atemp->children().at(2));
    tempLine->setText(session->sessName);


    if(session->ipVersion =="IPv4")
        qobject_cast<QRadioButton*>(atemp->children().at(4))->setChecked(true);

    if(session->ipVersion =="IPv6")
         qobject_cast<QRadioButton*>(atemp->children().at(5))->setChecked(true);

    if(session->protocol =="TCP")
        qobject_cast<QRadioButton*>(atemp->children().at(6))->setChecked(true);

    if(session->protocol =="UDP")
        qobject_cast<QRadioButton*>(atemp->children().at(7))->setChecked(true);

    if(session->protocol =="ICMP")
        qobject_cast<QRadioButton*>(atemp->children().at(8))->setChecked(true);

    if(session->protocol =="STCP")
        qobject_cast<QRadioButton*>(atemp->children().at(9))->setChecked(true);


    QComboBox* tempCombo = qobject_cast<QComboBox*>(btemp->children().at(10));
     tempCombo->setItemText(0,session->payload);


     tempLine = qobject_cast<QLineEdit*>(btemp->children().at(2));
         tempLine->setText(session->srcIP);
     tempLine = qobject_cast<QLineEdit*>(btemp->children().at(4));
         tempLine->setText(session->srcMAC);
     tempLine = qobject_cast<QLineEdit*>(btemp->children().at(6));
         tempLine->setText(session->dstIP);
     tempLine = qobject_cast<QLineEdit*>(btemp->children().at(8));
         tempLine->setText(session->dstMAC);

QVBoxLayout* tempLayout = qobject_cast<QVBoxLayout*>(btemp->children().at(0));
QPushButton* saveButton = qobject_cast<QPushButton*>(btemp->children().at(12));

saveButton->setText("Save as new");
QPushButton* newButton = new QPushButton("Save changes");
connect( newButton, newButton->clicked, this, this->close);
connect( newButton, newButton->clicked, this, this->createSession);
connect( newButton, newButton->clicked, this, this->editSession);
//connect( newButton, newButton->clicked , parentPointer , parentPointer->displaySessions );
tempLayout->addWidget(newButton);


}

void Window::editSession(){
    parentPointer->editSession(this->newSession);

}

void Window::createSession(){
    QString _sessName;
    QString _srcIP;
    QString _dstIP;
    QString _srcMAC;
    QString _dstMAC;
    QString _payload;
    QString _protocol;
    QString _ipVersion;


    QObjectList temp = this->children();
    QGroupBox *atemp = (QGroupBox*)temp.at(2);
    QGroupBox *btemp = (QGroupBox*)temp.at(3);


    QLineEdit* tempLine = qobject_cast<QLineEdit*>(atemp->children().at(2));
    _sessName = tempLine->text();

    if(qobject_cast<QRadioButton*>(atemp->children().at(4))->isChecked())
        _ipVersion = "IPv4";

    if(qobject_cast<QRadioButton*>(atemp->children().at(5))->isChecked())
        _ipVersion = "IPv6";

    if(qobject_cast<QRadioButton*>(atemp->children().at(6))->isChecked())
        _protocol = "TCP";

    if(qobject_cast<QRadioButton*>(atemp->children().at(7))->isChecked())
        _protocol = "UDP";

    if(qobject_cast<QRadioButton*>(atemp->children().at(8))->isChecked())
        _protocol = "ICMP";

    if(qobject_cast<QRadioButton*>(atemp->children().at(9))->isChecked())
        _protocol = "STCP";


   QComboBox* tempCombo = qobject_cast<QComboBox*>(btemp->children().at(10));
    _payload = tempCombo->currentText();

    tempLine = qobject_cast<QLineEdit*>(btemp->children().at(2));
    _srcIP = tempLine->text();
    tempLine = qobject_cast<QLineEdit*>(btemp->children().at(4));
    _srcMAC = tempLine->text();
    tempLine = qobject_cast<QLineEdit*>(btemp->children().at(6));
    _dstIP = tempLine->text();
    tempLine = qobject_cast<QLineEdit*>(btemp->children().at(8));
    _dstMAC = tempLine->text();

    /*this->newSession =  new Session(_sessName,_srcIP,
                       _dstIP,
                       _srcMAC,
                       _dstMAC,
                       _payload,
                       _protocol,
                       _ipVersion);
*/


}


void Window::firstVisible(){
QObjectList temp = this->children();
QGroupBox *btemp = (QGroupBox*)temp.at(2);
btemp->setVisible(1);

}

void Window::secondVisible(){
QObjectList temp = this->children();

QGroupBox *atemp = (QGroupBox*)temp.at(2);
//QObjectList children = atemp->children();
int sum = 0;

QListIterator<QObject *> i(atemp->children());
while (i.hasNext())
{
    QRadioButton* b = qobject_cast<QRadioButton*>( i.next() );
    if (b > 0 && b->isChecked())
    {
     sum = sum + 1;
    }
}

if (sum == 3){
QGroupBox *btemp = (QGroupBox*)temp.at(3);
btemp->setVisible(1);
}

}

void Window::thirdVisible(){
QObjectList temp = this->children();
QGroupBox *btemp = (QGroupBox*)temp.at(4);
btemp->setVisible(0);

}

/*Window::Window(TrafficWindow *parent, Session* session, int a){
    this->parentPointer = parent;
    QHBoxLayout *hor1 = new QHBoxLayout(this);

    QVBoxLayout *ver1 = new QVBoxLayout(this);
    QVBoxLayout *ver2 = new QVBoxLayout(this);

    ver1->addWidget(createList());


    //QHBoxLayout *hor2 = new QHBoxLayout(this);



    QGroupBox *protocol = createProtocolSelection();
    protocol->setVisible(1);
   // protocol->setAccessibleName(QString("protocol"));
   // protocol->setAccessibleDescription(QString("protocol"));
    ver2->addWidget(protocol);
    QGroupBox *data = createDataInput();
    data->setVisible(1);
    data->setAccessibleName("data");
    ver2->addWidget(data);
    QGroupBox *stream = this->createStream();
    stream->setAccessibleName("stream");

    ver2->addWidget(stream);
    ver2->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));




    hor1->addLayout(ver1);
    hor1->addLayout(ver2);



    setLayout(hor1);

    setWindowTitle(tr("Session Editor"));
    resize(600, 550);


    this->displaySession(session);






}*/





Window::Window(TrafficWindow *parent)
{
    this->parentPointer = parent;
    QHBoxLayout *hor1 = new QHBoxLayout(this);

    QVBoxLayout *ver1 = new QVBoxLayout(this);
    QVBoxLayout *ver2 = new QVBoxLayout(this);

    ver1->addWidget(createList());


    //QHBoxLayout *hor2 = new QHBoxLayout(this);



    QGroupBox *protocol = createProtocolSelection();
   // protocol->setAccessibleName(QString("protocol"));
   // protocol->setAccessibleDescription(QString("protocol"));
    ver2->addWidget(protocol);
    QGroupBox *data = createDataInput();
    data->setAccessibleName("data");
    ver2->addWidget(data);
    QGroupBox *stream = this->createStream();
    stream->setAccessibleName("stream");

    ver2->addWidget(stream);
    ver2->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));




    hor1->addLayout(ver1);
    hor1->addLayout(ver2);



    setLayout(hor1);

    setWindowTitle(tr("Session Editor"));
    resize(600, 550);


    /*QGridLayout *grid = new QGridLayout;
    grid->addWidget(, 0, 1);
    grid->addWidget(,1,0);
    grid->addWidget(, 2, 1);
    grid->addWidget(, 1, 1);
    setLayout(grid);*/
}

QGroupBox *Window::createProtocolSelection()
{
    QVBoxLayout *newmain = new QVBoxLayout;
    QHBoxLayout *main = new QHBoxLayout;
    QHBoxLayout *hor2 = new QHBoxLayout;
    QLabel *nameLabel = new QLabel("Session Name :");
    QLineEdit *sessionName = new QLineEdit();
    hor2->addWidget(nameLabel);
    hor2->addWidget(sessionName);
    QVBoxLayout *eth = new QVBoxLayout;
    QVBoxLayout *ip = new QVBoxLayout;
    QVBoxLayout *protocols = new QVBoxLayout;

    QButtonGroup *group1 = new QButtonGroup;
    QButtonGroup *group2 = new QButtonGroup;
    QButtonGroup *group3 = new QButtonGroup;

    QRadioButton *radio1 = new QRadioButton(tr("Ethernet II"));
    eth->addWidget(radio1);
    radio1->setChecked(1);
    radio1->setEnabled(0);
    group1->addButton(radio1);

    QRadioButton *radio2 = new QRadioButton(tr("IPv4"));
    connect(radio2, radio2->toggled, this , &Window::secondVisible);

    QRadioButton *radio3 = new QRadioButton(tr("IPv6"));
    connect(radio3, radio3->toggled, this , &Window::secondVisible);

    ip->addWidget(radio2);
    ip->addWidget(radio3);
    group2->addButton(radio2);
    group2->addButton(radio3);



    QRadioButton *radio4 = new QRadioButton(tr("TCP"));
    connect(radio4, radio4->toggled, this , &Window::secondVisible);
    QRadioButton *radio5 = new QRadioButton(tr("UDP"));
    connect(radio5, radio5->toggled, this , &Window::secondVisible);
    QRadioButton *radio6 = new QRadioButton(tr("ICMP"));
    connect(radio6, radio6->toggled, this , &Window::secondVisible);
    QRadioButton *radio7 = new QRadioButton(tr("STCP"));
    connect(radio7, radio7->toggled, this , &Window::secondVisible);
    group3->addButton(radio4);
    group3->addButton(radio5);
    group3->addButton(radio6);
    group3->addButton(radio7);



    protocols->addWidget(radio4);
    protocols->addWidget(radio5);
    protocols->addWidget(radio6);
    protocols->addWidget(radio7);

    main->addLayout(eth);
    eth->addStretch();
    main->addLayout(ip);
    ip->addStretch();
    main->addLayout(protocols);
    protocols->addStretch();


    QGroupBox *protocolBox = new QGroupBox(tr("Session"));

    protocolBox->setVisible(0);
    protocolBox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    newmain->addLayout(hor2);
    newmain->addSpacerItem(new QSpacerItem(0,20));
    newmain->addLayout(main);

    protocolBox->setLayout(newmain);
    return protocolBox;


}


void Window::clickedNew(){
    QObjectList temp = this->children();
    QGroupBox *atemp = (QGroupBox*)temp.at(2);
    atemp->setVisible(1);
    QGroupBox *btemp = (QGroupBox*)temp.at(3);
    btemp->setVisible(0);

    this->clearGroup();
    qobject_cast<QRadioButton*>(atemp->children().at(4))->setChecked(false);
    qobject_cast<QRadioButton*>(atemp->children().at(5))->setChecked(false);
    qobject_cast<QRadioButton*>(atemp->children().at(6))->setChecked(false);
    qobject_cast<QRadioButton*>(atemp->children().at(7))->setChecked(false);
    qobject_cast<QRadioButton*>(atemp->children().at(8))->setChecked(false);
    qobject_cast<QRadioButton*>(atemp->children().at(9))->setChecked(false);




}

QGroupBox *Window::createList()
{

    QVBoxLayout *list = new QVBoxLayout;
    QButtonGroup *group = new QButtonGroup;


    //QListWidget *list = new QListWidget;
    QPushButton *newSession = new QPushButton("Create new session");
    newSession->setCheckable(1);
    list->addWidget(newSession);
    group->addButton(newSession);
    connect(newSession, newSession->clicked, this , this->clickedNew);
    list->addSpacerItem(new QSpacerItem(0,20,QSizePolicy::Fixed, QSizePolicy::Fixed));

    list->addWidget(new QLabel("Predefined Sessions :"),0,Qt::AlignHCenter);


    QPushButton *tcpFlood = new QPushButton("TCP Flood attack");
    tcpFlood->setCheckable(1);
    list->addWidget(tcpFlood);
    connect(tcpFlood, tcpFlood->clicked , this, this->TCPFlood);
    group->addButton(tcpFlood);
    QPushButton *udpFlood = new QPushButton("UDP Flood attack");
    udpFlood->setCheckable(1);
    list->addWidget(udpFlood);
    connect(udpFlood, udpFlood->clicked , this, this->UDPFlood);
    group->addButton(udpFlood);
    QPushButton *tcpSyn = new QPushButton("TCP SYN/ACK attack");
    tcpSyn->setCheckable(1);
    list->addWidget(tcpSyn);
    connect(tcpFlood, tcpFlood->clicked , this, this->TCPFlood);
    group->addButton(tcpSyn);

    //list->addWidget(new QScrollBar(Qt::Vertical));
    list->setSpacing(0);
    list->setContentsMargins(0, 0, 0, 0);
    list->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));
    //listLayout->addWidget(list);

    QGroupBox *groupBox = new QGroupBox(tr("Session list"));
    groupBox->setLayout(list);
    //groupBox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    groupBox->setVisible(1);

    return groupBox;
}

void Window::TCPFlood(){
    QObjectList temp = this->children();
    QGroupBox *atemp = (QGroupBox*)temp.at(2);
    atemp->setVisible(1);
    QGroupBox *btemp = (QGroupBox*)temp.at(3);
    btemp->setVisible(1);

    QLineEdit* tempLine = qobject_cast<QLineEdit*>(atemp->children().at(2));
    tempLine->setText("Predefined TCP Flood");


        qobject_cast<QRadioButton*>(atemp->children().at(4))->setChecked(true);

        qobject_cast<QRadioButton*>(atemp->children().at(6))->setChecked(true);




}



void Window::UDPFlood(){
    QObjectList temp = this->children();
    QGroupBox *atemp = (QGroupBox*)temp.at(2);
    atemp->setVisible(1);
    QGroupBox *btemp = (QGroupBox*)temp.at(3);
    btemp->setVisible(1);

    QLineEdit* tempLine = qobject_cast<QLineEdit*>(atemp->children().at(2));
    tempLine->setText("Predefined UDP Flood");


        qobject_cast<QRadioButton*>(atemp->children().at(4))->setChecked(true);

        qobject_cast<QRadioButton*>(atemp->children().at(7))->setChecked(true);



}



QGroupBox *Window::createDataInput()
{
    QGroupBox *groupBox = new QGroupBox();

    QVBoxLayout *v1 = new QVBoxLayout;
    QVBoxLayout *v2 = new QVBoxLayout;
    QVBoxLayout *v3 = new QVBoxLayout;
    QVBoxLayout *v4 = new QVBoxLayout;


    QHBoxLayout *h1 = new QHBoxLayout;
    QHBoxLayout *h2 = new QHBoxLayout;
    //grid->addLayout(h1,0,0);
    //grid->addLayout(h2,1,0);
    //grid->addLayout(h1,1,1);
    //grid->addLayout(h2);

    QLabel *ipSrcLabel = new QLabel("Source IP");
    ipSrcLabel->setAlignment(Qt::AlignHCenter);
    QLineEdit *ipSrc = new QLineEdit();
    ipSrc->setInputMask("000.000.000.000; ");
    ipSrc->setAlignment(Qt::AlignHCenter);

    QLabel *ipDstLabel = new QLabel("Dest. IP");
    ipDstLabel->setAlignment(Qt::AlignHCenter);
    QLineEdit *ipDst = new QLineEdit();
    ipDst->setInputMask("000.000.000.000; ");
    ipDst->setAlignment(Qt::AlignHCenter);

    QLabel *macSrcLabel = new QLabel("Source MAC");
    macSrcLabel->setAlignment(Qt::AlignHCenter);
    QLineEdit *macSrc = new QLineEdit();
    macSrc->setInputMask("000.000.000.000; ");
    macSrc->setAlignment(Qt::AlignHCenter);

    QLabel *macDstLabel = new QLabel("Dest. MAC");
    macDstLabel->setAlignment(Qt::AlignHCenter);
    QLineEdit *macDst = new QLineEdit();
    macDst->setInputMask("000.000.000.000; ");
    macDst->setAlignment(Qt::AlignHCenter);


    QComboBox *Payload = new QComboBox();
    Payload->setEditable(1);
    Payload->addItem("Hello World.");
    Payload->addItem("Whats up");
    Payload->addItem("Test1");
    Payload->addItem("Test2");
    Payload->setCurrentText(" ");

    v3->addWidget(new QLabel("Payload"),1,Qt::AlignHCenter);
    v3->addWidget(Payload,1,Qt::AlignHCenter);




    v1->addWidget(ipSrcLabel,1,Qt::AlignHCenter);
    v1->addWidget(ipSrc,1,Qt::AlignHCenter);
    v2->addWidget(ipDstLabel,1,Qt::AlignHCenter);
    v2->addWidget(ipDst,1,Qt::AlignHCenter);

    v1->addWidget(macSrcLabel,1,Qt::AlignHCenter);
    v1->addWidget(macSrc,1,Qt::AlignHCenter);
    v2->addWidget(macDstLabel,1,Qt::AlignHCenter);
    v2->addWidget(macDst,1,Qt::AlignHCenter);


    h1->addLayout(v1);
    h1->addLayout(v2);

    v4->addLayout(h1);
    v4->addLayout(v3);

//
    /*grid->addWidget(ipSrc,0,0);
    grid->addWidget(ipDst ,0,1);
    grid->addWidget(macSrc,1,0);
    grid->addWidget(macDst,1,1);
    grid->addWidget(Payload,2,0);*/



    QPushButton *confirmButton = new QPushButton("Confirm");
    QPushButton *clearButton = new QPushButton("Clear");
    h2->addWidget(clearButton,0,Qt::AlignLeft);
    connect( clearButton, clearButton->clicked , this, this->clearGroup);
    h2->addWidget(confirmButton,0,Qt::AlignRight);
    connect( confirmButton, confirmButton->clicked , this, this->close );



    /*QSignalMapper* signalMapper = new QSignalMapper (parentPointer) ;
    //connect( confirmButton, confirmButton->clicked , parentPointer->addSession );

    connect(confirmButton, SIGNAL(clicked()) , signalMapper , SLOT(map()) );

    signalMapper->setMapping(this,newSession);

    connect ( signalMapper, SIGNAL(mapped(QObject*)), parentPointer, SLOT(addSession(QObject*)));
*/

    connect( confirmButton, confirmButton->clicked, this, this->createSession);
    connect( confirmButton, confirmButton->clicked, this, this->confirmBut);
    connect( confirmButton, confirmButton->clicked , parentPointer , parentPointer->displaySessions );
    v4->addLayout(h2);
    groupBox->setLayout(v4);
    groupBox->setVisible(0);

    return groupBox;

}

QGroupBox *Window::createStream()
{
    QGroupBox *groupBox = new QGroupBox(tr("Stream"));


       groupBox->setVisible(0);
       return groupBox;
}


void Window::confirmBut(){
    parentPointer->addSession(this->newSession);
}

/*SessionWindow::SessionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SessionWindow)
{
    ui->setupUi(this);
}

SessionWindow::~SessionWindow()
{
    delete ui;
}
*/
