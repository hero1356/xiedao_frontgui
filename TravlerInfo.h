#ifndef TRAVLERINFO_H
#define TRAVLERINFO_H

#include <QObject>

class TravlerInfo : public QObject
{
    Q_OBJECT

    QString m_username;
    QString m_age;
    QString m_email;
    QString m_telephone;

    QString m_account;
    QString m_cardid;
    QString m_cardsn;
    QString m_teamname;
    QString m_teamid;
    QString m_groupname;
    QString m_groupid;

    QString m_leadername;
    QString m_teamleader;
    QString m_guider;

public:
    explicit TravlerInfo(QObject *parent = nullptr);
    TravlerInfo(const TravlerInfo& obj);
    TravlerInfo& operator = (const TravlerInfo& obj);


    QString username();
    QString age();
    QString email();
    QString telephone();

    QString account();
    QString cardid();
    QString cardsn();
    QString teamname();
    QString teamid();
    QString groupname();
    QString groupid();

    QString leadername();
    QString teamleader();
    QString guider();


    void set_username(QString username);
    void set_age(QString age);
    void set_email(QString email);
    void set_telephone(QString telephone);

    void set_account(QString account);
    void set_cardid(QString cardid);
    void set_cardsn(QString cardsn);
    void set_teamname(QString teamname);
    void set_teamid(QString teamid);
    void set_groupname(QString groupname);
    void set_groupid(QString groupid);

    void set_leadername(QString leadername);
    void set_teamleader(QString teamleader);
    void set_guider(QString guider);

};

#endif // TRAVLERINFO_H
