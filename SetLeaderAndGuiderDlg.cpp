#include "SetLeaderAndGuiderDlg.h"
#include "ui_SetLeaderAndGuiderDlg.h"

SetLeaderAndGuiderDlg::SetLeaderAndGuiderDlg(QStringList list, QWidget *parent ): QDialog(parent,Qt::WindowCloseButtonHint), ui(new Ui::SetLeaderAndGuiderDlg)
{
    ui->setupUi(this);
    for(int i=0; i<list.count(); i++)
    {
        ui->m_guiderCb->addItem(list[i]);
    }
}


SetLeaderAndGuiderDlg::~SetLeaderAndGuiderDlg()
{
    delete ui;
}

QString SetLeaderAndGuiderDlg::guider()
{
    return ui->m_guiderCb->currentText();
}

void SetLeaderAndGuiderDlg::on_m_okBtn_clicked()
{
    done(Accepted);
}

void SetLeaderAndGuiderDlg::on_m_skipBtn_clicked()
{
    done(Rejected);
}
