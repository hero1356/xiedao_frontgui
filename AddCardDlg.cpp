#include "AddCardDlg.h"
#include "ui_AddCardDlg.h"
#include <QMessageBox>
#include <QTimer>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegExp>
#include <QCryptographicHash>
#include <qDebug>

AddCardDlg::AddCardDlg(QString userName, QString userPwd, QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::AddCardDlg)
{
    ui->setupUi(this);

    m_isAddCardSuccess = false;

    m_et.setUserName(userName);
    m_et.setUserPwd(userPwd);

    QRegExp regExp("[A-Z][0-9][0-9][0-9]");
    ui->cardSNEdit->setValidator(new QRegExpValidator(regExp,this));

    AppConfig config;
    dest_ip_and_port = "https://"+config.ip()+":"+config.port();
    qDebug() << "目标地址" <<dest_ip_and_port;
}

AddCardDlg::~AddCardDlg()
{
    delete ui;
}

void AddCardDlg::postProcess()
{
    if( isAddCardSuccedd() )
    {
        emit signal_addCardSuccessed(ui->cardIDEdit->text(), ui->cardSNEdit->text());
//        showInformationMessage("添加卡片成功！");
    }
    else
    {
        showErrorMessage("添加卡片失败！"+ AddFailedReason());
        qDebug() << AddFailedReason();
    }
}

void AddCardDlg::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("错误");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok," 确定 ");
    msg.setStyleSheet("QLabel { color: black; min-height: 30px;font-size: 15px;}"
                      "QPushButton{color : white; background-color: #27a9e3; border-width:0px; border-radius:3px;\
                      border-style: outset;\
                      min-height: 30px;min-width: 60px;\
                      font-size: 15px;}");
    QTimer::singleShot(1000, &msg,SLOT(accept()));
    msg.exec();
}

void AddCardDlg::showInformationMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("提示");
    msg.setText(message);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok," 确定 ");
    msg.setStyleSheet("QLabel { color: black; min-height: 30px;font-size: 15px;}"
                      "QPushButton{color : white; background-color: #27a9e3; border-width:0px; border-radius:3px;\
                      border-style: outset;\
                      min-height: 30px;min-width: 60px;\
                      font-size: 15px;}");
    QTimer::singleShot(1000, &msg,SLOT(accept()));
    msg.exec();
}

void AddCardDlg::on_OkBtn_clicked()
{           
    QString cardID = ui->cardIDEdit->text();
    QString cardSN = ui->cardSNEdit->text();

    Http* pHttpFun = new Http();
    QString strUrl = dest_ip_and_port+"/card/add?cardid=" + hex2dec(cardID) + "&cardsn=" + cardSN;
    strUrl = m_et.httpGetGenerateSign(strUrl);
    connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
            this,SLOT(slot_cardAddResult(bool,const QString&)));

    qDebug() <<"Send http: "<< strUrl;
    pHttpFun->get(strUrl);
}

QString AddCardDlg::slot_cardAddResult(bool success, const QString &strResult)
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
                        m_isAddCardSuccess = true;
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
                    m_isAddCardSuccess = false;
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

void AddCardDlg::slot_receCardID(QString cardID)
{
    ui->cardIDEdit->setText(cardID);
}
QString AddCardDlg::dec2hex(QString src)
{
    QString ret;
    bool ok;

    int temp = src.toInt(&ok, 10);
    if( ok )
    {
        ret = QString::number(temp, 16);
    }
    else
    {
        ret = "";
    }
    return ret;
}

QString AddCardDlg::hex2dec(QString src)
{
    QString ret;
    bool ok;

    int temp = src.toInt(&ok, 16);
    if( ok )
    {
        ret = QString::number(temp, 10);
    }
    else
    {
        ret = "";
    }

    return ret;
}
