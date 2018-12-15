#include "OrderInfo.h"

OrderInfo::OrderInfo(QObject *parent) : QObject(parent)
{
}
OrderInfo::OrderInfo(QString orderdate,
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
                        QObject *parent)
{
    m_orderdate =	orderdate;
    m_orderno =	orderno;
    m_userno =	userno;

    m_username =  username;
    m_age =  age;
    m_identitycard =  identitycard;
    m_telephone =  telephone;
    m_email =  email;

    m_workunit =  workunit;
    m_team =  team;
    m_group =  group;
    m_teamleader =  teamleader;

    m_leadername = leadername;

    m_account = account;
    m_ismakecard = ismakecard;
    m_cardsn = cardsn;
}

OrderInfo::OrderInfo(const OrderInfo& obj)
{
    m_orderdate = obj.m_orderdate;
    m_orderno = obj.m_orderno;
    m_userno = obj.m_userno;

    m_username = obj.m_username;
    m_age = obj.m_age;
    m_identitycard = obj.m_identitycard;
    m_telephone = obj.m_telephone;
    m_email = obj.m_email;

    m_workunit = obj.m_workunit;
    m_team = obj.m_team;
    m_group = obj.m_group;
    m_teamleader = obj.m_teamleader;

    m_leadername = obj.m_leadername;

    m_account = obj.m_account;
    m_ismakecard = obj.m_ismakecard;
    m_cardsn = obj.m_cardsn;
}

OrderInfo& OrderInfo::operator = (const OrderInfo& obj)
{
    if( this != &obj )
    {

        m_orderdate = obj.m_orderdate;
        m_orderno = obj.m_orderno;
        m_userno = obj.m_userno;

        m_username = obj.m_username;
        m_age = obj.m_age;
        m_identitycard = obj.m_identitycard;
        m_telephone = obj.m_telephone;
        m_email = obj.m_email;

        m_workunit = obj.m_workunit;
        m_team = obj.m_team;
        m_group = obj.m_group;
        m_teamleader = obj.m_teamleader;

        m_leadername = obj.m_leadername;

        m_account = obj.m_account;
        m_ismakecard = obj.m_ismakecard;
        m_cardsn = obj.m_cardsn;
    }
    return *this;
}

QString OrderInfo::orderdate()
{
    return m_orderdate;
}

QString OrderInfo::orderno()
{
    return m_orderno;
}

QString OrderInfo::userno()
{
    return m_userno;
}

QString OrderInfo::username()
{
    return m_username;
}

QString OrderInfo::age()
{
    return m_age;
}

QString OrderInfo::identitycard()
{
    return m_identitycard;
}

QString OrderInfo::telephone()
{
    return m_telephone;
}

QString OrderInfo::email()
{
    return m_email;
}

QString OrderInfo::workunit()
{
    return m_workunit;
}

QString OrderInfo::team()
{
    return m_team;
}

QString OrderInfo::group()
{
    return m_group;
}

QString OrderInfo::teamleader()
{
    return m_teamleader;
}

QString OrderInfo::leadername()
{
    return m_leadername;
}

QString OrderInfo::account()
{
    return m_account;
}

bool OrderInfo::ismakecard()
{
    return m_ismakecard;
}

QString OrderInfo::cardsn()
{
    return m_cardsn;
}

void OrderInfo::set_orderdate(QString orderdate)
{
    m_orderdate = orderdate;
}

void OrderInfo::set_orderno(QString orderno)
{
    m_orderno = orderno;
}

void OrderInfo::set_userno(QString userno)
{
    m_userno = userno;
}

void OrderInfo::set_username(QString username)
{
    m_username = username;
}

void OrderInfo::set_age(QString age)
{
    m_age = age;
}

void OrderInfo::set_identitycard(QString identitycard)
{
    m_identitycard = identitycard;
}

void OrderInfo::set_telephone(QString telephone)
{
    m_telephone = telephone;
}

void OrderInfo::set_email(QString email)
{
    m_email = email;
}

void OrderInfo::set_workunit(QString workunit)
{
    m_workunit = workunit;
}

void OrderInfo::set_team(QString team)
{
    m_team = team;
}

void OrderInfo::set_group(QString group)
{
    m_group = group;
}

void OrderInfo::set_teamleader(QString teamleader)
{
    m_teamleader = teamleader;
}

void OrderInfo::set_leadername(QString leadername)
{
    m_leadername = leadername;
}


void OrderInfo::set_account(QString account)
{
    m_account = account;
}

void OrderInfo::set_ismakecard(bool ismakecard)
{
    m_ismakecard = ismakecard;
}

void OrderInfo::set_cardsn(QString cardsn)
{
    m_cardsn = cardsn;
}

