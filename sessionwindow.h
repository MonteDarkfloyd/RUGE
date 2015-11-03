#ifndef SESSIONWINDOW_H
#define SESSIONWINDOW_H
#include <QWidget>
#include <QListWidget>
#include "trafficwindow.h"

class QGroupBox;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(TrafficWindow *parent = 0);
    //Window(TrafficWindow *parent = 0,Session* session=0, int a=0);


    void firstVisible();
    void secondVisible();
    void thirdVisible();
    void clearGroup();
    void confirmBut();
    void createSession();
    void displaySession(Session *session);
    void clickedNew();
    void editSession();
    void TCPFlood();
    void UDPFlood();
private:

    QGroupBox *createProtocolSelection();
    QGroupBox *createList();
    QGroupBox *createDataInput();
    QGroupBox *createStream();
    TrafficWindow* parentPointer;
    Session *newSession;



};




#endif // SESSIONWINDOW_H

