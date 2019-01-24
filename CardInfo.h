#ifndef _CARDINFO_H_
#define _CARDINFO_H_

#include <QObject>

class CardInfo : public QObject
{
    QString m_cardID;
    QString m_cardSN;
public:
    explicit CardInfo();
    explicit CardInfo(QString cardid, QString cardsn);
    CardInfo(const CardInfo& obj);
    CardInfo& operator = (const CardInfo& obj);
    QString cardID();
    QString cardSN();

};

#endif // _CARDINFO_H_
