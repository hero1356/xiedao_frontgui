#ifndef MODPERSONINFODLG_H
#define MODPERSONINFODLG_H

#include <QDialog>

namespace Ui {
class ModPersonInfoDlg;
}

class ModPersonInfoDlg : public QDialog
{
    Q_OBJECT

    QString m_username;
    QString m_age;
    QString m_telephone;
    QString m_cardid;
    QString m_ismodcard;


    QString old_cardid;

public:
    explicit ModPersonInfoDlg(QWidget *parent = 0);
    ~ModPersonInfoDlg();

    QString username();
    QString age();
    QString telephone();
    QString cardid();
    QString ismodcard();

    void set_username(QString username);
    void set_age(QString age);
    void set_telephone(QString telephone);
    void set_cardid(QString cardid);



private slots:
    void on_ok_clicked();

    void on_cancel_clicked();

    void slot_cardid(uint cardid);

    void on_cardidEdit_textChanged(const QString &cardid);

private:
    Ui::ModPersonInfoDlg *ui;
};

#endif // MODPERSONINFODLG_H
