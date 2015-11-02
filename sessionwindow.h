#ifndef SESSIONWINDOW_H
#define SESSIONWINDOW_H
#include <QWidget>
#include <QListWidget>

class QGroupBox;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);


    void firstVisible();
    void secondVisible();
    void thirdVisible();
    void clearGroup();
private:
    QGroupBox *createProtocolSelection();
    QGroupBox *createList();
    QGroupBox *createDataInput();
    QGroupBox *createStream();


};




#endif // SESSIONWINDOW_H

