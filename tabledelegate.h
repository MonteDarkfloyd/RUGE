#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QItemDelegate>


class TableDelegate: public QItemDelegate
{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                                         const QModelIndex & index) const;
};

#endif // TABLEDELEGATE_H
