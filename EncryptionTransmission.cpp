#include "EncryptionTransmission.h"
#include <QCryptographicHash>
#include <QDebug>

EncryptionTransmission::EncryptionTransmission( QObject *parent) : QObject(parent)
{



}
void EncryptionTransmission::setUserName(QString userName)
{
    m_userName = userName;
}

void EncryptionTransmission::setUserPwd(QString userPwd)
{
    m_userPwd = userPwd;
}

QString EncryptionTransmission::generateSign(QString input)
{
    QString ret;

    ret = QCryptographicHash::hash(input.toLatin1(),QCryptographicHash::Md5).toHex();

    return ret;
}

QString EncryptionTransmission::httpGetGenerateSign(QString input)
{
    if( !ENCRYPTION_TRANSMISSION )
        return input;

    QString ret = input;

    int pos = input.indexOf('?');

    if(pos == -1)
    {
        QString MD5_in = m_userPwd;

        qDebug() << "MD5_in:" << MD5_in;

        QString sign = generateSign(MD5_in);

        ret += QString("?keyid="+m_userName+"&sign="+sign);

    }else{

        QString MD5_in = QString(ret.mid(pos+1)+m_userPwd);

        qDebug() << "MD5_in:" << MD5_in;

        QString sign = generateSign(MD5_in);

        ret += QString("&keyid="+m_userName+"&sign="+sign);
    }

    return ret;
}
QString EncryptionTransmission::httpPostGenerateSign(QString input)
{
    if( !ENCRYPTION_TRANSMISSION )
        return input;

    QString ret = "{\"data\":"+input;

    QString MD5_in = input + m_userPwd;

    qDebug() << "MD5_in:" << MD5_in;

    QString sign = generateSign(MD5_in);

    ret += QString(",\"keyid\":\""+m_userName+"\",\"sign\":\""+sign+"\"}");

    return ret;
}
