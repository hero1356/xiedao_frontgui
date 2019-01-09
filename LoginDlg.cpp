#include "LoginDlg.h"
#include "ui_LoginDlg.h"
#include <QTime>
#include <QDateTime>
#include <QPainter>
#include <QFont>
#include <QMessageBox>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


LoginDlg::LoginDlg(QWidget *parent): QDialog(parent, Qt::FramelessWindowHint ),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);

    //读取配置文件，设置IP和Port
    AppConfig config;
    dest_ip_and_port = "https://"+config.ip()+":"+config.port();


    //
    m_timer.start(100);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
    m_captcha = getCaptcha();
    m_colors = getColors();

    ui->loginBtn->setShortcut(Qt::Key_Return);

    ui->captchaEdit->setVisible(false);

}

/*
void LoginDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int x = ui->captchaArea->geometry().x() + ui->LoginForm->x() + 6;
    int y = ui->captchaArea->geometry().y() + ui->LoginForm->y();
    int w = ui->captchaArea->geometry().width() - 6;
    int h = ui->captchaArea->geometry().height();


    painter.fillRect(x, y, w, h, Qt::white);

    painter.setFont(QFont("Comic Sans MS", 16));


    for(int i=0; i<150; i++)
    {
        painter.setPen(m_colors[i%4]);
        painter.drawPoint(x + qrand() % w, y + qrand() % h);

    }
    for(int i=0; i<4; i++)
    {
        painter.setPen(m_colors[i]);
        painter.drawText(x + w/4 * i, y, w/4, h, Qt::AlignCenter, QString(m_captcha[i]));
    }


}

void LoginDlg::mousePressEvent(QMouseEvent *event)
{
    if( event->type() == QMouseEvent::MouseButtonPress )
    {
        int x = ui->captchaArea->geometry().x() + ui->LoginForm->x() + 6;
        int y = ui->captchaArea->geometry().y() + ui->LoginForm->y();
        int w = ui->captchaArea->geometry().width() - 6;
        int h = ui->captchaArea->geometry().height();

        if( event->pos().x() >= x && event->pos().x() <= x+w &&
                event->pos().y() >= y && event->pos().y() <= y+h)
        {
            m_captcha = getCaptcha();
        }
    }
}
*/

void LoginDlg::on_loginBtn_clicked()
{

//    QString captcha = ui->captchaEdit->text().replace(" ", "");

//    if( m_captcha.toLower() == captcha.toLower() )
    {
        m_user = ui->usernameEdit->text().trimmed();
        m_pwd = ui->passwordEdit->text();

        EncryptionTransmission et;
        et.setUserName(m_user);
        et.setUserPwd(m_pwd);

        if( m_user == "")
        {
            QMessageBox::information(this, "提示", "用户名不能为空！");
        }
        else if( m_pwd == "")
        {
            QMessageBox::information(this, "提示", "密码不能为空！");
        }
        else
        {
            Http* pHttpFun = new Http();
            QString strUrl = dest_ip_and_port+"/user/login";
            strUrl = et.httpGetGenerateSign(strUrl);
            connect(pHttpFun,SIGNAL(signal_requestFinished(bool,const QString&)), //http请求结束信号
                    this,SLOT(on_loginResult(bool,const QString&)));
            qDebug() <<"Send http: "<< strUrl;
            pHttpFun->get(strUrl);

//            done(Accepted);
        }

    }
//    else
//    {
//        QMessageBox::critical(this, "错误", "验证码有误，请重新输入！");
//        m_captcha = getCaptcha();
//        ui->captchaEdit->selectAll();
//    }
}


QString LoginDlg::getCaptcha()
{
    QString ret = "";

    for(int i=0; i<4; i++)
    {
        int c = (qrand() % 2) ?  'a' : 'A';
        ret += static_cast<QChar>(c + qrand() % 26);
    }

    return ret;
}

Qt::GlobalColor* LoginDlg::getColors()
{
    static Qt::GlobalColor colors[4];

    for(int i=0; i<4; i++)
    {
        colors[i] = static_cast<Qt::GlobalColor>(2 + qrand() % 16);
    }

    return colors;
}

void LoginDlg::onTimeout()
{
    ui->timeLbl->setText(QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss"));

    m_colors = getColors();

    update();
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::on_shutdownBtn_clicked()
{
    done(Rejected);
}

void LoginDlg::on_loginResult(bool success, const QString &strResult)
{
    if( !success )
    {
        QMessageBox::information(this,"提示","登录超时！");
        return;
    }
    QString rslt, reason;
    jsonParse(strResult, rslt, reason);

    if(rslt == "success")
    {
        done(Accepted);
    }else{
        showInformationMessage(QString("登录失败！\n"+reason+"。"));
    }
}
bool LoginDlg::jsonParse(const QString& strResult, QString& rslt, QString& reason)
{
    bool ret = true;
    QString str = "default";
    QString buf = strResult;
    QByteArray data = buf.toUtf8();

    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);
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
                    rslt = value.toString();
                    str = "ok";
                }
            }

            //是否含有key  reason
            if(rootObj.contains("reason"))
            {
                QJsonValue value = rootObj.value("reason");
                if (value.isString())
                {
                    reason = value.toString();
                    str = "ok";
                }
            }
        }
        else
        {
            str = "json is not object";
            ret = false;
        }
    }
    qDebug() <<"Json Parse Result: "<< str;
    return ret;
}
void LoginDlg::showInformationMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("提示");
    msg.setText(message);
    msg.setIcon(QMessageBox::Information);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, QString("确定"));
//    QTimer::singleShot(1000, &msg,SLOT(accept()));

    msg.exec();
}
