#include "AddCard.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

AddCard::AddCard(QWidget *parent) : QWidget(parent, Qt::WindowCloseButtonHint)
{
    m_isAddCardSuccedd = false;

    m_cardIdLbl.setParent(this);
    m_cardSnLbl.setParent(this);
    m_cardSnEdit.setParent(this);
    m_cardIdEdit.setParent(this);
    m_OkBtn.setParent(this);

    m_cardIdEdit.setReadOnly(true);
    m_cardIdEdit.setPlaceholderText("请刷卡...");

    m_cardSnLbl.setText("卡片编号:");
    m_cardIdLbl.setText("卡片ID:");
    m_OkBtn.setText("添加");
    connect(&m_OkBtn, SIGNAL(clicked(bool)), this, SLOT(onOkBtnClicked()));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(&m_cardSnLbl, 0, 0);
    layout->addWidget(&m_cardSnEdit, 0, 1);
    layout->addWidget(&m_cardIdLbl, 1, 0);
    layout->addWidget(&m_cardIdEdit, 1, 1);
    layout->addWidget(&m_OkBtn, 1, 2);
    setLayout(layout);

    setWindowTitle("添加卡片");
}

void AddCard::onOkBtnClicked()
{
    QString cardID = m_cardIdEdit.text();
    QString cardSN = m_cardSnEdit.text();

    Http* pHttpFun = new Http();
    QString strUrl = "http://10.55.206.28:8687/card/add?cardid=" + cardID + "&cardsn=" + cardSN;
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_cardAddResult(bool,const QString&)));

    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->get(strUrl);
}


QString AddCard::slot_cardAddResult(bool success, const QString &strResult)
{
    success = success;

    qDebug() << "slot_cardAddResult:"<<strResult;
    QString ret = "default";
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
            if(rootObj.contains("rslt"))
            {
                QJsonValue value = rootObj.value("rslt");
                //判断是否是string类型
                if (value.isString())
                {
                    QString data1 = value.toString();
                    if(data1=="success")
                    {
                        m_isAddCardSuccedd = true;
                        ret = "ok";
                    }
                }
            }

            //是否含有key  reason
            if(rootObj.contains("reason"))
            {
                QJsonValue value = rootObj.value("reason");
                //判断是否是string类型
                if (value.isString())
                {
                    m_isAddCardSuccedd = false;
                    m_AddFailedReason = value.toString();
                    ret = "ok";
                }
            }

        }else{
            ret = "json is not object";
        }
    }
    qDebug() << ret;

    postProcess();

    return ret;
}

QString AddCard::slot_receiveCardID(unsigned int cardID)
{
    m_cardIdEdit.setText(QString::number(cardID));
}


void AddCard::postProcess()
{
    if( isAddCardSuccedd() )
    {
        showInformationMessage("添加卡片成功！");
    }
    else
    {
        showErrorMessage("添加卡片失败！\n"+ AddFailedReason());
        qDebug() << AddFailedReason();
    }
}


void AddCard::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("错误");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    QTimer::singleShot(1000, &msg,SLOT(accept()));
    msg.exec();
}

void AddCard::showInformationMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("提示");
    msg.setText(message);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    QTimer::singleShot(1000, &msg,SLOT(accept()));
    msg.exec();
}

