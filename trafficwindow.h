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


    ~TrafficWindow();

private slots:
    void on_actionQuit_triggered();

    void on_editButton_clicked();

    void on_deleteButton_clicked();

    void on_newButton_clicked();

    void on_tableWidget_itemSelectionChanged();


    void on_tableWidget_itemEntered(QTableWidgetItem *item);

private:
    Ui::TrafficWindow *ui;
    QList<Session *> sessionList;
    int lastRow;

};

#endif // TRAFFICWINDOW_H
