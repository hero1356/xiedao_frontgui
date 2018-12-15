#ifndef USERINPUTDLG_H
#define USERINPUTDLG_H

#include <QDialog>
#include <QDate>
#include "PersonInfo.h"
#include <QDebug>

namespace Ui {
class UserInputDlg;
}

class UserInputDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserInputDlg(QWidget *parent = 0);
    ~UserInputDlg();
    void showErrorMessage(QString message);

private slots:
    void on_m_inportBtn_clicked();

private:
    Ui::UserInputDlg *ui;
signals:
    void signal_sendPersonInfo(PersonInfo info);
};

#endif // USERINPUTDLG_H
