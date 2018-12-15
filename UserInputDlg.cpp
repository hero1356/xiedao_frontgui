#include "UserInputDlg.h"
#include "ui_UserInputDlg.h"
#include <QMessageBox>
#include <QTimer>

UserInputDlg::UserInputDlg(QWidget *parent) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::UserInputDlg)
{
    ui->setupUi(this);

    QDate date = QDate::currentDate();
    ui->m_playtimeEdit->setDate(date);

}

UserInputDlg::~UserInputDlg()
{
    delete ui;
}

void UserInputDlg::on_m_inportBtn_clicked()
{

    QString username = ui->m_usernameEdit->text().trimmed();
    QString age = ui->m_ageEdit->text();
    QString telephone = ui->m_telephoneEdit->text().trimmed();
    QString identitycard = ui->m_identitycardEdit->text().trimmed();
    QString teamname = ui->m_teamnameEdit->text().trimmed();
    QString groupname = ui->m_groupnameEdit->text().trimmed();
    QString playtime = ui->m_playtimeEdit->text().trimmed();

    if(username == "" )
    {
        showErrorMessage("请输入用户名！");
        return;
    }
    if(teamname == "" )
    {
        showErrorMessage("请输入团队名称！");
        return;
    }
    if( groupname == "")
    {
        showErrorMessage("请输入分组名称！");
        return;
    }


    PersonInfo info(username, age, telephone, identitycard, playtime, teamname, groupname);
    emit signal_sendPersonInfo(info);

    ui->m_usernameEdit->clear();
    ui->m_telephoneEdit->clear();
    ui->m_identitycardEdit->clear();

    ui->m_usernameEdit->setFocus();

}


void UserInputDlg::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("错误");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setButtonText(QMessageBox::Ok, QString("确定"));
    QTimer::singleShot(1000, &msg,SLOT(accept()));
    msg.setStyleSheet("QLabel{color: black}");

    msg.exec();
}
