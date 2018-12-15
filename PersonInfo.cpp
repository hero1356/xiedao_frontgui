#include "PersonInfo.h"

PersonInfo::PersonInfo(QObject *parent) : QObject(parent)
{

}

PersonInfo::PersonInfo(QString name, QString age, QString phoneNum, QString id, QString playTime, QString workUnit, QString groupNum, QObject *parent)
{
    m_name = name;
    m_age = age;
    m_phoneNum = phoneNum;
    m_id = id;
    m_playTime = playTime;
    m_workUnit = workUnit;
    m_groupNum = groupNum;
}

PersonInfo::PersonInfo(const PersonInfo& obj)
{
    m_name = obj.m_name;
    m_age = obj.m_age;
    m_phoneNum = obj.m_phoneNum;
    m_id = obj.m_id;
    m_playTime = obj.m_playTime;
    m_workUnit = obj.m_workUnit;
    m_groupNum = obj.m_groupNum;
}

PersonInfo& PersonInfo::operator = (const PersonInfo& obj)
{
    if( this != &obj)
    {
        m_name = obj.m_name;
        m_age = obj.m_age;
        m_phoneNum = obj.m_phoneNum;
        m_id = obj.m_id;
        m_playTime = obj.m_playTime;
        m_workUnit = obj.m_workUnit;
        m_groupNum = obj.m_groupNum;
    }
    return *this;
}

QString PersonInfo::name()
{
    return m_name;
}

QString PersonInfo::age()
{
    return m_age;
}

QString PersonInfo::phoneNum()
{
    return m_phoneNum;
}

QString PersonInfo::id()
{
    return m_id;
}

QString PersonInfo::playTime()
{
    return m_playTime;
}

QString PersonInfo::workUnit()
{
    return m_workUnit;
}

QString PersonInfo::groupNum()
{
    return m_groupNum;
}
