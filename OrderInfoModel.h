#ifndef ORDERINFOMODEL_H
#define ORDERINFOMODEL_H

#include <QObject>
#include <QStandardItem>
#include <QTableView>
#include <QList>
#include "OrderInfo.h"


class OrderInfoModel : public QObject
{
    Q_OBJECT

    QStandardItemModel m_model;
public:
    explicit OrderInfoModel(QObject *parent = 0);
    bool add(OrderInfo info);
    bool add(QList<OrderInfo> list);
    bool remove(int i);
    OrderInfo getItem(int i);
    int count();
    void clear();
    void setView(QTableView& view);

    QModelIndex index(int row, int column);
    void setData(QModelIndex index, QVariant data);

    void setHeader();

};

#endif // ORDERINFOMODEL_H
