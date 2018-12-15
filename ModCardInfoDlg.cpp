#include "ModCardInfoDlg.h"
#include "ui_ModCardInfoDlg.h"

ModCardInfoDlg::ModCardInfoDlg(QWidget *parent) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    ui(new Ui::ModCardInfoDlg)
{
    ui->setupUi(this);
    QRegExp regExp("[A-Z][0-9][0-9][0-9]");
    ui->cardSNEdit->setValidator(new QRegExpValidator(regExp,this));
}

ModCardInfoDlg::~ModCardInfoDlg()
{
    delete ui;
}

void ModCardInfoDlg::setCardID(QString cardID)
{
    ui->cardIDEdit->setText(cardID);
}

void ModCardInfoDlg::setCardSN(QString cardSN)
{
    ui->cardSNEdit->setText(cardSN);
}


void ModCardInfoDlg::on_okButton_clicked()
{
    m_cardID = ui->cardIDEdit->text().trimmed();
    m_cardSN = ui->cardSNEdit->text().trimmed();
    done(Accepted);
}

void ModCardInfoDlg::slot_receiveCardID(unsigned int cardID)
{
    ui->cardIDEdit->setText(QString::number(cardID));
}
