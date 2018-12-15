#include "CardInfo.h"

CardInfo::CardInfo(QObject *parent) : QObject(parent)
{
    m_cardID = "NULL";
    m_cardSN = "NULL";
}

CardInfo::CardInfo(QString cardid, QString cardsn, QObject *parent )
{
    parent = parent;
    m_cardID = cardid;
    m_cardSN = cardsn;
}

CardInfo::CardInfo(const CardInfo& obj)
{
    m_cardID = obj.m_cardID;
    m_cardSN = obj.m_cardSN;
}

CardInfo& CardInfo::operator = (const CardInfo& obj)
{
    if( this != &obj )
    {
        m_cardID = obj.m_cardID;
        m_cardSN = obj.m_cardSN;
    }
    return *this;
}

QString CardInfo::cardID()
{
    return m_cardID;
}

QString CardInfo::cardSN()
{
    return m_cardSN;
}
