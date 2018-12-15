#include "GroupInfo.h"

GroupInfo::GroupInfo(QObject *parent) : QObject(parent)
{

}

GroupInfo::GroupInfo(const GroupInfo &obj)
{
    m_cardid = obj.m_cardid;
    m_cardsn = obj.m_cardsn;
    m_teamname = obj.m_teamname;
    m_teamid = obj.m_teamid;
    m_groupname = obj.m_groupname;
    m_teamid = obj.m_teamid;
}

GroupInfo &GroupInfo::operator =(const GroupInfo &obj)
{
    if( this != &obj )
    {
        m_cardid = obj.m_cardid;
        m_cardsn = obj.m_cardsn;
        m_teamname = obj.m_teamname;
        m_teamid = obj.m_teamid;
        m_groupname = obj.m_groupname;
        m_teamid = obj.m_teamid;
    }
    return *this;
}

QString GroupInfo::key()
{
    return teamname()+"-"+groupname();
}

QString GroupInfo::value()
{
    return teamid()+"-"+groupid();
}

QString GroupInfo::cardid()
{
    return m_cardid;
}

QString GroupInfo::cardsn()
{
    return m_cardsn;
}

QString GroupInfo::teamname()
{
    return m_teamname;
}

QString GroupInfo::teamid()
{
    return m_teamid;
}

QString GroupInfo::groupname()
{
    return m_groupname;
}

QString GroupInfo::groupid()
{
    return m_groupid;
}

void GroupInfo::set_cardid(QString cardid)
{
    m_cardid = cardid;
}

void GroupInfo::set_cardsn(QString cardsn)
{
    m_cardsn = cardsn;
}

void GroupInfo::set_teamname(QString teamname)
{
    m_teamname = teamname;
}

void GroupInfo::set_teamid(QString teamid)
{
    m_teamid = teamid;
}

void GroupInfo::set_groupname(QString groupname)
{
    m_groupname = groupname;
}

void GroupInfo::set_groupid(QString groupid)
{
    m_groupid = groupid;
}
