#include "ModPersonInfoDlg.h"
#include "ui_ModPersonInfoDlg.h"

ModPersonInfoDlg::ModPersonInfoDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ModPersonInfoDlg)
{
    ui->setupUi(this);
    m_ismodcard = "no";
}

ModPersonInfoDlg::~ModPersonInfoDlg()
{
    delete ui;
}

void ModPersonInfoDlg::on_ok_clicked()
{
    m_username = ui->usernameEdit->text().trimmed();
    m_age = ui->ageEdit->text().trimmed();
    m_telephone = ui->telephoneEdit->text().trimmed();
    m_cardid = ui->cardidEdit->text().trimmed();
    done(Accepted);
}

void ModPersonInfoDlg::on_cancel_clicked()
{
    done(Rejected);
}

void ModPersonInfoDlg::slot_cardid(uint cardid)
{
    ui->cardidEdit->setText(QString::number(cardid,10));

    if(cardid != old_cardid)
        m_ismodcard = "yes";
}

void ModPersonInfoDlg::set_username(QString username)
{
    ui->usernameEdit->setText(username);
}

void ModPersonInfoDlg::set_age(QString age)
{
    bool ok;
    int ret = age.toInt(&ok, 10);
    if( ok )
        ui->ageEdit->setValue(ret);
}

void ModPersonInfoDlg::set_telephone(QString telephone)
{
    ui->telephoneEdit->setText(telephone);
}

void ModPersonInfoDlg::set_cardid(QString cardid)
{
    ui->cardidEdit->setText(cardid);
    old_cardid = cardid;
}

QString ModPersonInfoDlg::username()
{
    return m_username;
}

QString ModPersonInfoDlg::age()
{
    return m_age;
}

QString ModPersonInfoDlg::telephone()
{
    return m_telephone;
}

QString ModPersonInfoDlg::cardid()
{
    return m_cardid;
}
QString ModPersonInfoDlg::ismodcard()
{
    return m_ismodcard;
}

void ModPersonInfoDlg::on_cardidEdit_textChanged(const QString &cardid)
{

}
