#ifndef HISTORYINFOMODEL_H
#define HISTORYINFOMODEL_H

#include <QObject>
#include <QStandardItem>
#include <QTableView>
#include <QList>
#include "HistoryInfo.h"

class HistoryInfoModel : public QObject
{
    Q_OBJECT
    QStandardItemModel m_model;

public:
    explicit HistoryInfoModel(QObject *parent = nullptr);

    bool add(HistoryInfo info);
    bool add(QList<HistoryInfo> list);
    bool remove(int i);
    HistoryInfo getItem(int i);
    int count();
    void clear();
    void setView(QTableView& view);
    void setHeader();


};

#endif // HISTORYINFOMODEL_H
