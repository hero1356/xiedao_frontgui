#ifndef _CARDINFO_H_
#define _CARDINFO_H_

#include <QObject>

class CardInfo : public QObject
{
    Q_OBJECT
    QString m_cardID;
    QString m_cardSN;
public:
    explicit CardInfo(QObject *parent = 0);
    explicit CardInfo(QString cardid, QString cardsn, QObject *parent = 0);
    CardInfo(const CardInfo& obj);
    CardInfo& operator = (const CardInfo& obj);
    QString cardID();
    QString cardSN();

};

#endif // _CARDINFO_H_
