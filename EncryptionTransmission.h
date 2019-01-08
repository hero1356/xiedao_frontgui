#ifndef _ENCRYPTIONTRANSMISSION_H_
#define _ENCRYPTIONTRANSMISSION_H_

#include <QObject>

#define ENCRYPTION_TRANSMISSION 1

class EncryptionTransmission : public QObject
{
    Q_OBJECT

    QString m_userName;
    QString m_userPwd;
public:
    explicit EncryptionTransmission(QObject *parent = 0);

    void setUserName(QString userName);
    void setUserPwd(QString userPwd);
    QString generateSign(QString input);
    QString httpGetGenerateSign(QString input);
    QString httpPostGenerateSign(QString input);
};

#endif // _ENCRYPTIONTRANSMISSION_H_
