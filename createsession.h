#ifndef CREATESESSION_H
#define CREATESESSION_H

#include <QWidget>

namespace Ui {
class createsession;
}

class createsession : public QWidget
{
    Q_OBJECT

public:
    explicit createsession(QWidget *parent = 0);
    ~createsession();

private slots:
    void on_pushButton_clicked();

    void on_rb_ip4_toggled(bool checked);

    void on_lb_ip6_toggled(bool checked);

    void on_rb_tcp_toggled(bool checked);

    void on_rb_udp_toggled(bool checked);

    void on_rb_icmp_toggled(bool checked);

    void on_rb_sctp_clicked();

    void on_rb_sctp_toggled(bool checked);

private:
    Ui::createsession *ui;




};

#endif // CREATESESSION_H
