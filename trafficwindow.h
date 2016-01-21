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
    void displaySessions();
    void addSession(Session* newSess);
    void editSession(Session *Sess);
    bool checkName(QString name);


    ~TrafficWindow();

private slots:
    void on_actionQuit_triggered();

    void on_editButton_clicked();

    void on_deleteButton_clicked();

    void on_newButton_clicked();

    void on_tableWidget_itemSelectionChanged();


    void on_tableWidget_itemEntered(QTableWidgetItem *item);

    void on_loadSButton_clicked();

    void on_saveSButton_clicked();

    void on_loadTButton_clicked();

    void on_startButton_clicked();

    void on_saveTButton_clicked();

    void on_resetButton_clicked();

    void on_actionHard_Reset_triggered();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

private:

    void deleteSessions();

    Ui::TrafficWindow *ui;
    QList<Session *> sessionList;
    int lastRow;
    QString currentTraffic;
    bool edited;
    bool saveCancel;

};

#endif // TRAFFICWINDOW_H
