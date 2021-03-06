#ifndef ADDCARDDLG_H
#define ADDCARDDLG_H

#include <QDialog>
#include "Http.h"
#include "AppConfig.h"
#include "EncryptionTransmission.h"


namespace Ui {
class AddCardDlg;
}

class AddCardDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddCardDlg(QString userName, QString userPwd, QWidget *parent = 0);
    ~AddCardDlg();

private:
    Ui::AddCardDlg *ui;

    QString dest_ip_and_port;
    bool m_isAddCardSuccess;
    QString m_AddFailedReason;
    EncryptionTransmission m_et;

public:
    bool isAddCardSuccedd()   {return m_isAddCardSuccess;}
    QString AddFailedReason() {return m_AddFailedReason;}
    void postProcess();
    void showErrorMessage(QString message);
    void showInformationMessage(QString message);
    QString hex2dec(QString src);
    QString dec2hex(QString src);
signals:
    void signal_addCardSuccessed(QString cardID, QString cardSN);
protected slots:
    void on_OkBtn_clicked();
    QString slot_cardAddResult(bool success,const QString& strResult);
    void slot_receCardID(QString cardID);
};

#endif // ADDCARDDLG_H
