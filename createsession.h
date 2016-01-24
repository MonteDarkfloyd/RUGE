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

    // Constructor, that gets parent and the trafficwindow pointer.
    // This class uses trafficwindows functions.
    explicit createsession(QWidget *parent = 0, TrafficWindow* Tparent = 0);
    ~createsession();

    // Reads editedSessions data and adds it to the view.
    // Used when editing and opening predefined session.
    // Parameter: Pointer to the session which data is read
    // and added to the view.
    void setSession(Session* editedSession);

    // Sets edit mode to true. When the editmode is true,
    // and the view is accepted. The views data is
    // edited to existing session and the view doesn't create new session.
    void setEditMode();

private slots:

    // Radio buttons are toggled. Show apporiate data to user.
    void on_rb_ip4_toggled(bool checked);

    void on_lb_ip6_toggled(bool checked);

    void on_rb_tcp_toggled(bool checked);

    void on_rb_udp_toggled(bool checked);

    void on_rb_icmp_toggled(bool checked);

    void on_rb_sctp_toggled(bool checked);

    // Checkboxes are toggled. Enable/Disable
    // data editing.
    void on_mac_checkBox_toggled(bool checked);

    void on_overrideipsrc_toggled(bool checked);

    void on_sourceport_checkBox_toggled(bool checked);

    void on_payload_checkBox_toggled(bool checked);


    // Combobox values changed. Change range editing.

    void on_ipSourceComboBox_activated(int index);

    void on_ipDestComboBox_activated(int index);

    // One of the buttons pressed.

    void on_cancel_button_clicked();

    void on_confirm_Button_clicked();

    void on_predefined_Button_clicked();

protected:
    // Catch enter and escape keys presses.
    // Enter sets focus to confirm button and presses it
    // if it already had focus. Escape key closes window.
    void keyPressEvent(QKeyEvent *);
private:

    // Used to check the data that is inputted to the view.
    // Prints error messages if unsuccessful.
    // Saves the session to the createdSession_.
    // If edit mode is false, creates new Session.
    // Return value: boolean, true if success, false otherwise.
    bool makeSession();
    Ui::createsession *ui;
    TrafficWindow* parentPointer;
    Session* createdSession_;
    Session* editSession_;
    bool editMode;




};

#endif // CREATESESSION_H
