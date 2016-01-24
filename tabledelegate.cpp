#include "tabledelegate.h"
#include <QWidget>
#include <QLineEdit>

// Delegate to the table in trafficwindow so user can enter numbers from
// 0-10000000000
QWidget* TableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                                     const QModelIndex & index) const
                   {
                       QLineEdit *lineEdit = new QLineEdit(parent);
                       // Set validator
                       QIntValidator *validator = new QIntValidator(0, 10000000000, lineEdit);
                       lineEdit->setValidator(validator);
                       return lineEdit;
               };
