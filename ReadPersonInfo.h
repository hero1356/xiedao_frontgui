#ifndef READPERSONINFO_H
#define READPERSONINFO_H

#include <QObject>
#include <QList>
#include "PersonInfo.h"

class ReadPersonInfo : public QObject
{
    Q_OBJECT

    QList<PersonInfo> m_data;
    bool parse(QVariant var, QList<PersonInfo> &list);
    void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res);

public:
    explicit ReadPersonInfo(QObject *parent = 0);
    QString showFileDialog(QString title);
    QString getData();
    QList<PersonInfo> fetchData();
    int count();
};

#endif // READPERSONINFO_H
