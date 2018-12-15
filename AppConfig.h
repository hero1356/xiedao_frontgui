#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>

class AppConfig : public QObject
{
    Q_OBJECT

    QString m_ip;
    QString m_port;
public:
    explicit AppConfig(QObject *parent = 0);

    bool load();
    bool jsonParse(QString strResult);

    QString ip() {return m_ip;}
    QString port() {return m_port;}

};

#endif // APPCONFIG_H
