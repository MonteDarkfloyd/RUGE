#include "createsession.h"
#include "ui_createsession.h"
#include "mainwindow.h"

QVBoxLayout* v_ip4_layout;

createsession::createsession(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createsession)
{
    ui->setupUi(this);




     v_ip4_layout=new QVBoxLayout(this);

    QVBoxLayout* v_ip6_layout=new QVBoxLayout(this);
    QLabel* lbl_empty=new QLabel(this);
    lbl_empty->setText("edewded");
    v_ip6_layout->addWidget(lbl_empty);


    QHBoxLayout* h11_layout=new QHBoxLayout(this);


    QLabel* lbl_IPsource=new QLabel(this);
    lbl_IPsource->setText("IP Source: ");
    h11_layout->addWidget(lbl_IPsource);


    QLineEdit* txtbx_IPsource=new QLineEdit(this);
    h11_layout->addWidget(txtbx_IPsource);

    QLabel* lbl_incount=new QLabel(this);
    lbl_incount->setText("Increament Count: ");
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
    lbl_incount_dest->setText("Increament Count: ");
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





    //ui->groupBox_2->setLayout(v_ip4_layout);


    ui->groupBox_2->setLayout(v_ip4_layout);




    ui->rb_ip4->setChecked(true);

    ui->groupBox_6->setVisible(false);
    ui->rb_tcp->setChecked(true);


}

createsession::~createsession()
{
    delete ui;
}

void createsession::on_pushButton_clicked()
{
    MainWindow* w=new MainWindow();
    w->show();
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
