#include "TravlerInfo.h"

TravlerInfo::TravlerInfo(QObject *parent) : QObject(parent)
{

}

TravlerInfo::TravlerInfo(const TravlerInfo &obj)
{
    m_username = obj.m_username;
    m_age = obj.m_age;
    m_email = obj.m_email;
    m_telephone = obj.m_telephone;

    m_account = obj.m_account;
    m_cardid = obj.m_cardid;
    m_cardsn = obj.m_cardsn;
    m_teamname = obj.m_teamname;
    m_teamid = obj.m_teamid;
    m_groupname = obj.m_groupname;
    m_groupid = obj.m_groupid;

    m_leadername = obj.m_leadername;
    m_teamleader = obj.m_teamleader;
    m_guider = obj.m_guider;
}

TravlerInfo &TravlerInfo::operator =(const TravlerInfo &obj)
{
    if( this != &obj)
    {
        m_username = obj.m_username;
        m_age = obj.m_age;
        m_email = obj.m_email;
        m_telephone = obj.m_telephone;

        m_account = obj.m_account;
        m_cardid = obj.m_cardid;
        m_cardsn = obj.m_cardsn;
        m_teamname = obj.m_teamname;
        m_teamid = obj.m_teamid;
        m_groupname = obj.m_groupname;
        m_groupid = obj.m_groupid;

        m_leadername = obj.m_leadername;
        m_teamleader = obj.m_teamleader;
        m_guider = obj.m_guider;
    }
    return *this;
}

QString TravlerInfo::username()
{
    return m_username;
}

QString TravlerInfo::age()
{
    return m_age;
}

QString TravlerInfo::email()
{
    return m_email;
}

QString TravlerInfo::telephone()
{
    return m_telephone;
}

QString TravlerInfo::account()
{
    return m_account;
}

QString TravlerInfo::cardid()
{
    return m_cardid;
}

QString TravlerInfo::cardsn()
{
    return m_cardsn;
}

QString TravlerInfo::teamname()
{
    return m_teamname;
}

QString TravlerInfo::teamid()
{
    return m_teamid;
}

QString TravlerInfo::groupname()
{
    return m_groupname;
}

QString TravlerInfo::groupid()
{
    return m_groupid;
}

QString TravlerInfo::leadername()
{
    return m_leadername;
}

QString TravlerInfo::teamleader()
{
    return m_teamleader;
}

QString TravlerInfo::guider()
{
    return m_guider;
}

void TravlerInfo::set_username(QString username)
{
    m_username = username;
}

void TravlerInfo::set_age(QString age)
{
    m_age = age;
}

void TravlerInfo::set_email(QString email)
{
    m_email = email;
}

void TravlerInfo::set_telephone(QString telephone)
{
    m_telephone = telephone;
}

void TravlerInfo::set_account(QString account)
{
    m_account = account;
}

void TravlerInfo::set_cardid(QString cardid)
{
    m_cardid = cardid;
}

void TravlerInfo::set_cardsn(QString cardsn)
{
    m_cardsn = cardsn;
}

void TravlerInfo::set_teamname(QString teamname)
{
    m_teamname = teamname;
}

void TravlerInfo::set_teamid(QString teamid)
{
    m_teamid = teamid;
}

void TravlerInfo::set_groupname(QString groupname)
{
    m_groupname = groupname;
}

void TravlerInfo::set_groupid(QString groupid)
{
    m_groupid = groupid;
}

void TravlerInfo::set_leadername(QString leadername)
{
    m_leadername = leadername;
}

void TravlerInfo::set_teamleader(QString teamleader)
{
    m_teamleader = teamleader;
}

void TravlerInfo::set_guider(QString guider)
{
    m_guider = guider;
}
