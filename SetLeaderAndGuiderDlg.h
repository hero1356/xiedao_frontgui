#ifndef SETLEADERANDGUIDERDLG_H
#define SETLEADERANDGUIDERDLG_H

#include <QDialog>

namespace Ui {
class SetLeaderAndGuiderDlg;
}

class SetLeaderAndGuiderDlg : public QDialog
{
    Q_OBJECT

public:
//    explicit SetLeaderAndGuiderDlg(QWidget *parent = 0);
    explicit SetLeaderAndGuiderDlg(QStringList list, QWidget *parent = 0);
    ~SetLeaderAndGuiderDlg();

    QString guider();

private slots:
    void on_m_okBtn_clicked();

    void on_m_skipBtn_clicked();

private:
    Ui::SetLeaderAndGuiderDlg *ui;
};

#endif // SETLEADERANDGUIDERDLG_H
