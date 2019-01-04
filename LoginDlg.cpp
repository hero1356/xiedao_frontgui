#include "LoginDlg.h"
#include "ui_LoginDlg.h"
#include <QTime>
#include <QDateTime>
#include <QPainter>
#include <QFont>
#include <QMessageBox>


LoginDlg::LoginDlg(QWidget *parent): QDialog(parent, Qt::FramelessWindowHint ),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);

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
            done(Accepted);
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
