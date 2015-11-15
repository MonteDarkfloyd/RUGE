#ifndef NEWSESSIONDIALOG_H
#define NEWSESSIONDIALOG_H

#include <QDialog>
#include "trafficwindow.h"

namespace Ui {
class NewSessionDialog;
}

class NewSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewSessionDialog(TrafficWindow *parent = 0);
    ~NewSessionDialog();

private slots:
    void on_buttonBox_accepted();

    void on_createButton_clicked();

private:
    Ui::NewSessionDialog *ui;
    TrafficWindow* parentPointer;
};

#endif // NEWSESSIONDIALOG_H
