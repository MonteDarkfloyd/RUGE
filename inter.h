#ifndef INTER_H
#define INTER_H

#include <QDialog>

namespace Ui {
class inter;
}

class inter : public QDialog
{
    Q_OBJECT

public:
    explicit inter(QWidget *parent = 0);
    ~inter();

private slots:
    void on_checkBox_10_toggled(bool checked);

    void on_pushButton_3_clicked();

private:
    Ui::inter *ui;
};

#endif // INTER_H
