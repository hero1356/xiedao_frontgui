#ifndef ADDCARDDLG_H
#define ADDCARDDLG_H

#include <QDialog>
#include "Http.h"
#include "AppConfig.h"

namespace Ui {
class AddCardDlg;
}

class AddCardDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddCardDlg(QWidget *parent = 0);
    ~AddCardDlg();

private:
    Ui::AddCardDlg *ui;

    QString dest_ip_and_port;
    bool m_isAddCardSuccess;
    QString m_AddFailedReason;

public:
    bool isAddCardSuccedd()   {return m_isAddCardSuccess;}
    QString AddFailedReason() {return m_AddFailedReason;}
    void postProcess();
    void showErrorMessage(QString message);
    void showInformationMessage(QString message);
signals:
    void signal_addCardSuccessed(QString cardID, QString cardSN);
protected slots:
    void on_OkBtn_clicked();
    QString slot_cardAddResult(bool success,const QString& strResult);
    void slot_receCardID(unsigned int cardID);
};

#endif // ADDCARDDLG_H
