#include "HistoryInfo.h"

HistoryInfo::HistoryInfo(QObject *parent) : QObject(parent)
{
    m_username = "";
    m_telephone = "";
    m_score = "";
    m_ordertime  = "";
    m_scoretime = "";
    m_openid = "";
    m_scorejson = "";
}

HistoryInfo::HistoryInfo(QString username, QString telephone, QString score, QString ordertime, QString scoretime, QString openoid, QString scorejson)
{
    m_username = username;
    m_telephone = telephone;
    m_score = score;
    m_ordertime  = ordertime;
    m_scoretime = scoretime;
    m_openid = openoid;
    m_scorejson = scorejson;
}

HistoryInfo::HistoryInfo(const HistoryInfo &obj)
{

    m_username = obj.m_username;
    m_telephone = obj.m_telephone;
    m_score = obj.m_score;
    m_ordertime = obj.m_ordertime;
    m_scoretime = obj.m_scoretime;
    m_openid = obj.m_openid;
    m_scorejson = obj.m_scorejson;
}

HistoryInfo &HistoryInfo::operator =(const HistoryInfo &obj)
{
    if( &obj != this)
    {
        m_username = obj.m_username;
        m_telephone = obj.m_telephone;
        m_score = obj.m_score;
        m_ordertime = obj.m_ordertime;
        m_scoretime = obj.m_scoretime;
        m_openid = obj.m_openid;
        m_scorejson = obj.m_scorejson;
    }
    return *this;
}
