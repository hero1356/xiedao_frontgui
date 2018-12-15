#ifndef MODCARDINFODLG_H
#define MODCARDINFODLG_H

#include <QDialog>


namespace Ui {
class ModCardInfoDlg;
}

class ModCardInfoDlg : public QDialog
{
    Q_OBJECT

    QString m_cardID;
    QString m_cardSN;

public:
    explicit ModCardInfoDlg(QWidget *parent = 0);
    ~ModCardInfoDlg();
    void setCardID(QString cardID);
    void setCardSN(QString cardSN);
    QString cardID(){return m_cardID;}
    QString cardSN(){return m_cardSN;}


private slots:
    void on_okButton_clicked();
    void slot_receiveCardID(unsigned int cardID);

private:
    Ui::ModCardInfoDlg *ui;
};

#endif // MODCARDINFODLG_H
