#include "ModGroupInfoDlg.h"
#include "ui_ModGroupInfoDlg.h"

ModGroupInfoDlg::ModGroupInfoDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ModGroupInfoDlg)
{
    ui->setupUi(this);
}

ModGroupInfoDlg::~ModGroupInfoDlg()
{
    delete ui;
}

void ModGroupInfoDlg::on_ok_clicked()
{
    m_groupname = ui->old_groupname->text().trimmed();
    m_teamname = ui->old_teamname->text().trimmed();
    done(Accepted);
}

void ModGroupInfoDlg::on_cancel_clicked()
{
    done(Rejected);
}

void ModGroupInfoDlg::setTeamname(QString teamname)
{
    ui->old_teamname->setText(teamname);
}

void ModGroupInfoDlg::setGroupname(QString groupname)
{
    ui->old_groupname->setText(groupname);
}

QString ModGroupInfoDlg::getTeamname()
{
    return m_teamname;
}

QString ModGroupInfoDlg::getGroupname()
{
    return m_groupname;
}


