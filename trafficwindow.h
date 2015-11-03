#ifndef TRAFFICWINDOW_H
#define TRAFFICWINDOW_H

#include <QMainWindow>
#include <session.h>


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
    int lastRow;
    void editSession(Session *Sess);


    ~TrafficWindow();

private slots:
    void on_actionQuit_triggered();

    void on_pushButton_pressed();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void setRow(int a,int b);



    void on_pushButton_2_clicked();

private:
    Ui::TrafficWindow *ui;
    QList<Session *> sessionList;

};

#endif // TRAFFICWINDOW_H
