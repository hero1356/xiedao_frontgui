#ifndef _CARDINFOMODEL_H_
#define _CARDINFOMODEL_H_

#include <QObject>
#include <QStandardItem>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QTableView>
#include <QList>
#include "CardInfo.h"

class CardInfoModel : public QObject
{
    Q_OBJECT
    QStandardItemModel m_model;
public:
    explicit CardInfoModel(QObject* parent = 0);
    bool add(CardInfo info);
    bool add(QList<CardInfo> list);
    bool remove(int i);
    CardInfo getItem(int i);
    int count();
    void clear();
    void setView(QTableView& view);
    void setHeader();

};

#endif // _CARDINFOMODEL_H_
