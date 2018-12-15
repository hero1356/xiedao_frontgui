#ifndef ORDERINFO_H
#define ORDERINFO_H

#include <QObject>

class OrderInfo : public QObject
{
    Q_OBJECT

    QString m_orderdate;
    QString m_orderno;
    QString m_userno;
    QString m_username;

    QString m_age;
    QString m_identitycard;
    QString m_telephone;
    QString m_email;

    QString m_workunit;
    QString m_team;
    QString m_group;
    QString m_teamleader;

    QString m_leadername;

    QString m_account;
    bool m_ismakecard;
    QString m_cardsn;


public:
    explicit OrderInfo(QObject *parent = 0);
    explicit OrderInfo(     QString orderdate,
                            QString orderno,
                            QString userno,
                            QString username,

                            QString age,
                            QString identitycard,
                            QString telephone,
                            QString email,

                            QString workunit,
                            QString team,
                            QString group,
                            QString teamleader,

                            QString leadername,

                            QString account,
                            bool ismakecard,
                            QString cardsn,

                            QObject *parent = 0);

    OrderInfo(const OrderInfo& obj);
    OrderInfo& operator = (const OrderInfo& obj);

    QString orderdate();
    QString orderno();
    QString userno();

    QString username();
    QString age();
    QString identitycard();
    QString telephone();
    QString email();

    QString workunit();
    QString team();
    QString group();
    QString teamleader();

    QString leadername();

    QString account();
    bool ismakecard();
    QString cardsn();


    void set_orderdate(QString orderdate);
    void set_orderno(QString orderno);
    void set_userno(QString userno);

    void set_username(QString username);
    void set_age(QString age);
    void set_identitycard(QString identitycard);
    void set_telephone(QString telephone);
    void set_email(QString email);

    void set_workunit(QString workunit);
    void set_team(QString team);
    void set_group(QString group);
    void set_teamleader(QString teamleader);

    void set_leadername(QString leadername);

    void set_account(QString account);
    void set_ismakecard(bool ismakecard);
    void set_cardsn(QString cardsn);
};

#endif // ORDERINFO_H

