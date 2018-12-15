#ifndef PERSONINFOMODEL_H
#define PERSONINFOMODEL_H

#include <QObject>
#include <QStandardItem>
#include <QTableView>
#include <QList>
#include "PersonInfo.h"

class PersonInfoModel : public QObject
{
    Q_OBJECT

    QStandardItemModel m_model;
public:
    explicit PersonInfoModel(QObject *parent = 0);
    bool add(PersonInfo info);
    bool add(QList<PersonInfo> list);
    bool remove(int i);
    PersonInfo getItem(int i);
    int count();
    void clear();
    void setView(QTableView& view);

    void setHeader();

};

#endif // PERSONINFOMODEL_H
