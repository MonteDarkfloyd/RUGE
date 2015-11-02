#ifndef TRAFFICWINDOW_H
#define TRAFFICWINDOW_H

#include <QMainWindow>


namespace Ui {
class TrafficWindow;
}

class TrafficWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrafficWindow(QWidget *parent = 0);
    ~TrafficWindow();

private slots:
    void on_actionQuit_triggered();

    void on_pushButton_pressed();

private:
    Ui::TrafficWindow *ui;
};

#endif // TRAFFICWINDOW_H
