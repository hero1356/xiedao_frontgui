#ifndef GROUPINFO_H
#define GROUPINFO_H

#include <QObject>

class GroupInfo : public QObject
{
    Q_OBJECT
    QString m_cardid;
    QString m_cardsn;
    QString m_teamname;
    QString m_teamid;
    QString m_groupname;
    QString m_groupid;
public:
    explicit GroupInfo(QObject *parent = 0);
    GroupInfo(const GroupInfo& obj);
    GroupInfo& operator = (const GroupInfo& obj);

    QString key();
    QString value();

    QString cardid();
    QString cardsn();
    QString teamname();
    QString teamid();
    QString groupname();
    QString groupid();


    void set_cardid(QString cardid);
    void set_cardsn(QString cardsn);
    void set_teamname(QString teamname);
    void set_teamid(QString teamid);
    void set_groupname(QString groupname);
    void set_groupid(QString groupid);
};

#endif // GROUPINFO_H
