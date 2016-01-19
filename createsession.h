#ifndef CREATESESSION_H
#define CREATESESSION_H

#include <QWidget>
#include "trafficwindow.h"
#include "session.h"

namespace Ui {
class createsession;
}

class createsession : public QWidget
{
    Q_OBJECT

public:
    explicit createsession(QWidget *parent = 0, TrafficWindow* Tparent = 0);
    ~createsession();

    void setSession(Session* editedSession);
    void setEditMode();

private slots:

    void on_rb_ip4_toggled(bool checked);

    void on_lb_ip6_toggled(bool checked);

    void on_rb_tcp_toggled(bool checked);

    void on_rb_udp_toggled(bool checked);

    void on_rb_icmp_toggled(bool checked);

    void on_rb_sctp_toggled(bool checked);

    void on_mac_checkBox_toggled(bool checked);

    void on_overrideipsrc_toggled(bool checked);

    void on_checkBox_toggled(bool checked);

    void on_sourceport_checkBox_toggled(bool checked);

    void on_checkBox_seq_tcp_toggled(bool checked);

    void on_checkBox_ack_tcp_toggled(bool checked);

    void on_checkBox_src_porttcp_toggled(bool checked);

    void on_cancel_button_clicked();

    void on_payload_checkBox_toggled(bool checked);

    void on_confirm_Button_clicked();


    void on_ipSourceComboBox_activated(int index);

    void on_ipDestComboBox_activated(int index);

private:
    Ui::createsession *ui;
    TrafficWindow* parentPointer;
    Session* editSession;
    bool editMode;




};

#endif // CREATESESSION_H
