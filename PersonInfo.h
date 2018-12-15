#ifndef PERSONINFO_H
#define PERSONINFO_H

#include <QObject>

class PersonInfo : public QObject
{
    Q_OBJECT

    QString m_name;
    QString m_age;
    QString m_phoneNum;
    QString m_id;
    QString m_playTime;
    QString m_workUnit;
    QString m_groupNum;
public:
    explicit PersonInfo(QObject *parent = 0);
    explicit PersonInfo(QString name , QString age, QString phoneNum = NULL, QString id = NULL, QString playTime = NULL, QString workUnit = "新建队", QString groupNum = "1", QObject *parent = 0);
    PersonInfo(const PersonInfo& obj);
    PersonInfo& operator = (const PersonInfo& obj);

    QString name();
    QString age();
    QString phoneNum();
    QString id();
    QString playTime();
    QString workUnit();
    QString groupNum();

signals:

public slots:
};

#endif // PERSONINFO_H
