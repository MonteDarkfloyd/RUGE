#ifndef PREDEFINEDWINDOW_H
#define PREDEFINEDWINDOW_H

#include <QDialog>
#include "trafficwindow.h"

namespace Ui {
class predefinedwindow;
}

class predefinedwindow : public QDialog
{
    Q_OBJECT

public:
    explicit predefinedwindow(TrafficWindow *parent = 0);
    ~predefinedwindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::predefinedwindow *ui;
    TrafficWindow* parentPointer;
};

#endif // PREDEFINEDWINDOW_H
