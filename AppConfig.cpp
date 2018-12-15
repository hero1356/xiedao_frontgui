#include "AppConfig.h"
#include <QFile>
#include <QDataStream>
#include <QApplication>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{
    load();
}
bool AppConfig::load()
{
    bool ret = true;

    QString path = QApplication::applicationDirPath() + "/config.json";

    QFile file(path);

    if(file.open(QIODevice::ReadOnly))
    {

        QString configJson = file.readAll();

        if(!jsonParse(configJson))
        {
            ret = false;
            QMessageBox::critical(0,"错误","读取配置文件失败！",QMessageBox::Ok);
        }

        file.close();
    }
    else
    {
        ret = false;
        m_ip = "192.168.1.10";
        m_port = "8687";
    }

    return ret;
}

bool AppConfig::jsonParse(QString strResult)
{
    bool ret = true;
    QString str = "default";
    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;//Qt5新类
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);//Qt5新类
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject rootObj = json.object();
            //是否含有key  rslt
            if(rootObj.contains("ip"))
            {
                QJsonValue value = rootObj.value("ip");
                //判断是否是string类型
                if (value.isString())
                {
                    m_ip = value.toString();
                    str = "ok";
                }
            }

            //是否含有key  port
            if(rootObj.contains("port"))
            {
                QJsonValue value = rootObj.value("port");
                if (value.isString())
                {
                    m_port = value.toString();
                    str = "ok";
                }
            }

        }else{
            str = "json is not object";
            ret = false;
        }
    }
    qDebug() << str;
    return ret;
}

