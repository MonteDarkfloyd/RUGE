#ifndef TRAFFICWINDOW_H
#define TRAFFICWINDOW_H

#include <QMainWindow>
#include <session.h>
#include <QTableWidgetItem>



namespace Ui {
class TrafficWindow;
}

class TrafficWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit TrafficWindow(QWidget *parent = 0);
    ~TrafficWindow();

    // Used to update the table in the view
    // that shows the sessions.
    void displaySessions();

    // Add new session to the sessionlist.
    // Will also do displaySessions when used.
    void addSession(Session* newSess);

    // Check if session name is already in use.
    // Parameter: name, the name that is checked
    // Return value false if the name is already in use,
    // true if the name is not in use.
    bool checkName(QString name);


private slots:

    // Quit is selected from top menu
    // Quit the program
    void on_actionQuit_triggered();

    // Edit button is clicked. Send the selected sessions
    // data to the createsession view so user can edit it.
    void on_editButton_clicked();

    // Delete the selected session when delete is clicked.
    void on_deleteButton_clicked();

    // Add new session by opening newsessiondialog or if
    // there is no predefined sessions then move to createsession view.
    void on_newButton_clicked();

    // Selection changed from mouse or keyboard
    // Enable delete and edit buttons
    void on_tableWidget_itemSelectionChanged();

    // Mouse is over an item in the table. If the item is the session name
    // set a new tooltip.
    void on_tableWidget_itemEntered(QTableWidgetItem *item);

    // Load session button, open file dialog and then try to open
    // the xml user chose.
    void on_loadSButton_clicked();

    // Save session button, open file dialog and then try to save
    // the session profile file.
    void on_saveSButton_clicked();

    // Load traffic button, open file dialog and then try to open
    // the xml user chose.
    void on_loadTButton_clicked();

    // Save traffic button, open file dialog and then try to save
    // the traffic profile file and all sessions.
    void on_saveTButton_clicked();

    // Start generation button. First, if edited, ask user to save the traffic profile.
    // Then use command line to sent the traffic file for the RUGE program.
    void on_startButton_clicked();

    // Reset button pressed. Use command line to send
    // a command to the RUGE program to soft reset the engine
    void on_resetButton_clicked();

    // Hard reset chosed from top menu. Use command line to send
    // a command to the RUGE program to hard reset the engine
    void on_actionHard_Reset_triggered();

    // When table is edited, set the values to maximum value if necessary.
    // And save the value to the session.
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    // Item was double clicked. If item was session name, move to the edit
    // otherwise enter editing the value in the table.
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_actionAbout_triggered();

private:

    // Deletes all sessions.
    void deleteSessions();

    Ui::TrafficWindow *ui;
    QList<Session *> sessionList_;
    int selectedRow_;
    QString currentTraffic_;
    bool edited_;
    bool saveCancel_;

};

#endif // TRAFFICWINDOW_H
