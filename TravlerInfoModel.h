#ifndef TRAVLERINFOMODEL_H
#define TRAVLERINFOMODEL_H

#include <QObject>
#include <QStandardItem>
#include <QTableView>
#include "TravlerInfo.h"

class TravlerInfoModel : public QObject
{
    Q_OBJECT

    QStandardItemModel m_model;
public:
    explicit TravlerInfoModel(QObject *parent = 0);
    bool add(TravlerInfo info);
    TravlerInfo getItem(int i);

    bool add(QList<TravlerInfo> list);
    bool remove(int i);
    int count();
    void clear();
    void setView(QTableView& view);

    QModelIndex index(int row, int column);
    void setData(QModelIndex index, QVariant data);

    void setHeader();
};

#endif // TRAVLERINFOMODEL_H
