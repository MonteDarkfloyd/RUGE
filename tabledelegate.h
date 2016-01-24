#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QItemDelegate>

// Delegate to the table in trafficwindow so user can enter numbers from
// 0-10000000000
class TableDelegate: public QItemDelegate
{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                                         const QModelIndex & index) const;
};

#endif // TABLEDELEGATE_H
