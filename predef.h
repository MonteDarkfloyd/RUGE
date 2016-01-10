#ifndef PREDEF_H
#define PREDEF_H

#include <QDialog>

namespace Ui {
class predef;
}

class predef : public QDialog
{
    Q_OBJECT

public:
    explicit predef(QWidget *parent = 0);
    ~predef();

private:
    Ui::predef *ui;
};

#endif // PREDEF_H
