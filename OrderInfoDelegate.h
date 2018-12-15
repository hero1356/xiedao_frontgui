#ifndef ORDERINFODELEGATE_H
#define ORDERINFODELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>

class OrderInfoDelegate : public QItemDelegate
{
public:
    explicit OrderInfoDelegate(QObject* parent=0);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // ORDERINFODELEGATE_H
