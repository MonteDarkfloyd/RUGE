#ifndef NEWSESSIONWINDOW_H
#define NEWSESSIONWINDOW_H

#include <QWidget>
#include "trafficwindow.h"
#include "predef.h"


namespace Ui {
class newsessionwindow;
}

class newsessionwindow : public QWidget
{
    Q_OBJECT

public:
    newsessionwindow(TrafficWindow *parent, Session *_session=0);
    newsessionwindow(predef *parent,Session* _session = 0);
    //newsessionwindow(TrafficWindow *parent = 0);
    void RemoveLayout(QWidget* widget);
    void createSession();
    void displaySession();
    ~newsessionwindow();

private slots:
    void on_radioButton_2_toggled(bool checked);

    void on_radioButton_8_toggled(bool checked);

    //void on_radioButton_11_toggled(bool checked);

    void on_radioButton_4_toggled(bool checked);

    void on_radioButton_7_toggled(bool checked);




    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::newsessionwindow *ui;
    TrafficWindow* parentPointer;
    predef* tempPointer;
    Session *newSession;

};

#endif // NEWSESSIONWINDOW_H
